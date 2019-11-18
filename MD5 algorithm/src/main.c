#include "md5.h"

int main() {
	printf("\n================== MD5 Message-Digest Algorithm ==================\n\n");
	printf("[INPUT] Enter the filepath: ");
	
	// �����ļ�·��
	char filepath[256];
	scanf("%s", filepath);

	// 128λ���
	uint8_t result[16];
	int i;

	// ����ֵ������1��ʾ�д���
	if (MD5(filepath, result) == 1) {
		printf("\n[INFO] The result is: ");

		//���ֽ�ͨ���޷���ʮ���������
		for (i = 0; i < 16; i++) {
			printf("%2.2x", result[i]);
		}
		printf("\n");
	}

	system("pause");
	return 0;
}