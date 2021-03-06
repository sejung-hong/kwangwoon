module _dff_r(clk, reset_n, d, q);
//resettable D flopflop with active low asynchrnous reset
input clk, reset_n, d;
output reg q;

always@(posedge clk or negedge reset_n)
//falling edge clk, risingedge reset_n
begin
if(reset_n==0) q <=1'b0;
else q <= d;
end
endmodule 
