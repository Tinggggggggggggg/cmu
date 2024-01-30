`default_nettype none // Required in every sv file
module chipInterface
    (input logic SW [17:0],
    output logic LEDR[17:0]);
    multiplexer DUT (.a(SW[0]),
                    .b(SW[1]),
                    .f(LEDR[15]),
                    .sel(SW[7]));
endmodule: chipInterface
module multiplexer
    (output logic f,
    input logic a, b, sel);
    logic f1, f2, n_sel;
    and #2 g1(f1, a, n_sel),
    g2(f2, b, sel);
    or #2 g3(f, f1, f2);
    not g4(n_sel, sel);
endmodule: multiplexer

    // TODO:
    // - Instantiate your multiplexer module
    // inside this chipInterface
    // - Hook up SW[0], SW[1] and SW[7] to a, b and
    // sel inputs on the mux
    // - Hook up LEDR[15] to the output of your multiplexer
    // - Finish the module (you'll need something else
    // to indicate your module is complete)
