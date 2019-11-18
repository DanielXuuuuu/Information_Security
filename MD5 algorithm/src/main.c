#include "md5.h"

int main() {
	printf("\n================== MD5 Message-Digest Algorithm ==================\n\n");
	printf("[INPUT] Enter the filepath: ");
	
	// 输入文件路径
	char filepath[256];
	scanf("%s", filepath);

	// 128位结果
	uint8_t result[16];
	int i;

	// 返回值不等于1表示有错误
	if (MD5(filepath, result) == 1) {
		printf("\n[INFO] The result is: ");

		//将字节通过无符号十六进制输出
		for (i = 0; i < 16; i++) {
			printf("%2.2x", result[i]);
		}
		printf("\n");
	}

	system("pause");
	return 0;
}