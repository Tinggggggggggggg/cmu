module hw1prob7
    (input logic a, b, c, d,
    output logic prime, div3);

    logic p2, p3, p5, p7, p11, p13;
    logic d0, d3, d6, d9, d12;

    assign num1 = b & ~c & d;
    assign num2 = ~a & b & d;
    assign num3 = ~a & ~b & c;
    assign num4 = ~b & c &  d;

    assign d1 = ~a & ~b & ~c & ~d;
    assign d2 = ~a & ~b &  c &  d;
    assign d3 = ~a &  b &  c & ~d;
    assign d4 =  a & ~b & ~c &  d;
    assign d5 =  a &  b & ~c & ~d;
    assign d6 =  a &  b & c & d;

    assign prime = num1 | num2 | num3 | num4;
    assign div3 = d1 | d2 | d3 | d4 | d5 | d6;

endmodule : hw1prob7



