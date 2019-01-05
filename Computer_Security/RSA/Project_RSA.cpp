/* ��ǻ�� ���� 2������ - 30-bit triple RSA and Digital Signature */

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
						//20���� ���� �ٸ� �Ҽ��� 20ȸ �׽�Ʈ�� �Ѵ�.

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

bool miller_rabin(ull n, ull a) {		//n : �Ҽ����� �ƴ��� �Ǻ��� �� / a : �׽�Ʈ�� ���� ���� �Ҽ�
	ull d = n - 1;
	while (d % 2 == 0) {
		if (powmod(a, d, n) == n - 1)		//a^d%n == n-1�̶�� �Ҽ��� ���� �ִ�.
			return true;
		d /= 2;
	}
	ull tmp = powmod(a, d, n);
	if(tmp == n - 1 || tmp == 1)
		return true;					//true : �Ҽ��� ���� �ִ�.
	return false;						//false : �Ҽ��� �ƴϴ�.
}

bool is_prime(ull n) {
	if (n <= 1)						//1 ������ ��� �Ҽ��� �ƴϴ�.
		return false;

	for (ull i=0;i<20;i++)		//�� test�� ���� miller_rabin �Ҽ��Ǻ����� �����Ѵ�.
		if (!miller_rabin(n, primes[i]))	//�Ҽ��� �ƴ� ���
			return false;					//false ��ȯ
	return true;					//��� �׽�Ʈ�� ����� ��� �Ҽ���� �Ǵ�. true�� ��ȯ�Ѵ�.

}

int gcd(int a, int b) {		//��Ŭ���� ȣ������ �̿��Ͽ� �ִ������� ���Ѵ�.
	int c;
	while (b != 0) {		//gcd(a,b) = gcd(b,a%b) = ... = gcd(GCD,0)�̹Ƿ� �ݺ����� ���� �ִ������� ã�´�.
		c = a%b;
		a = b;
		b = c;
	}
	return a;
}

int inverse(int ee, int pphi) {			// Ȯ�� ��Ŭ���� �˰����� ���� (d * e) % phi = 1�� �Ǵ� d�� ã�´�.
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
		p,q,r�� 10bit�̹Ƿ� 0(0000000000) ~ 1023(1111111111)���� �����ϴ�. ���� %1024�� ���ش�.
		���� p,q,r�� �Ҽ��̹Ƿ� 0�� ������ ��쿡 �ٽ� �����ش�.
	*/

	n = p*q*r;
	phi = (p - 1)*(q - 1)*(r - 1);
	/*
		n�� phi�� �����ش�.
	*/

	e = rand() % phi;
	while(gcd(phi,e) != 1)
		e = rand() % phi;
	/*
		e�� phi���� �۰�, phi�� ���μ��� ������ �����̹Ƿ�
		gcd(phi,e) == 1�� �����ϴ� e�� ���Ѵ�.
	*/
	
	d = inverse(e, phi);
	/*
		inverse �Լ��� �̿��� (d*e)%phi = 1�� �����ϴ� d���� ���Ѵ�.
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
	sprintf(buf, "%d", message);					//message�� ���ۿ� �����Ѵ�.
	ull hash = XXH64(buf, sizeof(buf) - 1, 0);		//message���� �ش��ϴ� hash���� ���Ѵ�.
	hash = hash % n;

	printf("message's hash value : %lu\n",hash);	//hash���� ����Ѵ�.

	ull cc = powmod(hash, d, n);					//���� hash���� ���Ű�� ��ȣȭ�Ͽ� ������ �����Ѵ�.
	cc %= n;
	printf("generated signature : %lu\n\n\n", cc);	//������ ������ ����Ѵ�.


	/* Decryption */
	printf("**Decryption\n");

	int descripted = powmod(c, d, n);				//m = (c^d)%n

	printf("decrypted cipher : %d\n\n", descripted);

	/* Verify signature */

	char buf2[65];
	sprintf(buf2, "%d", descripted);				//��ȣȭ�� ���� ���ۿ� �����Ѵ�.
	ull hash2 = XXH64(buf2, sizeof(buf2) - 1, 0);	//��ȣȭ�� �򹮿� �ش��ϴ� hash���� ���Ѵ�.
	hash2 = hash2 % n;

	printf("received signature value : %lu\n", cc);						//������ ������ ����Ѵ�.
	printf("decrypted message's hash value : %lu\n", hash2);			//��ȣȭ�� �򹮿� �ش��ϴ� hash���� ����Ѵ�.
	printf("verify value from signature : %lu\n", powmod(cc, e, n));	//������ ������ ��ȣȭ�Ͽ� ����Ѵ�.

	if (powmod(cc, e, n) == hash2)
		printf("Signature valid!\n\n");		//
	else
		printf("Signature not valid!\n\n");
}