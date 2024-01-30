`default_nettype none // Required in every sv file
module multiplexer
    (output logic f,
    input logic a, b, sel);
    logic f1, f2, n_sel;
    and #2 g1(f1, a, n_sel),
    g2(f2, b, sel);
    or #2 g3(f, f1, f2);
    not g4(n_sel, sel);
endmodule: multiplexer