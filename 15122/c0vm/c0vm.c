/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "lib/xalloc.h"
#include "lib/stack.h"
#include "lib/contracts.h"
#include "lib/c0v_stack.h"
#include "lib/c0vm.h"
#include "lib/c0vm_c0ffi.h"
#include "lib/c0vm_abort.h"

/* call stack frames */
typedef struct frame_info frame;
struct frame_info
{
  c0v_stack_t S; /* Operand stack of C0 values */
  ubyte *P;      /* Function body */
  size_t pc;     /* Program counter */
  c0_value *V;   /* The local variables */
};

int execute(struct bc0_file *bc0)
{
  REQUIRES(bc0 != NULL);

  /* Variables */
  c0v_stack_t S = c0v_stack_new();                                             /* Operand stack of C0 values */
  ubyte *P = ((bc0->function_pool)[0]).code;                                   /* Array of bytes that make up the current function */
  size_t pc = 0;                                                               /* Current location within the current byte array P */
  c0_value *V = xcalloc(sizeof(c0_value), ((bc0->function_pool)[0]).num_vars); /* Local variables (you won't need this till Task 2) */
  // (void)V;                                                                     // silences compilation errors about V being currently unused

  /* The call stack, a generic stack that should contain pointers to frames */
  /* You won't need this until you implement functions. */
  gstack_t callStack = stack_new();
  // (void)callStack; // silences compilation errors about callStack being currently unused

  while (true)
  {

#ifdef DEBUG
    /* You can add extra debugging information here */
    fprintf(stderr, "Opcode %x -- Stack size: %zu -- PC: %zu\n",
            P[pc], c0v_stack_size(S), pc);
#endif

    switch (P[pc])
    {

      /* Additional stack operation: */

    case POP:
    {
      pc++;
      c0v_pop(S);
      break;
    }

    case DUP:
    {
      pc++;
      c0_value v = c0v_pop(S);
      c0v_push(S, v);
      c0v_push(S, v);
      break;
    }

    case SWAP:
    {
      pc++;
      c0_value val1 = c0v_pop(S);
      c0_value val2 = c0v_pop(S);
      c0v_push(S, val1);
      c0v_push(S, val2);
      break;
    }

      /* Returning from a function.
       * This currently has a memory leak! You will need to make a slight
       * change for the initial tasks to avoid leaking memory.  You will
       * need to revise it further when you write INVOKESTATIC. */

    case RETURN:
    {
      if (stack_empty(callStack))
      {
        int retval = val2int(c0v_pop(S));
        ASSERT(c0v_stack_empty(S));
        // Another way to print only in DEBUG mode
        IF_DEBUG(fprintf(stderr, "Returning %d from execute()\n", retval));
        // Free everything before returning from the execute function!
        free(V);
        c0v_stack_free(S);
        // How to free callStack; NO need
        stack_free(callStack, NULL);
        return retval;
      }
      else
      {
        c0_value val = c0v_pop(S);
        // frame *frm = (frame *)pop(callStack);
        // free(V);
        c0v_stack_free(S);
        free(V);
        frame *frm = (frame *)pop(callStack);
        S = frm->S;
        P = frm->P;
        V = frm->V;
        pc = frm->pc;
        c0v_push(S, val);
        free(frm);
        break;
      }
    }

      /* Arithmetic and Logical operations */

    case IADD:
    {
      int num2 = val2int(c0v_pop(S));
      int num1 = val2int(c0v_pop(S));
      // if (num1 + num2 > int_max() || num1 + num2 < int_min())
      // {
      //   c0_arith_error("overflow");
      // }
      c0_value res = int2val(num1 + num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case ISUB:
    {
      int num2 = val2int(c0v_pop(S));
      int num1 = val2int(c0v_pop(S));
      // if (num1 - num2 > int_max() || num1 - num2 < int_min())
      // {
      //   c0_arith_error("overflow");
      // }
      c0_value res = int2val(num1 - num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case IMUL:
    {
      int num2 = val2int(c0v_pop(S));
      int num1 = val2int(c0v_pop(S));
      c0_value res = int2val(num1 * num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case IDIV:
    {
      int num2 = val2int(c0v_pop(S));
      if (num2 == 0)
      {
        c0_arith_error("denominator 0");
      }
      int num1 = val2int(c0v_pop(S));
      // overflow ?
      if (num2 == -1 && num1 == -2147483648)
      {
        c0_arith_error("overflow");
      }
      c0_value res = int2val(num1 / num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case IREM:
    {
      int num2 = val2int(c0v_pop(S));
      if (num2 == 0)
      {
        c0_arith_error("denominator 0");
      }
      int num1 = val2int(c0v_pop(S));
      // overflow ?
      if (num2 == -1 && num1 == -2147483648)
      {
        c0_arith_error("overflow");
      }
      c0_value res = int2val(num1 % num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case IAND:
    {
      int num2 = val2int(c0v_pop(S));
      int num1 = val2int(c0v_pop(S));
      c0_value res = int2val(num1 & num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case IOR:
    {
      int num2 = val2int(c0v_pop(S));
      int num1 = val2int(c0v_pop(S));
      c0_value res = int2val(num1 | num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case IXOR:
    {
      int num2 = val2int(c0v_pop(S));
      int num1 = val2int(c0v_pop(S));
      c0_value res = int2val(num1 ^ num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }
      // case ISHR:
      // {
      //   pc++;
      //   int num2 = val2int(c0v_pop(S));
      //   if (num2 < 0 || num2 > 31)
      //     c0_arith_error("shifffffft illiano");
      //   int num1 = val2int(c0v_pop(S));
      //   c0v_push(S, int2val(num1 >> num2));
      //   break;
      // }
    case ISHR:
    {
      int num2 = val2int(c0v_pop(S));
      if (num2 < 0 || num2 >= 32)
      {
        c0_arith_error("over shift");
      }
      int num1 = val2int(c0v_pop(S));
      // overflow???
      c0_value res = int2val(num1 >> num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case ISHL:
    {
      int num2 = val2int(c0v_pop(S));
      if (num2 < 0 || num2 >= 32)
      {
        c0_arith_error("over shift");
      }
      int num1 = val2int(c0v_pop(S));
      // overflow???
      c0_value res = int2val(num1 << num2);
      c0v_push(S, res);
      pc += 1;
      break;
    }

      /* Pushing constants */

    case BIPUSH:
    {
      pc += 1;
      char i = (char)(P[pc]);
      pc += 1;
      c0_value res = int2val((int)i);
      // pc += 1;
      c0v_push(S, res);
      // pc += 1;
      break;
    }

    case ILDC:
    {
      pc += 1;
      int val1 = P[pc];
      pc += 1;
      int val2 = P[pc];
      int index = val1 << 8 | val2;
      c0_value res = int2val(((bc0->int_pool)[index]));
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case ALDC:
    {
      pc += 1;
      int val1 = P[pc];
      pc += 1;
      int val2 = P[pc];
      int index = (val1 << 8) | val2;
      c0_value res = ptr2val((void *)(&(bc0->string_pool[index])));
      c0v_push(S, res);
      pc += 1;
      break;
    }

    case ACONST_NULL:
    {
      c0v_push(S, ptr2val(NULL));
      pc += 1;
      break;
    }

      /* Operations on local variables */

    case VLOAD:
    {
      pc += 1;
      int index = P[pc];
      c0v_push(S, V[index]);
      pc += 1;
      break;
    }

    case VSTORE:
    {
      pc += 1;
      int index = P[pc];
      V[index] = c0v_pop(S);
      pc += 1;
      break;
    }

      /* Assertions and errors */

    case ATHROW:
    {
      char *error = (char *)val2ptr(c0v_pop(S));
      c0_user_error(error);
      pc += 1;
      break;
    }

    case ASSERT:
    {
      char *error = (char *)val2ptr(c0v_pop(S));
      int x = val2int(c0v_pop(S));
      if (x == 0)
      {
        c0_assertion_failure(error);
      }
      pc += 1;
      break;
    }

      /* Control flow operations */

    case NOP:
    {
      pc += 1;
      break;
    }

    case IF_CMPEQ:
    {
      // pc += 1;
      c0_value val1 = c0v_pop(S);
      c0_value val2 = c0v_pop(S);
      if (val_equal(val1, val2))
      {
        unsigned int v1 = (unsigned int)P[pc + 1];
        unsigned int v2 = (unsigned int)P[pc + 2];
        pc += (int16_t)((uint16_t)((v1 << 8) | v2));
      }
      else
      {
        pc += 3;
      }
      break;
    }

    case IF_CMPNE:
    {
      // pc += 1;
      c0_value val1 = c0v_pop(S);
      c0_value val2 = c0v_pop(S);
      if (!val_equal(val1, val2))
      {
        unsigned int v1 = (unsigned int)P[pc + 1];
        unsigned int v2 = (unsigned int)P[pc + 2];
        pc += (int16_t)((uint16_t)((v1 << 8) | v2));
      }
      else
      {
        pc += 3;
      }
      break;
    }

    case IF_ICMPLT:
    {
      // pc += 1;
      c0_value val2 = c0v_pop(S);
      c0_value val1 = c0v_pop(S);
      if (val2int(val1) < val2int(val2))
      {
        unsigned int v1 = (unsigned int)P[pc + 1];
        unsigned int v2 = (unsigned int)P[pc + 2];
        pc += (int16_t)((uint16_t)((v1 << 8) | v2));
      }
      else
      {
        pc += 3;
      }
      break;
    }

    case IF_ICMPGE:
    {
      // pc += 1;
      c0_value val2 = c0v_pop(S);
      c0_value val1 = c0v_pop(S);
      if (val2int(val1) >= val2int(val2))
      {
        unsigned int v1 = (unsigned int)P[pc + 1];
        unsigned int v2 = (unsigned int)P[pc + 2];
        pc += (int16_t)((uint16_t)((v1 << 8) | v2));
      }
      else
      {
        pc += 3;
      }
      break;
    }

    case IF_ICMPGT:
    {
      // pc += 1;
      c0_value val2 = c0v_pop(S);
      c0_value val1 = c0v_pop(S);
      if (val2int(val1) > val2int(val2))
      {
        unsigned int v1 = (unsigned int)P[pc + 1];
        unsigned int v2 = (unsigned int)P[pc + 2];
        pc += (int16_t)((uint16_t)((v1 << 8) | v2));
      }
      else
      {
        pc += 3;
      }
      break;
    }

    case IF_ICMPLE:
    {
      // pc += 1;
      c0_value val2 = c0v_pop(S);
      c0_value val1 = c0v_pop(S);
      if (val2int(val1) <= val2int(val2))
      {
        unsigned int v1 = (unsigned int)P[pc + 1];
        unsigned int v2 = (unsigned int)P[pc + 2];
        pc += (int16_t)((uint16_t)((v1 << 8) | v2));
      }
      else
      {
        pc += 3;
      }
      break;
    }

    case GOTO:
    {
      unsigned int v1 = (unsigned int)P[pc + 1];
      unsigned int v2 = (unsigned int)P[pc + 2];
      pc += (int16_t)((uint16_t)((v1 << 8) | v2));
      break;
    }

      /* Function call operations: */

    case INVOKESTATIC:
    {
      pc += 1;
      unsigned int v1 = (unsigned int)P[pc];
      pc += 1;
      unsigned int v2 = (unsigned int)P[pc];
      pc += 1;
      frame *new = xmalloc(sizeof(frame));
      new->P = P;
      new->V = V;
      new->S = S;
      new->pc = pc;
      push(callStack, (void *)new);
      // restore
      pc = 0;
      int index = (v1 << 8) | v2;
      struct function_info f = bc0->function_pool[index];
      P = f.code;
      V = xcalloc(sizeof(c0_value), f.num_vars);
      for (int i = 0; i < f.num_args; i++)
      {
        V[(f.num_args - 1) - i] = c0v_pop(S);
      }
      S = c0v_stack_new();
      break;
    }

    case INVOKENATIVE:
    {
      pc += 1;
      unsigned int v1 = (unsigned int)P[pc];
      pc += 1;
      unsigned int v2 = (unsigned int)P[pc];
      pc += 1;
      int index = (v1 << 8) | v2;
      struct native_info f = bc0->native_pool[index];
      // P = f.code;
      c0_value *val_array = xcalloc(sizeof(c0_value), f.num_args);
      for (int i = 0; i < f.num_args; i++)
      {
        val_array[(f.num_args - 1) - i] = c0v_pop(S);
      }
      c0_value elem = (*(native_function_table[f.function_table_index]))(val_array);
      c0v_push(S, elem);
      free(val_array);
      break;
    }

      /* Memory allocation and access operations: */

    case NEW:
    {
      pc += 1;
      void *new = xcalloc(P[pc], 1);
      c0v_push(S, ptr2val(new));
      pc += 1;
      break;
    }

    case IMLOAD:
    {
      // pc += 1;
      int *val = (int *)(val2ptr(c0v_pop(S)));
      if (val == NULL)
      {
        c0_memory_error("deferencing NULL");
      }
      c0v_push(S, int2val(*val));
      pc += 1;
      break;
    }

    case IMSTORE:
    {
      // pc += 1;
      int val = val2int(c0v_pop(S));
      int *store = (int *)val2ptr(c0v_pop(S));
      if (store == NULL)
      {
        c0_memory_error("deferencing NULL");
      }
      *store = val;
      pc += 1;
      break;
      // c0v_push(S,)
    }

    case AMLOAD:
    {
      // pc += 1;
      void **val = val2ptr(c0v_pop(S));
      if (val == NULL)
      {
        c0_memory_error("deferencing NULL");
      }
      c0v_push(S, ptr2val(*val));
      pc += 1;
      break;
    }

    case AMSTORE:
    {
      // pc += 1;
      void *val = val2ptr(c0v_pop(S));
      void **store = val2ptr(c0v_pop(S));
      if (store == NULL)
      {
        c0_memory_error("deferencing NULL");
      }
      *store = val;
      pc += 1;
      break;
      // c0v_push(S,)
    }

    case CMLOAD:
    {
      // type of val
      char *val = (char *)(val2ptr(c0v_pop(S)));
      if (val == NULL)
      {
        c0_memory_error("deferencing NULL");
      }
      c0v_push(S, int2val(*val));
      pc += 1;
      break;
    }

    case CMSTORE:
    {
      // pc += 1;
      char val = (char)val2int(c0v_pop(S));
      char *store = (char *)val2ptr(c0v_pop(S));
      if (store == NULL)
      {
        c0_memory_error("deferencing NULL");
      }
      *store = val;
      pc += 1;
      break;
      // c0v_push(S,)
    }

    case AADDF:
    {
      pc += 1;
      unsigned char num = P[pc];
      pc += 1;
      char *elem = (char *)(val2ptr(c0v_pop(S)));
      if (elem == NULL)
      {
        c0_memory_error("deferencing NULL");
      }
      c0v_push(S, ptr2val((void *)(elem + num)));
      break;
    }

      /* Array operations: */

    case NEWARRAY:
    {
      pc += 1;
      unsigned char s = P[pc];
      pc += 1;
      unsigned int n = val2int(c0v_pop(S));

      // ********************************************************
      // whether valid here for comparison
      // case when n = 0, null
      // ********************************************************
      if ((int)n < 0)
      {
        c0_memory_error("negative elems");
      }

      c0_array *array = xcalloc(sizeof(c0_array), 1);
      // c0_array elem;
      array->count = (int)n;
      array->elt_size = (int)s;
      array->elems = (void *)(xcalloc(sizeof(char), n * s));
      c0v_push(S, ptr2val(array));
      break;
    }

    case ARRAYLENGTH:
    {
      c0_array *array = val2ptr(c0v_pop(S));
      c0v_push(S, int2val(array->count));
      pc += 1;
      break;
    }

    case AADDS:
    {
      pc += 1;
      unsigned int i = val2int(c0v_pop(S));
      c0_array *array = val2ptr(c0v_pop(S));
      if (array == NULL)
      {
        c0_memory_error("Invalid array");
      }
      if ((i >= array->count) | ((int)i < 0))
      {
        c0_memory_error("Invalid index");
      }
      char *elem = (char *)(array->elems);
      void *pelem = (void *)(elem + (array->elt_size) * i);
      c0v_push(S, ptr2val(pelem));
      break;
    }

      /* BONUS -- C1 operations */

    case CHECKTAG:

    case HASTAG:

    case ADDTAG:

    case ADDROF_STATIC:

    case ADDROF_NATIVE:

    case INVOKEDYNAMIC:

    default:
      fprintf(stderr, "invalid opcode: 0x%02x\n", P[pc]);
      abort();
    }
  }

  /* cannot get here from infinite loop */
  assert(false);
}
