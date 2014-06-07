// new41.c : Defines the entry point for the console application.
//

#include <stdio.h>
#include <io.h>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)


// for read dir and file search
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")
// end read dir

#include "short_types.h"

extern int main_unpack (u8 *indata, u32 inlen);
extern int main_unpack_getdata1 (u8 *indata, u32 inlen, u8 *cred, u8 *rnd64bit, u32 *sess_id);
extern int main_unpack_getdata2 (u8 *indata, u32 inlen, u8 *nonce);


int convert_str_to_hex(char *ptr);
int show_memory(char *mem, int len, char *text);
int process_str(char *str_buf);


u8 bigbuf[42000];
unsigned int bigbuf_len;


//
// Main
//
int main(char *lol, char *outbuf) {
	int len;

	//main_unpack42(test, test_len);
	//main_unpack(bigbuf, bigbuf_len);

	//init memory

	memset(bigbuf,0,sizeof(bigbuf));
	bigbuf_len = 0;

	process_str(lol);
	len = main_unpack(bigbuf, bigbuf_len, outbuf);

	return len;
};


int process_str(char *str_buf){
	unsigned int i, len;
	u32 hex_digit;
	char *ptr;

	len = strlen(str_buf)-1;
	//printf("%s",str_buf);

	ptr = str_buf;
	for(i=0;i<len;i++){

		if (ptr[0] == '|') {
			ptr++;
			continue;
		};
		if (ptr[0] == ' ') {
			ptr++;
			continue;
		};

		hex_digit=convert_str_to_hex(ptr);
		//printf("hex_digit=0x%08X\n",hex_digit);		

		bigbuf[bigbuf_len]=(char)hex_digit;
		
		bigbuf_len++;
		ptr+=2;
		i++;
	};

	return 0;
};

//
// converting 2 bytes of ptr from ascii to hex
//
int convert_str_to_hex(char *ptr) {
		int hex_digit=0;

		if ((ptr[0]>=0x30) && (ptr[0]<=0x39)){
			hex_digit=ptr[0]-0x30;
		};
		if ((ptr[0]>='A') && (ptr[0]<='F')){
			hex_digit=ptr[0]-0x41+0x0A;
		};
		if ((ptr[0]>='a') && (ptr[0]<='f')){
			hex_digit=ptr[0]-0x41+0x0A;
		};
		hex_digit=hex_digit<<4;

		//printf("ptr[i]=0x%08X\n",ptr[i]);
		//printf("hex_digit=0x%08X\n",hex_digit);

		if ((ptr[1]>=0x30) && (ptr[1]<=0x39)){
			hex_digit+=ptr[1]-0x30;
		};
		if ((ptr[1]>='A') && (ptr[1]<='F')){
			hex_digit+=ptr[1]-0x41+0x0A;
		};
		if ((ptr[1]>='a') && (ptr[1]<='f')){
			hex_digit+=ptr[1]-0x41+0x0A;
		};
		
		//printf("ptr[i]=0x%08X\n",ptr[i+1]);
		//printf("hex_digit=0x%08X\n",hex_digit);

		return hex_digit;
};


int show_memory(char *mem, int len, char *text){
	int zz;
	int i;

	printf("%s\n",text);
	printf("Len: 0x%08X\n",len);

	zz=0;
	for(i=0;i<len;i++){
		printf("%02X ",mem[i] & 0xff);
		zz++;if (zz == 16) { zz=0; printf("\n ");};
	};
	printf("\n");

	return 0;
};

