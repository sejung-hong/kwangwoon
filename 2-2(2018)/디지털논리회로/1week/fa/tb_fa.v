`timescale 1ns/100ps 

module tb_fa; //fulladder 검증 
	reg tb_a, tb_b, tb_ci; //input
	wire tb_s, tb_co; // output
	
	fa U0_fa(.a(tb_a), .b(tb_b), .ci(tb_ci), .s(tb_s), .co(tb_co));
	
	initial
	begin
	
	tb_a = 0; tb_b = 0; tb_ci = 0; //초기값 abc=000
	#10; tb_a = 0; tb_b = 0; tb_ci = 1;
	#10; tb_a = 0; tb_b = 1; tb_ci = 0;
	#10; tb_a = 0; tb_b = 1; tb_ci = 1;
	#10; tb_a = 1; tb_b = 0; tb_ci = 0;
	#10; tb_a = 1; tb_b = 0; tb_ci = 1;
	#10; tb_a = 1; tb_b = 1; tb_ci = 0;
	#10; tb_a = 1; tb_b = 1; tb_ci = 1;
	#10; $stop;
		
	end
endmodule
