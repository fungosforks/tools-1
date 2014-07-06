//
// util.c
//

#include <stdio.h>
#include <io.h>

// for read dir and file search
#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")
// end read dir

#include "short_types.h"



int fill_data_buf(char *file, char *buf41) {
	FILE *fp;
	char str_buf[4096];
	int err;
	int buf41_len;
	int MAX41BUF;

	MAX41BUF = 42000;
	buf41_len = 0;

	fp = fopen(file, "r");
	if (fp == NULL){
		perror("Open failed on input file1");
		exit(1);
	};
	
	while(!feof(fp)){
		if ( fgets(str_buf, sizeof(str_buf), fp) != NULL ) {
			process_str(str_buf, buf41, &buf41_len);
			if (buf41_len > MAX41BUF){
				printf("buf41len: 0x%08X\n", buf41_len);
				printf("text file too big, internal buffer overrun\n");
				exit(1);
			};
		};
	};

	fclose(fp);

	show_memory(buf41, buf41_len, "Input Buffer:");
	main_unpack(buf41, buf41_len);

	return buf41_len;
}


int process_str(char *str_buf, char *buf41, int *buf41_len){
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

		buf41[*buf41_len]=(char)hex_digit;
		
		(*buf41_len)++;
		ptr+=2;
		i++;
	};

	return buf41_len;
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
	int k;
	char b[16+1];
	int t;

	printf("%s\n",text);
	printf("Len: 0x%08X\n",len);

	zz=0;
	k=0;
	b[16]=0;
	for(i=0;i<len;i++){
		printf("%02X ",mem[i] & 0xff);
		t=mem[i] & 0xff;
		if ((t>=0x20) && (t<=0x7f)){
			memcpy(b+k,mem+i,1);
		}else{
			memcpy(b+k,"\x20",1);
		};
		zz++;
		k++;
		if (zz == 16) { 
			zz=0;
			k=0;
            printf(" ; %s",b);
			printf("\n ");

		};
	};
	printf("\n");

	return 0;
};

