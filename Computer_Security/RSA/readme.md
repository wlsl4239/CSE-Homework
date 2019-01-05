# 30-bit triple RSA, Digital Signature  

p, q, r : random prime number (10-bit)  
N = p * q * r  
phi = p-1 * q-1 * r-1  

e : random number

e*d = 1 (mod phi)

- Square and multiply algorithm for modular exponentiation  
- Miller-Rabin Primality test (테스트는 20회로 설정)  
- Extended Euclidean Algorithm  
- Chinese Remainder Theorem   

RSA
C = M^e % N  
M = C^d % N  


Digital Signature  
DS = hash(M) ^ d % N  

Decrypt(DS) = hash(M)을 비교하여 validity 확인.
