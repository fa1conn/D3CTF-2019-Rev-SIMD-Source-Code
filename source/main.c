#include <intrin.h>

#include <stdio.h>
#include "check.h"
#include "sm4.h"
extern unsigned long CK[32];
extern const unsigned long aes_s[256];
extern unsigned long FK[4];
extern const unsigned long md5_context[4];
extern unsigned char SboxTable[16][16];
sm4_context ctx;
unsigned char key[16] = { 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };

#ifdef __cplusplus
#define INITIALIZER(f) \
        static void f(void); \
        struct f##_t_ { f##_t_(void) { f(); } }; static f##_t_ f##_; \
        static void f(void)
#elif defined(_MSC_VER)
#pragma section(".CRT$XCU",read)
#define INITIALIZER2_(f,p) \
        static void f(void); \
        __declspec(allocate(".CRT$XCU")) void (*f##_)(void) = f; \
        __pragma(comment(linker,"/include:" p #f "_")) \
        static void f(void)
#ifdef _WIN64
#define INITIALIZER(f) INITIALIZER2_(f,"")
#else
#define INITIALIZER(f) INITIALIZER2_(f,"_")
#endif
#else
#define INITIALIZER(f) \
        static void f(void) __attribute__((constructor)); \
        static void f(void)
#endif

static void finalize(void)
{
	//printf("finalize\n");
}

INITIALIZER(initialize)
{
	change_ck();
	//unsigned char key[16] = { 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
	sm4_setkey_enc(&ctx, key);
	//printf("initialize\n");
	atexit(finalize);
}
//https://stackoverflow.com/questions/1113409/attribute-constructor-equivalent-in-vc


void read_flag(char* flag,__m256i* vindex,__m256i* R0, __m256i* R1, __m256i* R2, __m256i* R3) {
	*R0 = _mm256_i32gather_epi32((int*)(flag + 4 * 0), *vindex, 4);
	*R1 = _mm256_i32gather_epi32((int*)(flag + 4 * 1), *vindex, 4);
	*R2 = _mm256_i32gather_epi32((int*)(flag + 4 * 2), *vindex, 4);
	*R3 = _mm256_i32gather_epi32((int*)(flag + 4 * 3), *vindex, 4);
}
void reverse_flag(__m256i* mask1, __m256i* R0, __m256i* R1, __m256i* R2, __m256i* R3) {
	*R0 = _mm256_shuffle_epi8(*R0, *mask1);
	*R1 = _mm256_shuffle_epi8(*R1, *mask1);
	*R2 = _mm256_shuffle_epi8(*R2, *mask1);
	*R3 = _mm256_shuffle_epi8(*R3, *mask1);
}

int main()
{
	//flag:d^3ctf{tHis_is_avX2_sms4_You_can_fiNd_the_constant_And_solve_1t}
	char flag[100];
	printf("Welcome to D^3CTF,this is a SIMD challenge for you.wish you have a good time.\n");
	printf("Please enter your flag:");
	scanf_s("%s",flag,65);
	//printf("%s",flag);
	check_len(flag);
	
	__m256i id, R0, R1, R2, R3;
	__m256i vindex;
	//unsigned char key[16] = { 0x01,0x23,0x45,0x67,0x89,0xab,0xcd,0xef,0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10 };
	//sm4_setkey_enc(&ctx, key);

	vindex = _mm256_setr_epi32(0, 4, 8, 12, 0, 4, 8, 12);//改一下向量

	read_flag(flag, &vindex, &R0, &R1, &R2, &R3);
	__m256i mask1 = _mm256_setr_epi8(3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12, 3, 2, 1, 0, 7, 6, 5, 4, 11, 10, 9, 8, 15, 14, 13, 12);
	reverse_flag(&mask1, &R0, &R1, &R2, &R3);

	//sm4_encrypt(&ctx, R0, R1, R2, R3);
	sm4_encrypt_ecb(ctx.sk, &R0, &R1, &R2, &R3);

}