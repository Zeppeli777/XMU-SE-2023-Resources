//采用行为描述方式实现例题5.3的串行加法器，该电路的输入为x1、x2，状态为y，输出为Z，CP为时钟信号。

`timescale 1ns / 1ps	

module example_5_3_1(
    input cp,
    input x1, x2,                               		 	
    input y,			 
    output reg ny, z    		 	          
);

    always @(negedge cp)                      //行为描述方式

    begin
                case({y, x1, x2})
	                0: begin ny <= 0; z <= 0; end			      //  y x1 x2 = 000       yn+1 = 0       z=0
	                1: begin ny <= 0; z <= 1; end			      //  y x1 x2 = 001       yn+1 = 0       z=1	                
		2: begin ny <= 0; z <= 1; end			      //  y x1 x2 = 010       yn+1 = 0       z=1	                
		3: begin ny <= 1; z <= 0; end			      //  y x1 x2 = 011       yn+1 = 1       z=0	                
		4: begin ny <= 0; z <= 1; end			      //  y x1 x2 = 100       yn+1 = 0       z=1	                
		5: begin ny <= 1; z <= 0; end			      //  y x1 x2 = 101       yn+1 = 1       z=0	                
		6: begin ny <= 1; z <= 0; end			      //  y x1 x2 = 110       yn+1 = 1       z=0	                
		7: begin ny <= 1; z <= 1; end			      //  y x1 x2 = 111       yn+1 = 1       z=1
                endcase
    end

endmodule