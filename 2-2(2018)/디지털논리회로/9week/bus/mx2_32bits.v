module mx2_32bits(d0, d1, s, y); 
//32bits 8-to-1 multiplexer
input [31:0] d0, d1;
input s;
output [31:0] y;
assign y =(s==1'b0)?d0[31:0]:d1[31:0];
//conditional operator
endmodule 
