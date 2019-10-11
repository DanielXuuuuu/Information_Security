#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void DES_encrypt(char* plain, char* key, char* cipher);
void DES_decrypt(char* cipher, char* key, char* plain);

void genereteSubKeys(int* key, int subKeys[][48]);

void IP_Transform(int* src);
void split(int* src, int* left, int* right);
void T_Iterations(int* left, int* right, int subKeys[][48], int isEncrypt);
void merge(int* res, int* left, int* right);
void IP_1_Transform(int* src);

void feistel(int* R, int* K, int* res);
void leftShift(int* src, int shiftBits);