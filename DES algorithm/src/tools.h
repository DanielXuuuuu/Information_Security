#pragma once

static const char *ALPHA_BASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
 * 将字符串转为Base64编码
 */
void char2base64(const char *buf, const long size, char *base64Char) {
	int a = 0;
	int i = 0;
	while (i < size) {
		char b0 = buf[i++];
		char b1 = (i < size) ? buf[i++] : 0;
		char b2 = (i < size) ? buf[i++] : 0;

		int int63 = 0x3F;  // 00111111
		int int255 = 0xFF; // 11111111
		base64Char[a++] = ALPHA_BASE[(b0 >> 2) & int63];
		base64Char[a++] = ALPHA_BASE[((b0 << 4) | ((b1 & int255) >> 4)) & int63];
		base64Char[a++] = ALPHA_BASE[((b1 << 2) | ((b2 & int255) >> 6)) & int63];
		base64Char[a++] = ALPHA_BASE[b2 & int63];
	}
	int temp = a;
	switch (size % 3) {
	case 1:
		base64Char[--temp] = '=';
	case 2:
		base64Char[--temp] = '=';
	}
	base64Char[a] = '\0';
}

/*
 * 将Base64编码转为字符串形式
 */
void base642char(const char *base64Char, const long base64CharSize, char *originChar, long originCharSize) {
	int toInt[128] = { -1 };
	for (int i = 0; i < 64; i++) {
		toInt[ALPHA_BASE[i]] = i;
	}
	int int255 = 0xFF;
	int index = 0;
	for (int i = 0; i < base64CharSize; i += 4) {
		int c0 = toInt[base64Char[i]];
		int c1 = toInt[base64Char[i + 1]];
		originChar[index++] = (((c0 << 2) | (c1 >> 4)) & int255);
		if (index >= originCharSize) {
			return;
		}
		int c2 = toInt[base64Char[i + 2]];
		originChar[index++] = (((c1 << 4) | (c2 >> 2)) & int255);
		if (index >= originCharSize) {
			return;
		}
		int c3 = toInt[base64Char[i + 3]];
		originChar[index++] = (((c2 << 6) | c3) & int255);
	}
}

/*
 * 将char数组转为二进制，每个字符8位
 */
void char2Bits(const char* buf, int size, int* bits)
{
	for (int i = 0; i < size; i++)
		for (int j = 0; j < 8; j++)
			bits[i * 8 + 7 - j] = ((buf[i] >> j) & 1);
}

/*
 * 将二进制转换为char数组
 */
void bits2char(const int* bits, int size, char* buf)
{
	int sum = 0, count = 0;
	for (int i = 0; i < size; i++) {
		sum = sum * 2 + bits[i];
		if (i % 8 == 7) {
			buf[count++] = sum;
			sum = 0;
		}
	}
}

/*
 * 数组拷贝
 */
void copyArray(int* dest, int* src, int size)
{
	for (int i = 0; i < size; i++)
		dest[i] = src[i];
}