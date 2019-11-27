#include <stdio.h>
#include <string.h>
#include <intrin.h>
#include "check.h"
int cmp[] = { 0x3a19acb0,0x048e3970,0x3a2716b6,0xad7f3b4d,
			0xa64eb06c,0xb1dd4426,0x8af76743,0xbb8e7bf6,
			0xf8200f50,0xa073879d,0x3b82ac26,0x188d5573,
			0xbbafd2f5,0x15db6925,0x348f4054,0xa5f17eca };
void check_len(char *flag) {
	if (strlen(flag) != 64) {
		exit(-1);
	}
}
void check_flag(__m256i R0, __m256i R1, __m256i R2, __m256i R3) {
	int* val[4];
	val[0] = (int*)& R3;
	val[1] = (int*)& R2;
	val[2] = (int*)& R1;
	val[3] = (int*)& R0;
	int cnt = 0;
	/*
	printf("0x%08x,0x%08x,0x%08x,0x%08x\n", val[0][0], val[0][1], val[0][2], val[0][3]);
	printf("0x%08x,0x%08x,0x%08x,0x%08x\n", val[1][0], val[1][1], val[1][2], val[1][3]);
	printf("0x%08x,0x%08x,0x%08x,0x%08x\n", val[2][0], val[2][1], val[2][2], val[2][3]);
	printf("0x%08x,0x%08x,0x%08x,0x%08x\n", val[3][0], val[3][1], val[3][2], val[3][3]);
	*/
	for (int i = 0; i < 4; i++) {
		if (cmp[cnt] == val[i][0] && cmp[cnt + 1] == val[i][1] && cmp[cnt + 2] == val[i][2] && cmp[cnt + 3] == val[i][3]) {
			cnt += 4;
		}
		else {
			printf("wrong.\n");
			exit(-1);
		}
	}
	printf("right\n");
}