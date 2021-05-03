module _dff_r_en(clk, reset_n, en, d, q); //enabled D flip-flop module
input clk, reset_n, en, d;
output reg q;

always @ (posedge clk or negedge reset_n)
begin
if(~reset_n) q <= 1'b0;
else if(en)  q <= d;
else         q <= q;
end 
endmodule 