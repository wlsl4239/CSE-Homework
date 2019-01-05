/* 컴퓨터 보안 2차과제 - 30-bit triple RSA and Digital Signature */

#include <iostream>
#include <ctime>
#include "xxhash.c"

typedef unsigned long long ull;

using namespace std;

int n, phi;
int p, q, r ;
int e, d;
int message;
int c;

ull primes[20] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71};
						//20개의 서로 다른 소수로 20회 테스트를 한다.

ull powmod(ull x, ull y, ull nn) {		// x^y(mod n)
	x %= nn;
	ull r = 1;
	while (y > 0) {
		if (y % 2 == 1)
			r = (r * x) % nn;
		x = (x * x) % nn;
		y /= 2;
	}
	return r;
}

bool miller_rabin(ull n, ull a) {		//n : 소수인지 아닌지 판별할 수 / a : 테스트를 위해 사용될 소수
	ull d = n - 1;
	while (d % 2 == 0) {
		if (powmod(a, d, n) == n - 1)		//a^d%n == n-1이라면 소수일 수도 있다.
			return true;
		d /= 2;
	}
	ull tmp = powmod(a, d, n);
	if(tmp == n - 1 || tmp == 1)
		return true;					//true : 소수일 수도 있다.
	return false;						//false : 소수가 아니다.
}

bool is_prime(ull n) {
	if (n <= 1)						//1 이하일 경우 소수가 아니다.
		return false;

	for (ull i=0;i<20;i++)		//각 test에 대해 miller_rabin 소수판별법을 수행한다.
		if (!miller_rabin(n, primes[i]))	//소수가 아닐 경우
			return false;					//false 반환
	return true;					//모든 테스트를 통과할 경우 소수라고 판단. true를 반환한다.

}

int gcd(int a, int b) {		//유클리드 호제법을 이용하여 최대공약수를 구한다.
	int c;
	while (b != 0) {		//gcd(a,b) = gcd(b,a%b) = ... = gcd(GCD,0)이므로 반복문을 통해 최대공약수를 찾는다.
		c = a%b;
		a = b;
		b = c;
	}
	return a;
}

int inverse(int ee, int pphi) {			// 확장 유클리드 알고리즘을 통해 (d * e) % phi = 1이 되는 d를 찾는다.
	long r1, r2, q, r, t, t1, t2;
	r1 = ee;
	r2 = pphi;
	t1 = 0; t2 = 1;

	while (r1 != 1)
	{
		q = r2 / r1;
		r = r2 - r1 * q;
		t = t1 - t2 * q;
		r2 = r1;
		r1 = r;
		t1 = t2;
		t2 = t;
	}
	if (t2 < 0)
		t2 = t2 + pphi;
	return t2;
}


int main()
{
	srand(time(NULL));
	
	
	p = rand() % 1024;
	while(!is_prime(p) || p <= 1)		
		p = rand() % 1024;
	q = rand() % 1024;
	while (!is_prime(q) || q <= 1)
		q = rand() % 1024;
	r = rand() % 1024;
	while (!is_prime(r) || r <= 1)
		r = rand() % 1024;
	/*
		p,q,r은 10bit이므로 0(0000000000) ~ 1023(1111111111)까지 가능하다. 따라서 %1024를 해준다.
		또한 p,q,r은 소수이므로 0이 나오는 경우에 다시 구해준다.
	*/

	n = p*q*r;
	phi = (p - 1)*(q - 1)*(r - 1);
	/*
		n과 phi를 구해준다.
	*/

	e = rand() % phi;
	while(gcd(phi,e) != 1)
		e = rand() % phi;
	/*
		e는 phi보다 작고, phi와 서로소인 임의의 숫자이므로
		gcd(phi,e) == 1을 만족하는 e를 구한다.
	*/
	
	d = inverse(e, phi);
	/*
		inverse 함수를 이용해 (d*e)%phi = 1을 만족하는 d값을 구한다.
	*/

	printf("p = %d\nq = %d\nr = %d\n", p, q, r);
	printf("N = %d\nphi = %d\n", n, phi);
	printf("e = %d\nd = %d\n\n", e, d);

	printf("Message Input : ");
	scanf("%d", &message);
	printf("Message : %d\n\n",message);

	/* Encryption */
	printf("**Encryption\n");
	c = powmod(message, e, n);						//c = (m^e)%n
	printf("cipher : %d\n\n", c);


	/* Generate signature */
	char buf[65];
	sprintf(buf, "%d", message);					//message를 버퍼에 복사한다.
	ull hash = XXH64(buf, sizeof(buf) - 1, 0);		//message값에 해당하는 hash값을 구한다.
	hash = hash % n;

	printf("message's hash value : %lu\n",hash);	//hash값을 출력한다.

	ull cc = powmod(hash, d, n);					//구한 hash값을 비밀키로 암호화하여 서명값을 생성한다.
	cc %= n;
	printf("generated signature : %lu\n\n\n", cc);	//생성된 서명값을 출력한다.


	/* Decryption */
	printf("**Decryption\n");

	int descripted = powmod(c, d, n);				//m = (c^d)%n

	printf("decrypted cipher : %d\n\n", descripted);

	/* Verify signature */

	char buf2[65];
	sprintf(buf2, "%d", descripted);				//복호화된 평문을 버퍼에 복사한다.
	ull hash2 = XXH64(buf2, sizeof(buf2) - 1, 0);	//복호화된 평문에 해당하는 hash값을 구한다.
	hash2 = hash2 % n;

	printf("received signature value : %lu\n", cc);						//생성된 서명값을 출력한다.
	printf("decrypted message's hash value : %lu\n", hash2);			//복호화된 평문에 해당하는 hash값을 출력한다.
	printf("verify value from signature : %lu\n", powmod(cc, e, n));	//생성된 서명값을 복호화하여 출력한다.

	if (powmod(cc, e, n) == hash2)
		printf("Signature valid!\n\n");		//
	else
		printf("Signature not valid!\n\n");
}