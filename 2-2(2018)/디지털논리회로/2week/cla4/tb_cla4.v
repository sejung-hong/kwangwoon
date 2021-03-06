`timescale 1ns/100ps

module tb_cla4;//tb_cla4
reg [3:0] tb_a, tb_b;//4-bit register tb_a,tb_b
reg tb_ci;
wire [3:0] tb_s;//4-bit wire tb_s
wire tb_co;

wire [4:0] tb_result;//5-bit wire tb_result

assign tb_result={tb_co,tb_s};//tb_result is assinged by tb_co, tb_s

cla4 U0_cla4(.a(tb_a),.b(tb_b),.ci(tb_ci),.s(tb_s),.co(tb_co));//cla4 instance operation

initial
begin
		tb_a = 4'h0; tb_b = 4'h0; tb_ci = 0;
		#10; tb_a = 4'h5; tb_b = 4'h4;
		#10; tb_a = 4'hF; tb_b = 4'hF;
		#10; tb_ci = 1;
		#10; $stop;
end
endmodule 