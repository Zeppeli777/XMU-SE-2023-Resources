//结构化描述方式（子模块采用行为描述方式）实现例题5.3的串行加法器，该电路的输入为x1、x2，状态为y，输出为Z，CP为时钟信号。

`timescale 1ns / 1ps	

module xor_gate(
    input a,
    input b,
    output f                               
);
    reg y;
    always @(*)                        //行为描述方式
    begin
        y <= a ^ b; 
    end
    assign f = y;
endmodule

module nand_gate(
    input a,
    input b,
    output f                               
);
    reg y;
    always @(*)                        //行为描述方式
    begin
        y <= ~(a & b); 
    end
    assign f = y;
endmodule

module d_flip_flop(
    input d,                               	  
    input cp,                                                         
    output q, qn		
);
    reg y;
    always @(negedge cp)                     //行为描述方式
    begin
                case(d)
	               0: y <= 0;			      //  D  =  01       Y=0
	               1: y <= 1;			      //  D  =  10       Y=1
                endcase
    end
    assign  q= y;
    assign qn = ~y;
endmodule

module example_5_3(
    input cp, x1, x2,                               		 
    input y,				
    output ny, z    		 	        
);

    wire t1, t2, t3, d, nyn;

    xor_gate U1(.a(x1),.b(x2),.f(t1));				//结构化描述方式
    nand_gate U2(.a(t1),.b(y),.f(t2));				//结构化描述方式
    nand_gate U3(.a(x1),.b(x2),.f(t3));				//结构化描述方式
    nand_gate U4(.a(t2),.b(t3),.f(d));				//结构化描述方式
    xor_gate U5(.a(t1),.b(y),.f(z));				//结构化描述方式
    d_flip_flop U6(.d(d),.cp(cp),.q(ny),.qn(nyn));			//结构化描述方式

endmodule