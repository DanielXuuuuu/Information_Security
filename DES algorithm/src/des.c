#pragma once
#include "des.h"
#include "tables.h"
#include "tools.h"

/*
 * 加密函数
 */
void DES_encrypt(char* plain, char* key, char* cipher)
{
	int plainBits[64] = { 0 };	//明文的64位二进制形式
	int middleBits[64] = { 0 }; //用于保存中间结果
	int cipherBits[64] = { 0 };	//密文的64位二进制形式

	int keyBit[64] = { 0 };		//64位密钥
	int subKeys[16][48] = { 0 };//16个48位子密钥

	int left[32] = { 0 };		
	int right[32] = { 0 };

	char2Bits(key, 8, keyBit);
	char2Bits(plain, 8, plainBits);
	
	genereteSubKeys(keyBit, subKeys);

	copyArray(middleBits, plainBits, 64);

	IP_Transform(middleBits);

	split(middleBits, left, right);

	T_Iterations(left, right, subKeys, 1);

	merge(middleBits, left, right);

	IP_1_Transform(middleBits);

	copyArray(cipherBits, middleBits, 64);

	//打印结果
	printf("[INFO] Encryption finished!\n");
	printf("[RESULT]\n");
	printf("\tBinary form: ");
	for (int i = 0; i < 64; i++) {
		printf("%d", cipherBits[i]);
	}

	printf("\n\tASCII form: ");
	bits2char(cipherBits, 64, cipher);
	cipher[8] = '\0';
	printf("%s", cipher);

	//base64编码形式
	printf("\n\tBase64 form: ");
	char base64Char[100];
	char2base64(cipher, 8, base64Char);
	printf("%s", base64Char);
}

/*
 * 解密函数
 */
void DES_decrypt(char* base64Char, char* key, char* plain)
{
	//将Base64转换成char
	char cipher[9];
	base642char(base64Char, strlen(base64Char), cipher, 8);

	int plainBits[64];	//明文的64位二进制形式
	int middleBits[64]; //用于保存中间结果
	int cipherBits[64];	//密文的64位二进制形式

	int keyBit[64] = { 0 };
	int subKeys[16][48] = { 0 };

	int left[32] = { 0 };
	int right[32] = { 0 };

	char2Bits(key, 8, keyBit);
	char2Bits(cipher, 8, cipherBits);

	genereteSubKeys(keyBit, subKeys);

	copyArray(middleBits, cipherBits, 64);

	IP_Transform(middleBits);

	split(middleBits, left, right);

	T_Iterations(left, right, subKeys, 0);

	merge(middleBits, left, right);

	IP_1_Transform(middleBits);

	copyArray(plainBits, middleBits, 64);

	bits2char(plainBits, 64, plain);
	
	//打印结果
	printf("[INFO] Decryption finished!\n");
	printf("[RESULT]\n");
	printf("\tASCII form: ");
	plain[8] = '\0';
	printf("%s", plain);
}

/*
 * 初始IP置换
 */
void IP_Transform(int* src)
{
	int temp[64];
	for (int i = 0; i < 64; i++)
		temp[i] = src[i];
	for (int i = 0; i < 64; i++)
		src[i] = temp[IP[i] - 1];
}

/*
 * 迭代T
 */
void T_Iterations(int* left, int* right, int subKeys[][48], int isEncrypt)
{
	int leftTemp[32], rightTemp[32];
	int feistelResult[32];
	
	//共十六次迭代
	for (int i = 0; i < 16; i++) {
		if (isEncrypt)
			feistel(right, subKeys[i], feistelResult);
		else
			feistel(right, subKeys[15 - i], feistelResult);
		for (int j = 0; j < 32; j++) {
			leftTemp[j] = right[j];
			rightTemp[j] = left[j] ^ feistelResult[j];
		}
		for (int j = 0; j < 32; j++) {
			left[j] = leftTemp[j];
			right[j] = rightTemp[j];
		}
	}
}

/*
 * IP-1逆置换
 */
void IP_1_Transform(int* src)
{
	int temp[64];
	for (int i = 0; i < 64; i++)
		temp[i] = src[i];
	for (int i = 0; i < 64; i++)
		src[i] = temp[IP_1[i] - 1];
}

/*
 * Feistel函数
 */
void feistel(int* R, int* K, int* res)
{
	int E[48];
	int S[32];
	
	// E-扩展成48位
	for (int i = 0; i < 48; i++)
		E[i] = R[E_Expend[i] - 1];

	// 与48位子密钥作按位异或运算
	for (int i = 0; i < 48; i++)
		E[i] = E[i] ^ K[i];

	// 进行S盒变换成32位
	for (int i = 0; i < 8; i++) {
		//得到相应S盒对应位置的十进制数
		int row = E[i * 6] * 2 + E[i * 6 + 5];
		int col = E[i * 6 + 1] * 8 + E[i * 6 + 2] * 4 + E[i * 6 + 3] * 2 + E[i * 6 + 4];
		int S_BoxValue = S_Box[i][row][col];

		//将十进制数转换成对应4位二进制
		for (int j = 3; j >= 0; j--) {
			S[i * 4 + j] = S_BoxValue % 2;
			S_BoxValue /= 2;
		}
	}

	// 进行P置换
	for (int i = 0; i < 32; i++)
		res[i] = S[P_Transform[i] - 1];
}

/*
 * 将64位分解为前后32位
 */
void split(int* src, int* left, int* right)
{
	for (int i = 0; i < 32; i++) {
		left[i] = src[i];
		right[i] = src[i + 32];
	}

}

/*
 * 最后的合并函数，注意要交换顺序
 */
void merge(int* res, int* left, int* right)
{
	for (int i = 0; i < 32; i++) {
		res[i] = right[i];
		res[i + 32] = left[i];
	}
}

/*
 * 子密钥生成
 */
void genereteSubKeys(int* key, int subKeys[][48])
{
	int realKey[56] = { 0 };
	int left[28] = { 0 };
	int right[28] = { 0 };
	int subKey[48] = { 0 };
	
	// PC-1置换
	for (int i = 0; i < 56; i++)
		realKey[i] = key[PC_1[i] - 1];

	// 循环产生16个子密钥
	for (int round = 0; round < 16; round++) {
		for (int i = 0; i < 28; i++)
			left[i] = realKey[i];
		for (int i = 28; i < 56; i++)
			right[i - 28] = realKey[i];

		// 对前、后28位进行循环左移
		leftShift(left, leftShiftBits[round]);
		leftShift(right, leftShiftBits[round]);
		for (int i = 0; i < 28; i++)
			realKey[i] = left[i];
		for (int i = 0; i < 28; i++)
			realKey[i + 28] = right[i];

		// PC-2压缩置换
		for (int i = 0; i < 48; i++)
			subKey[i] = realKey[PC_2[i] - 1];
		
		for(int i = 0; i < 48; i++)
			subKeys[round][i] = subKey[i];
	}
}

/*
 * 循环左移
 */
void leftShift(int* src, int shiftBits)
{
	int temp[28];
	for (int i = 0; i < 28; i++)
		temp[i] = src[i];

	for (int i = 0; i < 28; i++) {
		src[i] = temp[(i + shiftBits) % 28];
	}
}