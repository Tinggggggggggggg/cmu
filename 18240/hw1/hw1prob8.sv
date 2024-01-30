`default_nettype none
module hw1prob7_test;

    logic a, b, c, d, prime, div3;

    hw1prob7 DUT( .a, .b, .c, .d, .prime, .div3 );

    initial begin
        $monitor("a=%b, b=%b, c=%b, d=%b, prime=%b, div3=%b",
                 a, b, c, d, prime, div3);

        for (int i = 0; i < 16; i++) begin
            {a, b, c, d} = i;
            #5;
        end

        #5 $finish;
    end

endmodule: hw1prob7_test


