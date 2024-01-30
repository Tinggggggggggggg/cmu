/**************************************************************************/
/*              COPYRIGHT Carnegie Mellon University 2023                 */
/* Do not post this file or any derivative on a public site or repository */
/**************************************************************************/
/* Huffman coding
 *
 * 15-122 Principles of Imperative Computation
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#include "lib/contracts.h"
#include "lib/xalloc.h"
#include "lib/file_io.h"
#include "lib/pq.h"

#include "freqtable.h"
#include "htree.h"
#include "encode.h"
#include "bitpacking.h"

// Print error message
void error(char *msg)
{
  fprintf(stderr, "%s\n", msg);
  exit(1);
}

/* in htree.h: */
/* typedef struct htree_node htree; */
struct htree_node
{
  symbol_t value;
  unsigned int frequency;
  htree *left;
  htree *right;
};

/**********************************************/
/* Task 1: Checking data structure invariants */
/**********************************************/

/* forward declaration -- DO NOT MODIFY */
bool is_htree(htree *H);

bool is_htree_leaf(htree *H)
{
  return H != NULL && (H->frequency > 0) && (H->left == NULL) && (H->right == NULL);
}

bool is_htree_interior(htree *H)
{
  return H != NULL && is_htree(H->left) && is_htree(H->right) && (H->frequency == H->left->frequency + H->right->frequency);
}

bool is_htree(htree *H)
{
  return H != NULL && (is_htree_leaf(H) || is_htree_interior(H));
}

/********************************************************/
/* Task 2: Building Huffman trees from frequency tables */
/********************************************************/

/* for lib/pq.c */
bool htree_higher_priority(elem e1, elem e2)
{
  return ((htree *)e1)->frequency < ((htree *)e2)->frequency;
}

int create_pq(pq_t pq, freqtable_t table)
{
  int count = 0;
  int i = 0;
  while (i < NUM_SYMBOLS)
  {
    if (table[i] > 0)
    {
      htree *node = xcalloc(1, sizeof(htree));
      node->frequency = table[i];
      node->value = i;
      pq_add(pq, (elem)node);
      count += 1;
    }
    i += 1;
  }
  return count;
}

// build a htree from a frequency table
htree *build_htree(freqtable_t table)
{
  pq_t pq = pq_new(NUM_SYMBOLS, &htree_higher_priority, NULL);
  // int count = 0;
  int count = create_pq(pq, table);
  printf("%d", count);
  if (count < 2)
  {
    error("useless");
  }
  while (count > 1)
  {
    htree *left = (htree *)(pq_rem(pq));
    htree *right = (htree *)(pq_rem(pq));
    htree *push_new = xcalloc(1, sizeof(htree));
    push_new->frequency = left->frequency + right->frequency;
    push_new->left = left;
    push_new->right = right;
    pq_add(pq, (elem)push_new);
    count -= 1;
  }
  htree *res = (htree *)(pq_rem(pq));
  pq_free(pq);
  return res;
}

/*******************************************/
/*  Task 3: decoding a text                */
/*******************************************/
// Initialize a pointer to the root of the Huffman tree.

// Repeatedly read bits from the bit string and update the pointer: when you
// read a 0, follow the left branch, and when you read a 1, follow the right
// branch.

// Whenever you reach a leaf, output the symbol at that leaf and reset the
// pointer to the root of the Huffman tree.

// Decode code according to H, putting decoded length in src_len

void get_arraylen(htree *H, bit_t *code, size_t *src_len)
{
  htree *temp = H;
  // size_t *bit = xcalloc(1, sizeof(size_t));
  size_t length = 0;
  for (size_t i = 0; code[i] != '\0'; i++)
  {
    if (code[i] == '1')
    {
      temp = temp->right;
    }
    else if (code[i] == '0')
    {
      temp = temp->left;
    }
    else
    {
      error("wrong input");
    }
    if (is_htree_leaf(temp))
    {
      length += 1;
      temp = H;
    }
  }

  *src_len = length;
  // free(bit);
}

void decode_chars(htree *H, symbol_t *res, bit_t *code)
{
  htree *temp = H;
  // size_t *bit = xcalloc(1, sizeof(size_t));
  int index = 0;
  for (size_t i = 0; code[i] != '\0'; i++)
  {
    if (code[i] == '1')
    {
      temp = temp->right;
    }
    else if (code[i] == '0')
    {
      temp = temp->left;
    }
    else
    {
      error("wrong input");
    }
    if (is_htree_leaf(temp))
    {
      res[index] = temp->value;
      index += 1;
      temp = H;
    }
    // *bit += 1;
  }
  // free(bit);
}

symbol_t *decode_src(htree *H, bit_t *code, size_t *src_len)
{
  get_arraylen(H, code, src_len);
  size_t num_chars = *src_len;
  symbol_t *res = xcalloc(num_chars, sizeof(symbol_t));
  decode_chars(H, res, code);
  return res;
}

/***************************************************/
/* Task 4: Building code tables from Huffman trees */
/***************************************************/

void populate_codetable(htree *H, codetable_t res, char *cur, int size)
{
  // char* bit = xmalloc(1*char)
  if (is_htree_leaf(H))
  {
    res[H->value] = cur;
    return;
  }

  char *left = xcalloc(size + 1, sizeof(char));
  strcpy(left, cur);
  left[size] = '\0';
  left[size - 1] = '0';
  populate_codetable(H->left, res, left, size + 1);

  char *right = xcalloc(size + 1, sizeof(char));
  strcpy(right, cur);
  right[size] = '\0';
  right[size - 1] = '1';
  populate_codetable(H->right, res, right, size + 1);

  free(cur);
}

// Returns code table for characters in H
codetable_t htree_to_codetable(htree *H)
{
  codetable_t res = xcalloc(NUM_SYMBOLS, sizeof(bitstring_t));
  char *cur = xmalloc(1 * sizeof(char));
  *cur = '\0';
  int init = 1;
  populate_codetable(H, res, cur, init);
  // free(cur);
  return res;
}

/*******************************************/
/*  Task 5: Encoding a text                */
/*******************************************/

size_t get_length(codetable_t table, symbol_t *src, size_t src_len)
{
  size_t res = 0;
  for (size_t i = 0; i < src_len; i++)
  {
    res += strlen(table[*(src + i)]);
  }
  return res;
}

// Encodes source according to codetable
bit_t *encode_src(codetable_t table, symbol_t *src, size_t src_len)
{
  size_t length = get_length(table, src, src_len);
  bit_t *res = xcalloc((length + 1), sizeof(bit_t));
  int index = 0;
  for (size_t i = 0; i < src_len; i++)
  {
    size_t j = 0;
    while (j < strlen(table[*(src + i)]))
    {
      res[index] = *(table[*(src + i)] + j);
      index += 1;
      j += 1;
    }
  }
  res[length] = '\0';
  return res;
}

/**************************************************/
/*  Task 6: Building a frequency table from file  */
/**************************************************/

// Build a frequency table from a source file (or STDIN)
freqtable_t build_freqtable(char *fname)
{
  freqtable_t res = xcalloc(NUM_SYMBOLS, sizeof(unsigned int));
  FILE *read = fopen(fname, "r");
  while (1)
  {
    int pos = fgetc(read);
    // res[pos] += 1;
    if (feof(read))
    {
      break;
    }
    res[pos] += 1;
  }
  fclose(read);
  return res;
}

/************************************************/
/*  Task 7: Packing and unpacking a bitstring   */
/************************************************/

// Pack a string of bits into an array of bytes
uint8_t *pack(bit_t *bits)
{
  size_t length = strlen(bits);
  // if (length % 8 == 0){
  //   unit8_t *res = xcalloc(length / 8, sizeof(unit8_t));
  // }
  // else{
  //   unit8_t *res = xcalloc(length / 8 + 1, sizeof(unit8_t));
  // }
  uint8_t *res = xcalloc((length + 7) / 8, sizeof(uint8_t));
  for (size_t i = 0; i < length; i++)
  {
    if (bits[i] == '1')
    {
      res[i / 8] |= (1 << (7 - i % 8));
    }
  }
  return res;
}

// Unpack an array of bytes c of length len into a NUL-terminated string of ASCII bits
bit_t *unpack(uint8_t *c, size_t len)
{
  bit_t *res = xcalloc((len * 8) + 1, sizeof(bit_t));
  // size_t index = 0;
  for (size_t i = 0; i < 8 * len; i++)
  {
    uint8_t cur = c[i / 8];
    if ((((cur >> (7 - i % 8))) & 1) == 1)
    {
      res[i] = '1';
      // index += 1;
    }
    else
    {
      res[i] = '0';
      // index += 1;
    }
    // index += 1;
  }
  return res;
}
