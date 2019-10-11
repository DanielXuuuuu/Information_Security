#include "des.h"

void menu();
void encrypt();
void decrypt();

/*
 * 主函数
 */
int main() {
	int choice;
	while (1) {
		menu();
		scanf("%d", &choice);
		while (choice < 0 || choice > 2) {
			printf("[ERROR] Wrong choice! Try again: ");
			scanf("%d", &choice);
		}
		getchar();
		if (choice == 1) {
			encrypt();
		}
		else if (choice == 2) {
			decrypt();
		}
		else if (choice == 0) {
			printf("Bye!\n");
			break;
		}
	}
	system("pause");
	return 0;
}

/*
 * 菜单
 */
void menu() {
	printf("================== DES Cryptographic Algorithms ==================\n\n");
	printf("                       1. Encrypt\n");
	printf("                       2. Decrypt\n");
	printf("                       0. Quit\n\n");
	printf("==================================================================\n");
	printf("[INPUT] Enter your choice: ");
}

/*
 * 加密
 */
void encrypt()
{
	char plain[9];
	char key[9];
	char cipher[9];

	printf("[INPUT] Enter plaintext to be encrypted: ");
	gets_s(plain, 9);

	printf("[INPUT] Enter the key: ");
	gets_s(key, 9);

	DES_encrypt(plain, key, cipher);

	printf("\n\n\n");
}

/*
 * 解密
 */
void decrypt() 
{
	char plain[9];
	char key[9];
	char base64Char[100];

	printf("[INPUT] Enter ciphertext (Base64 form) to be decrypted: ");
	scanf("%s", base64Char);
	getchar();

	printf("[INPUT] Enter the key: ");
	gets_s(key, 9);

	DES_decrypt(base64Char, key, plain);

	printf("\n\n\n");
}