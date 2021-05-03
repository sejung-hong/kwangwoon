module _dlatch(clk, d, q, q_bar); //D latch module
input clk, d;
output q, q_bar;
wire d_bar, r, s;

_inv U0_inv(.a(d), .y(d_bar));
_and2 U0_and2(.a(clk), .b(d_bar), .y(r));
_and2 U1_and2(.a(clk), .b(d), .y(s));
_srlatch U0_srlatch(.r(r), .s(s), .q(q), .q_bar(q_bar));
//instance of inveter, and2, srlatch
endmodule 