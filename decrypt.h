/************************************************************************
 * RSA加密解密函数
 *
 * file: test_rsa_encdec.c
 * gcc -Wall -O2 -o test_rsa_encdec test_rsa_encdec.c -lcrypto -lssl
 *
 * author: tonglulin@gmail.com by www.qmailer.net
 * new author : 曾润良
 ************************************************************************/
#include<stdio.h>
#include<string.h>
#include<iostream>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#define BUFFSIZE 4096
using namespace std;
//解密类
class my_decrypt
{
	public:
		char *de;//解密后
		char *str;//加密后的
		my_decrypt( char *en,int len);
		~my_decrypt();
};

