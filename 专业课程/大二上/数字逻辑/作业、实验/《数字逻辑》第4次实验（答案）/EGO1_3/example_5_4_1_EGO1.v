//行为描述方式实现例题5.4的3位串行输入移位寄存器，输入x和CP为开发板上最左边的拨动开关以及S1按键，状态（y3、y2、y1）和输出Z为开发板上最左边的3个LED灯以及最右边的LED灯。

`timescale 1ns / 1ps	

module example_5_4_1(
    input cp, x,
    input y3, y2, y1,				 	
    output reg ny3, ny2, ny1, z    		 	          
);

    always @(negedge cp)                      //行为描述方式
    begin
                case({y3, y2, y1, x})
	                4'b0000: begin ny3 <= 0; ny2 <= 0; ny1 <= 0; z <= 0; end			      //  y3 y2 y1 x = 0000       y3n+1 y2n+1 y1n+1 = 000    z=0
	                4'b0001: begin ny3 <= 0; ny2 <= 0; ny1 <= 1; z <= 0; end			      //  y3 y2 y1 x = 0001       y3n+1 y2n+1 y1n+1 = 001    z=0
	                4'b0010: begin ny3 <= 0; ny2 <= 1; ny1 <= 0; z <= 1; end			      //  y3 y2 y1 x = 0010       y3n+1 y2n+1 y1n+1 = 010    z=1
	                4'b0011: begin ny3 <= 0; ny2 <= 1; ny1 <= 1; z <= 1; end			      //  y3 y2 y1 x = 0011       y3n+1 y2n+1 y1n+1 = 011    z=1
	                4'b0100: begin ny3 <= 1; ny2 <= 0; ny1 <= 0; z <= 1; end			      //  y3 y2 y1 x = 0100       y3n+1 y2n+1 y1n+1 = 100    z=1
	                4'b0101: begin ny3 <= 1; ny2 <= 0; ny1 <= 1; z <= 1; end			      //  y3 y2 y1 x = 0101       y3n+1 y2n+1 y1n+1 = 101    z=1
	                4'b0110: begin ny3 <= 1; ny2 <= 1; ny1 <= 0; z <= 0; end			      //  y3 y2 y1 x = 0110       y3n+1 y2n+1 y1n+1 = 110    z=0
	                4'b0111: begin ny3 <= 1; ny2 <= 1; ny1 <= 1; z <= 0; end			      //  y3 y2 y1 x = 0111       y3n+1 y2n+1 y1n+1 = 111    z=0
	                4'b1000: begin ny3 <= 0; ny2 <= 0; ny1 <= 0; z <= 1; end			      //  y3 y2 y1 x = 1000       y3n+1 y2n+1 y1n+1 = 000    z=1
	                4'b1001: begin ny3 <= 0; ny2 <= 0; ny1 <= 1; z <= 1; end			      //  y3 y2 y1 x = 1001       y3n+1 y2n+1 y1n+1 = 001    z=1
	                4'b1010: begin ny3 <= 0; ny2 <= 1; ny1 <= 0; z <= 0; end			      //  y3 y2 y1 x = 1010       y3n+1 y2n+1 y1n+1 = 010    z=0
	                4'b1011: begin ny3 <= 0; ny2 <= 1; ny1 <= 1; z <= 0; end			      //  y3 y2 y1 x = 1011       y3n+1 y2n+1 y1n+1 = 011    z=0
	                4'b1100: begin ny3 <= 1; ny2 <= 0; ny1 <= 0; z <= 0; end			      //  y3 y2 y1 x = 1100       y3n+1 y2n+1 y1n+1 = 100    z=0
	                4'b1101: begin ny3 <= 1; ny2 <= 0; ny1 <= 1; z <= 0; end			      //  y3 y2 y1 x = 1101       y3n+1 y2n+1 y1n+1 = 101    z=0
	                4'b1110: begin ny3 <= 1; ny2 <= 1; ny1 <= 0; z <= 1; end			      //  y3 y2 y1 x = 1110       y3n+1 y2n+1 y1n+1 = 110    z=1
	                4'b1111: begin ny3 <= 1; ny2 <= 1; ny1 <= 1; z <= 1; end			      //  y3 y2 y1 x = 1111       y3n+1 y2n+1 y1n+1 = 111    z=1
                endcase
    end

endmodule

module example_5_4_1_exe(
    input sw_pin[7:0],                               		 //8个拨动开关
    input btn_1,				 //1个按钮
    output [15:0] led_pin    		 	//16个led灯            
);

    reg y3, y2, y1;

    example_5_4_1 U(.cp(btn_1), .x(sw_pin[0]), .y3(y3), .y2(y2), .y1(y1), .ny3(led_pin[0]), .ny2(led_pin[1]), .ny1(led_pin[2]), .z(led_pin[7]));

    initial begin
        y3=0;
        y2=0;
        y1=0;
    end

    always @(*)                     //行为描述方式
    begin
	y3 <= led_pin[0];
	y2 <= led_pin[1];
	y1 <= led_pin[2];
    end

endmodule