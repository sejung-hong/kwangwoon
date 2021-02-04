`timescale 1ns/100ps

module test_RCA32;
 
 reg [31:0] A, B;
 reg Cin;
 wire [31:0] S;
 wire Cout;
 
RCA32 dut(.A(A), .B(B), .Cin(Cin), .S(S), .Cout(Cout));

initial
begin 
  #0 A=32'b00000000000000000000000000000001; B=32'b00000000000000000000000000000001; Cin=1'b0;
  #5 A=32'b00000000000000000000000000000001; B=32'b00000000000000000000000000000001; Cin=1'b1;
  #10 A=32'b00000000000000000000000000000010; B=32'b00000000000000000000000000000010; Cin=1'b0;
  #15 A=32'b00000000000000000000000000000010; B=32'b00000000000000000000000000000010; Cin=1'b1;
  #20 A=32'b00000000000000000000000000000011; B=32'b00000000000000000000000000000011; Cin=1'b0;
  #25 A=32'b00000000000000000000000000000011; B=32'b00000000000000000000000000000011; Cin=1'b1;
  $stop;
  end
 
 endmodule
  