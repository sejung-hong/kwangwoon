module cal_flags32(op, result, co_add, co_prev_add, co_sub, co_prev_sub, c, n, z, v);

input [2:0] op;
input [31:0] result;
input co_add, co_prev_add, co_sub, co_prev_sub;
output c,n,z,v;

assign c=(op[2:1]!=2'b11)?1'b0:((op[0]==1'b0)?co_add:co_sub);
//op=110 return co_add, op=111 return co_sub
assign n=result[31];
//return result
assign z=(result==32'b00000000000000000000000000000000)?1'b1:1'b0;

assign v=(op[2:1]!=2'b11)?1'b0:((op[0]==1'b0)?co_add^co_prev_add:co_sub^co_prev_sub);
//op=110 return co_add^c3add, op=111 return co_sub^c3_sub
//assign conditional operator

endmodule 