`timescale 1ns/100ps

module tb_srlatch; //tb_srlatch module
reg tb_r, tb_s;
wire tb_q, tb_q_bar;

_srlatch U0_srlatch(.r(tb_r), .s(tb_s), .q(tb_q), .q_bar(tb_q_bar));

initial 
begin
tb_r = 0; tb_s =0;
#10; tb_r=1; tb_s=0;
#10; tb_r=0; tb_s=1;
#10; tb_r=1; tb_s=1;
#10; tb_r=1; tb_s=0;
#10; tb_r=0; tb_s=1;
#10; tb_r=0; tb_s=0;
#10; $stop;
end
//testbench
endmodule 