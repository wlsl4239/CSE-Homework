/* Computer Security Assignment - Modified AES */
/* 12131469 김경수 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

using namespace std;

int key[4][4];			//키
int plain[4][4];		//평문

int expandedkey[4][44];		//확장 키

int RC[10] = { 0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x69,0xD2 };		//R function 상수

int sbox[16][16] =			//s-box
{
	{ 0x15,0x0A,0xF4,0x54,0xE5,0x2A,0xB5,0x38,0x6D,0x0E,0x8A,0x05,0x45,0xBA,0x83,0xFF },
	{ 0x47,0xF8,0x98,0xF7,0xDA,0xE8,0x1D,0x8F,0x3D,0xB3,0xAC,0x1C,0x5E,0x51,0x60,0x4E },
	{ 0x3C,0x97,0x8D,0x3B,0xD3,0x1A,0x64,0xD6,0xF2,0x72,0x85,0x77,0x11,0xED,0x36,0x6C },
	{ 0x6F,0x9B,0x28,0xB1,0xC9,0xB4,0x91,0xA5,0xB0,0x9C,0x59,0x2E,0xAF,0xE0,0xB8,0xF5 },
	{ 0xEF,0x01,0x3A,0xA1,0x37,0x26,0x02,0xB9,0x76,0xFA,0xFC,0xD9,0xC3,0xB7,0x9A,0x2F },
	{ 0x88,0xF6,0xA6,0x90,0x33,0xC2,0x4A,0x93,0x79,0xCE,0x69,0x35,0xEA,0xBD,0xC7,0x3E },
	{ 0x46,0xDE,0x52,0x94,0x8B,0x0F,0x29,0x53,0x7B,0xAB,0xC5,0x87,0x57,0x27,0x4D,0x10 },
	{ 0xA9,0x89,0xBF,0x17,0x5D,0x67,0xE6,0x09,0x48,0xAA,0x81,0x7C,0xAD,0x80,0x65,0x7A },
	{ 0x68,0x30,0x71,0x1F,0x82,0x16,0x21,0x99,0x6A,0x1B,0xE2,0xCB,0xF0,0xCD,0x2D,0xF1 },
	{ 0xCA,0x50,0x8C,0x61,0xE1,0xA0,0x73,0x4C,0x7E,0x55,0x44,0x9F,0xD2,0x1E,0x66,0xC4 },
	{ 0xDB,0xD1,0xE4,0xE7,0xCC,0x95,0xD7,0x92,0x06,0x63,0xFE,0x08,0xD4,0x5C,0x56,0xA8 },
	{ 0x23,0xE9,0x96,0xC0,0x2B,0x4B,0x6B,0x4F,0x84,0x07,0x41,0x7F,0x12,0xD0,0xEE,0x20 },
	{ 0xBC,0x78,0x9E,0x19,0xD8,0x86,0xBB,0x6E,0x5A,0x8E,0x18,0x70,0x0B,0xE3,0x58,0xFB },
	{ 0x22,0x03,0x24,0xC8,0x13,0x0D,0x32,0x3F,0x34,0xEB,0x62,0x04,0x39,0xAE,0xF9,0x9D },
	{ 0x25,0xF3,0x5B,0xB2,0x40,0x2C,0x14,0xC6,0x31,0xC1,0x42,0xA3,0xEC,0xBE,0x75,0x7D },
	{ 0xD5,0x74,0xA4,0x00,0x5F,0xDD,0xCF,0x0C,0x49,0xDC,0xDF,0xB6,0x43,0xFD,0xA2,0xA7 }
};

int rsbox[16][16];			//reversed s-box
void sbox_inverse()			 //s-box를 통해 rsbox를 구하는 함수
{
	for (int i = 0;i < 16;i++)
		for (int j = 0;j < 16;j++)
			rsbox[sbox[i][j] / 16][sbox[i][j] % 16] = i * 16 + j;
} 

void substitutebytes()		//s박스를 참조하여 값을 변경
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			plain[i][j] = sbox[plain[i][j] / 16][plain[i][j] % 16];
}

void inv_substitutebytes()		//rsbox를 참조하여 값을 변경
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			plain[i][j] = rsbox[plain[i][j] / 16][plain[i][j] % 16];

}

void shiftrow()						//행에 따라 값을 왼쪽으로 shift하는 함수.
{
	int temp = plain[1][0];			//2번째 줄은 왼쪽으로 1칸
	plain[1][0] = plain[1][1];
	plain[1][1] = plain[1][2];
	plain[1][2] = plain[1][3];
	plain[1][3] = temp;

	temp = plain[2][0];				//3번째 줄은 왼쪽으로 2칸
	plain[2][0] = plain[2][2];
	plain[2][2] = temp;
	temp = plain[2][1];
	plain[2][1] = plain[2][3];
	plain[2][3] = temp;

	temp = plain[3][3];				//4번째 줄은 왼쪽으로 3칸
	plain[3][3] = plain[3][2];
	plain[3][2] = plain[3][1];
	plain[3][1] = plain[3][0];
	plain[3][0] = temp;
}

void inv_shiftrow()					//행에 따라 값을 왼쪽으로 shift하는 함수.
{
	int temp = plain[1][3];			//2번째 줄은 오른쪽으로 1칸
	plain[1][3] = plain[1][2];
	plain[1][2] = plain[1][1];
	plain[1][1] = plain[1][0];
	plain[1][0] = temp;

	temp = plain[2][0];				//3번째 줄은 오른쪽으로 2칸
	plain[2][0] = plain[2][2];
	plain[2][2] = temp;
	temp = plain[2][1];
	plain[2][1] = plain[2][3];
	plain[2][3] = temp;

	temp = plain[3][0];				//4번째 줄은 오른쪽으로 3칸
	plain[3][0] = plain[3][1];
	plain[3][1] = plain[3][2];
	plain[3][2] = plain[3][3];
	plain[3][3] = temp;

}

void addroundkey(int round)			//확장 키와 현재 평문을 XOR연산하는 함수.
{
	int roundkey[4][4];
	for (int i = 0;i<4;i++)
		for (int j = 0;j < 4;j++)
			roundkey[j][i] = expandedkey[j][round * 4 + i];

	for (int i = 0;i<4;i++)
		for (int j = 0;j < 4;j++)
			plain[j][i] = roundkey[j][i] ^ plain[j][i];		//평문과 확장키를 XOR한다.
}

int GF(int a, int b) {		//두 수의 GF곱을 구하는 함수
	int p = 0;
	int counter;
	int hi_bit_set;
	for (counter = 0; counter < 8; counter++) {
		if (b & 1)
			p ^= a;
		hi_bit_set = (a & 0x80);		//carry가 발생하는지 체크
		a <<= 1;
		if (hi_bit_set)
			a ^= 0x169;		//Mod(x^8 + x^6 + x^5 + x^3 + 1)
		b >>= 1;
	}
	return p;
}

void mixcolumn()			//행렬 곱을 통해 값을 섞는 함수
{
	int newplain[4][4];

	for (int i = 0; i < 4; i++) {
		newplain[0][i] = GF(0x02, plain[0][i]) ^ GF(0x03, plain[1][i]) ^ GF(0x01, plain[2][i]) ^ GF(0x01, plain[3][i]);
		newplain[1][i] = GF(0x01, plain[0][i]) ^ GF(0x02, plain[1][i]) ^ GF(0x03, plain[2][i]) ^ GF(0x01, plain[3][i]);
		newplain[2][i] = GF(0x01, plain[0][i]) ^ GF(0x01, plain[1][i]) ^ GF(0x02, plain[2][i]) ^ GF(0x03, plain[3][i]);
		newplain[3][i] = GF(0x03, plain[0][i]) ^ GF(0x01, plain[1][i]) ^ GF(0x01, plain[2][i]) ^ GF(0x02, plain[3][i]);
	}						//GF의 행렬곱이므로 곱은 GF의곱으로, 합은 XOR연산을 통해 구한다.

	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			plain[i][j] = newplain[i][j];
}

void inv_mixcolumn()			//행렬 곱을 통해 값을 섞는 함수
{
	int newplain[4][4];

	for (int i = 0; i < 4; i++) {
		newplain[0][i] = GF(0x0e, plain[0][i]) ^ GF(0x0b, plain[1][i]) ^ GF(0x0d, plain[2][i]) ^ GF(0x09, plain[3][i]);
		newplain[1][i] = GF(0x09, plain[0][i]) ^ GF(0x0e, plain[1][i]) ^ GF(0x0b, plain[2][i]) ^ GF(0x0d, plain[3][i]);
		newplain[2][i] = GF(0x0d, plain[0][i]) ^ GF(0x09, plain[1][i]) ^ GF(0x0e, plain[2][i]) ^ GF(0x0b, plain[3][i]);
		newplain[3][i] = GF(0x0b, plain[0][i]) ^ GF(0x0d, plain[1][i]) ^ GF(0x09, plain[2][i]) ^ GF(0x0e, plain[3][i]);
	}						//GF의 행렬곱이므로 곱은 GF의곱으로, 합은 XOR연산을 통해 구한다.

	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			plain[i][j] = newplain[i][j];
}

void keyexpansion()						//키 확장 함수
{
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			expandedkey[i][j] = key[i][j];		//w0~w3은 키와 같으므로 복사해온다.


	for (int i = 1;i <= 10;i++) {

		/* R function */
		int tempkey[4] = {
			expandedkey[1][i * 4 - 1],				//R function에서 맨 처음의 swap
			expandedkey[2][i * 4 - 1],
			expandedkey[3][i * 4 - 1],
			expandedkey[0][i * 4 - 1] };


		tempkey[0] = sbox[(tempkey[0] / 16)][(tempkey[0] % 16)];	//s박스에서 값을 찾아 입력한다.
		tempkey[0] = tempkey[0] ^ RC[i-1];						//RC[round]와 XOR연산을 한다.
		tempkey[1] = sbox[(tempkey[1] / 16)][(tempkey[1] % 16)];	//s박스에서 값을 찾아 입력한다.
		tempkey[2] = sbox[(tempkey[2] / 16)][(tempkey[2] % 16)];	//s박스에서 값을 찾아 입력한다.
		tempkey[3] = sbox[(tempkey[3] / 16)][(tempkey[3] % 16)];	//s박스에서 값을 찾아 입력한다.




		/* key expansion */
		for (int j = 0;j < 4;j++)
			expandedkey[j][i * 4] = tempkey[j] ^ expandedkey[j][(i - 1) * 4];
					//이전 round의 마지막 column을 R function에 넣은 결과와 이전 round의 첫 column을 XOR연산한다.
		for (int k = 1;k <= 3;k++)
		{
			for (int j = 0;j<4;j++)
				expandedkey[j][i * 4 + k] = expandedkey[j][i * 4 + k - 1] ^ expandedkey[j][(i - 1) * 4 + k];
		}		
					//현재 round의 직전 column과 이전 round의 해당 순서의 column을 XOR연산한다.
	}
}

void printEncryption()		//암호화 결과 출력함수
{

	printf("\n\n<------ ENCRYPTION ------>\n\nKEY EXPANSION\n");


	for (int i = 0;i < 11;i++)
	{
		printf("ROUND %d:", i);
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", expandedkey[k][i * 4 + j]);		//확장키 출력
		printf("\n");

	}


	addroundkey(0);						//round 0 : add round key
	printf("\nRound 0\nAR:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);		//round 0 결과
	printf("\n\n");



	for (int i = 1;i <= 9;i++)
	{
		printf("Round %d\n", i);		//round 1~9 : substitute Byte -> Shift Row -> Mix Column -> add round key

		substitutebytes();
		printf("SB:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n");

		shiftrow();
		printf("SR:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n");

		mixcolumn();
		printf("MC:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n");

		addroundkey(i);
		printf("AR:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n\n");


	}
	printf("Round %d\n", 10);		//round 10 :  substitute Byte -> Shift Row -> add round key

	substitutebytes();
	printf("SB:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);
	printf("\n");

	shiftrow();
	printf("SR:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);
	printf("\n");


	addroundkey(10);
	printf("AR:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);
	printf("\n\n");


	printf("\n\nCIPHER: ");
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			printf("%02X ", plain[j][i]);			//암호화 결과 출력
	printf("\n\n\n");

}

void printDecryption()		//복호화 결과 출력함수
{

	printf("<------ DECRYPTION ------>\n\n");

	addroundkey(10);				//key는 역순으로 사용한다.
	printf("Round 0\nAR:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);		//round 0 결과
	printf("\n\n");



	for (int i = 9;i >= 1;i--)
	{
		printf("Round %d\n", 10-i);
						//round 1~9 : inversed Shift Row -> inversed substitute Byte 
									// -> inversed add round key -> inversed Mix Column 
		inv_shiftrow();
		printf("SR:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n");

		inv_substitutebytes();
		printf("SB:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n");

		addroundkey(i);
		printf("AR:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n");

		inv_mixcolumn();
		printf("MC:");
		for (int j = 0;j<4;j++)
			for (int k = 0;k < 4;k++)
				printf(" %02X", plain[k][j]);
		printf("\n\n");



	}
	printf("Round %d\n", 10);
					//round 10 : inversed Shift Row -> inversed substitute Byte -> inversed add round key
	inv_substitutebytes();
	printf("SB:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);
	printf("\n");

	inv_shiftrow();
	printf("SR:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);
	printf("\n");


	addroundkey(0);
	printf("AR:");
	for (int j = 0;j<4;j++)
		for (int k = 0;k < 4;k++)
			printf(" %02X", plain[k][j]);
	printf("\n\n");


	printf("DECRYPTED:");
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			printf(" %02X", plain[j][i]);		//복호화 결과 출력
	printf("\n\n\n");
}


int main()
{
	/* 파일 입출력 */
	ifstream kkey;
	ifstream pplain;
	ofstream ccypher;
	ofstream ddecrypt;


	kkey.open("key.bin", ios::binary);
	pplain.open("plain.bin", ios::binary);
	ccypher.open("cypher.bin", ios::binary);
	ddecrypt.open("decrypt.bin", ios::binary);

	/* read key, plaintext 00~70*/
	for (int i = 0;i < 2;i++) {
		for (int j = 0;j < 4;j++)
		{
			char buf[1];
			kkey.read(buf, 1);
			key[j][i] = (int)buf[0];
			pplain.read(buf, 1);
			plain[j][i] = (int)buf[0];
		}
	}

	/* read key, plaintext 80~F0*/
	for (int i = 2;i < 4;i++) {
		for (int j = 0;j < 4;j++)
		{
			char buf[1];
			kkey.read(buf, 1);
			key[j][i] = (int)buf[0] ^ 0xffffff00;
			pplain.read(buf, 1);
			plain[j][i] = (int)buf[0] ^ 0xffffff00;	
			//값이 0xFFFFFF80~0xFFFFFFF0이라 0x00000080~0x000000F0으로 바꾸기 위함
		}
	}

	sbox_inverse();		//rsbox 생성
	keyexpansion();		//확장키 생성


	printf("RC: ");					//RC 출력
	for (int i = 0;i < 10;i++)
		printf("%02X ", RC[i]);

	printf("\nPLAIN: ");			//평문 출력
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			printf("%02X ", plain[j][i]);
	printf("\nKEY: ");				//키 출력
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			printf("%02X ", key[j][i]);


	printEncryption();				// 암호화 결과 출력
	
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			ccypher<<(char)plain[j][i];		//암호화 결과를 파일에 저장


	printDecryption();				// 복호화 결과 출력

	
	for (int i = 0;i < 4;i++)
		for (int j = 0;j < 4;j++)
			ddecrypt<<(char)plain[j][i];	//복호화 결과를 파일에 저장

	
	return 0;
}