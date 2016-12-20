#include <iostream>
int main() {
	unsigned char memory[4096];
	unsigned char r0;
	unsigned char r1;
	unsigned char r2;
	unsigned char r3;
	unsigned char r4;
	unsigned char r5;
	unsigned char r6;
	unsigned char r7;
	unsigned char flags[2];
do {	memory[0] = 72;
	flags[0] |= 1 << 0;
	memory[1] = 101;
	flags[0] |= 1 << 1;
	memory[2] = 108;
	flags[0] |= 1 << 2;
	memory[3] = 108;
	flags[0] |= 1 << 3;
	memory[4] = 111;
	flags[0] |= 1 << 4;
	memory[5] = 44;
	flags[0] |= 1 << 5;
	memory[6] = 32;
	flags[0] |= 1 << 6;
	memory[7] = 87;
	flags[0] |= 1 << 7;
	memory[8] = 111;
	flags[1] |= 1 << 0;
	memory[9] = 114;
	flags[1] |= 1 << 1;
	memory[10] = 108;
	flags[1] |= 1 << 2;
	memory[11] = 100;
	flags[1] |= 1 << 3;
	memory[12] = 33;
	flags[1] |= 1 << 4;
	memory[13] = 10;
	flags[1] |= 1 << 5;
	for (int i = 0; i < 14; ++i) {std::cout.put(memory[0 + i]);}
	flags[1] |= 1 << 6;
}while (flags[1] & 1 << 7);
}