#include "macros.h"
#include <iostream>
#include <Windows.h>
#include <bitset>
#include <fstream>
#include <vector>

const char* filename = "test.bin";

unsigned char memory[4096]; // the computer's memory
unsigned char chardisplay[16][4]; //16-wide character display with two lines
unsigned short opcode;

bool singlestepbool = false;
bool singlestep = true;

//registers
enum reg{
	t=0,
	p=1,
	a=2,
	i1=3,
	i2=4,

	b=0xd,
	in1=0xe,
	in2=0xf
};

unsigned char r[16];
/*unsigned char t; //temp / swap
unsigned char p; //pointer (for memory operations)
unsigned char a; //accumulator (only updated after ALU function)
unsigned char i1; //in 1 of ALU
unsigned char i2; //in 2 of ALU*/

/*unsigned char b; //memory bank
unsigned char in1; //hex keypad input 0-7
unsigned char in2; //hex keypad input 8-F*/


unsigned short pc; //program counter

void emulateCycle();

void init();
void updateKeys();

void redrawScreen() {
	system("CLS"); //evil command and a big security hole, but we ain't no code wizzards
	std::cout << "text output:" << std::endl;
	std::cout << "##################" << std::endl;
	for (int a = 0; a < 4; a++) {
		std::cout << '#';
		for (int i = 0; i < 16; i++) {
			std::cout << chardisplay[i][a];
		}
		std::cout << '#' << std::endl;
	}
	std::cout << "##################" << std::endl;
	Debug("T=" << BIN(r[t]) << " P=" << BIN(r[p]) << " " << "A=" << BIN(r[a]) << " " << "i1=" << BIN(r[i1]) << " " << "i2=" << BIN(r[i2]) << " " << std::endl);
	Debug("PC=" << BIN(pc) << std::endl);
}

void main() {
	init();
	while (true){
		updateKeys();
		std::cin.ignore();
		redrawScreen();
		emulateCycle();
		
		

		//input testing:
		//std::bitset<8> loli(in1);
		//std::bitset<8> loli2(in2);
		//std::cout << loli << ' ' << loli2 << std::endl;
		
	}
}

void updateKeys() {
	if (GetKeyState('0') & 0x8000) {
		r[in1] = (r[in1] & 0xFE) | 0x01;
	}
	else {
		r[in1] = r[in1] & 0xFE;
	}

	if (GetKeyState('1') & 0x8000) {
		r[in1] = (r[in1] & 0xFD) | 0x02;
	}
	else {
		r[in1] = r[in1] & 0xFD;
	}

	if (GetKeyState('2') & 0x8000) {
		r[in1] = (r[in1] & 0xFB) | 0x04;
	}
	else {
		r[in1] = r[in1] & 0xFB;
	}

	if (GetKeyState('3') & 0x8000) {
		r[in1] = (r[in1] & 0xF7) | 0x08;
	}
	else {
		r[in1] = r[in1] & 0xF7;
	}


	if (GetKeyState('4') & 0x8000) {
		r[in1] = (r[in1] & 0xEF) | 0x10;
	}
	else {
		r[in1] = r[in1] & 0xEF;
	}

	if (GetKeyState('5') & 0x8000) {
		r[in1] = (r[in1] & 0xDF) | 0x20;
	}
	else {
		r[in1] = r[in1] & 0xDF;
	}

	if (GetKeyState('6') & 0x8000) {
		r[in1] = (r[in1] & 0xBF) | 0x40;
	}
	else {
		r[in1] = r[in1] & 0xBF;
	}

	if (GetKeyState('7') & 0x8000) {
		r[in1] = (r[in1] & 0x7F) | 0x80;
	}
	else {
		r[in1] = r[in1] & 0x7F;
	}




	if (GetKeyState('8') & 0x8000) {
		r[in2] = (r[in2] & 0xFE) | 0x01;
	}
	else {
		r[in2] = r[in2] & 0xFE;
	}

	if (GetKeyState('9') & 0x8000) {
		r[in2] = (r[in2] & 0xFD) | 0x02;
	}
	else {
		r[in2] = r[in2] & 0xFD;
	}

	if (GetKeyState('A') & 0x8000) {
		r[in2] = (r[in2] & 0xFB) | 0x04;
	}
	else {
		r[in2] = r[in2] & 0xFB;
	}

	if (GetKeyState('B') & 0x8000) {
		r[in2] = (r[in2] & 0xF7) | 0x08;
	}
	else {
		r[in2] = r[in2] & 0xF7;
	}


	if (GetKeyState('C') & 0x8000) {
		r[in2] = (r[in2] & 0xEF) | 0x10;
	}
	else {
		r[in2] = r[in2] & 0xEF;
	}

	if (GetKeyState('D') & 0x8000) {
		r[in2] = (r[in2] & 0xDF) | 0x20;
	}
	else {
		r[in2] = r[in2] & 0xDF;
	}

	if (GetKeyState('E') & 0x8000) {
		r[in2] = (r[in2] & 0xBF) | 0x40;
	}
	else {
		r[in2] = r[in2] & 0xBF;
	}

	if (GetKeyState('F') & 0x8000) {
		r[in2] = (r[in2] & 0x7F) | 0x80;
	}
	else {
		r[in2] = r[in2] & 0x7F;
	}


}

void emulateCycle() {

	//fetch opcode
	opcode = (memory[pc] << 8) | memory[pc + 1]; //merge two instruction bytes together
	Debug("opcode: " << HEX(opcode) << std::endl);

	switch (opcode & 0xF000) {

	case 0x0000: //multi-option
		switch (opcode & 0xFFF) {
		case 0x0000:
			Debug("no-op");
			pc += 2;
			break;
		case 0x0001:
			Debug("i1 AND i2");
			r[a] = (r[i1] & r[i2]);
			pc += 2;
			break;

		default:
			Debug("unrecognized opcode");
			break;
		}
		break;

	case 0x4000:
		switch (opcode & 0x0F00) {

		case 0x0000: //0x40XX
			Debug("load XX into temp");
			r[t] = opcode & 0x00FF;
			pc += 2;
			break;

		case 0x0300: //0x43XY
			Debug("load reg X into reg Y");
			r[opcode & 0x000F] = r[(opcode & 0x00F0) >> 4];
			pc += 2;
			break;

		default:
			Debug("unrecognized opcode");
			break;
		}


	}
}

void init() {
	r[t] = 0;
	r[p] = 0;
	r[a] = 0;
	r[i1] = 0;
	r[i2] = 0;

	r[b] = 0;
	r[in1] = 0;
	r[in2] = 0;


	//load asm file
	std::streampos filesize;
	std::ifstream programfile(filename, std::ios::binary);

	programfile.seekg(0, std::ios::end); //seek for end of file
	filesize = programfile.tellg(); //store length of file
	programfile.seekg(0, std::ios::beg); //go back to beginning

	std::vector<unsigned char> fileData(filesize);
	programfile.read((char*)&fileData[0], filesize);

	for (int i = 0; i < filesize; i++) {
		memory[i] = fileData[i];
	}

}