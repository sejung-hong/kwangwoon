module memory(clk, cen, wen, addr, din, dout);
//address에 기반하여 데이터를 저장하는 hardware
input clk;
input cen, wen;
input [4:0] addr;
input [31:0] din;
output reg [31:0] dout;

reg [31:0] mem [0:31];

integer i; //i선언 

initial begin 
for(i=0; i<32; i=i+1)begin
mem[i] = 32'b0;
end 
//memory initialization
end 

always @ (posedge clk) //clk rising edge
begin
if (cen==1'b1 & wen==1'b1) begin
mem[addr] =din;
dout = 32'b0;
end 
//mem din write
else if (cen==1'b1 & wen==1'b0) begin
dout = mem[addr];
end 
//dout mem write
else if (cen==1'b0) begin
dout = 32'b0;
end
else dout = 32'bx;
end
//read/write performance
endmodule 
