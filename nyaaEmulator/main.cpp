#include "macros.h"
#include <iostream>


unsigned char memory[4096]; // the computer's memory
unsigned char chardisplay[16][2]; //16-wide character display with two lines


//registers
unsigned char t; //temp / swap
unsigned char p; //pointer (for memory operations)
unsigned char a; //accumulator (only updated after ALU function)
unsigned char i1; //in 1 of ALU
unsigned char i2; //in 2 of ALU

unsigned char b; //memory bank
unsigned char in1; //hex keypad input 0-7
unsigned char in2; //hex keypad input 8-F


unsigned short pc; //program counter



void redrawScreen() {
	system("CLS"); //evil command and a big security hole, but we ain't no code wizzards
	std::cout << "text output:" << std::endl;
	std::cout << "##################" << std::endl;
	for (int a = 0; a < 2; a++) {
		std::cout << '#';
		for (int i = 0; i < 16; i++) {
			std::cout << chardisplay[i][a];
		}
		std::cout << '#' << std::endl;
	}
	std::cout << "##################" << std::endl;
}

void main() {
	std::cout << "lolis" << std::endl;
	redrawScreen();
}