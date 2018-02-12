#include "decrypt.h"

my_decrypt::my_decrypt( char *en,int len):de(NULL)
{

	str = new char[len];
	memcpy(str,en,len);
	RSA *rsa = NULL;
	FILE *fp = NULL;
	int rsa_len = 0;

	if ((fp = fopen("prikey.pem", "r")) == NULL) {
		cout<<"prikey open failed"<<endl;
		return ;
	}

	if ((rsa = PEM_read_RSAPrivateKey(fp, NULL, NULL, NULL)) == NULL) {
		cout<<"pem read"<<endl;
		return ;
	}

	rsa_len = RSA_size(rsa);
	de = (char *)malloc(rsa_len + 1);
	memset(de, 0, rsa_len + 1);

	if (RSA_private_decrypt(rsa_len, (unsigned char *)str, (unsigned char*)de, rsa, RSA_NO_PADDING) < 0) {
		cout<<"rsa private"<<endl;
		return ;
	}
	cout<<"de ok"<<endl;
	if(de !=NULL)
		RSA_free(rsa);
	fclose(fp);
}
my_decrypt::~my_decrypt()
{
	if(de != NULL){
		free(de);
		de = NULL;
	}
	delete [] str;
	str = NULL;
}
