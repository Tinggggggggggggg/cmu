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



module muxTester
    (output logic a, b, sel,
    input logic muxOut);
    initial begin
        $monitor($time,,
            "a = %b, b = %b, sel = %b, muxOut = %b",
            a, b, sel, muxOut);
            a = 0;
            b = 0;
            sel = 0;
        #10 b = 1;
        #10 a = 1;
        #10 b = 0;
        #10 sel = 1;
        #10 b = 1;
        #10 a = 0;
        #10 b = 0;
        #10 $finish;
    end
endmodule: muxTester


module system();
    logic wire_a, wire_b, select, mux_out;
    multiplexer DUT (.a(wire_a),
                    .b(wire_b),
                    .f(mux_out),
                    .sel(select));
    muxTester mt (.a(wire_a),
                .b(wire_b),
                .muxOut(mux_out),
                .sel(select));
endmodule: system