//
// get blob data
//

#include <stdio.h>
#include <stdlib.h>

#include "short_types.h"

extern int main_unpack_checkblob (u8 *indata, u32 inlen, int type, int id);
extern int main_unpack_getbuf (u8 *indata, u32 inlen, u8 *membuf, int *membuf_len, int type, int id);
extern int main_unpack_getobj00 (u8 *indata, u32 inlen, u32 *data_int, int type, int id);

extern int show_memory(char *mem, int len, char *text);



int get_04_04_blob(u8 *buf, int len, u8 *membuf, int *membuf_len) {
	int ret;

	printf("Looking for 04-04 blob...\n");
	ret = main_unpack_checkblob(buf, len, 0x04, 0x04);
	if (ret == 1){
		printf("BLOB found!\n");
		main_unpack_getbuf(buf, len, membuf, membuf_len, 0x04, 0x04);
		if (membuf_len<=0) {
			printf("unpack_getbuf size error\n");
			exit(-1);
		};
		printf("MEMBUF_LEN: %d bytes\n", *membuf_len);
		show_memory(membuf, *membuf_len, "MEMBUF");

	} else {
		printf("04-04 blob not found\n");
		exit(1);
	};

	return 0;
};



//
// 04-03 signed block
//
int get_04_03_blob(u8 *buf, int len, u8 *membuf, int *membuf_len) {
	int ret;

	printf("Looking for 04-03 blob...\n");
	ret = main_unpack_checkblob(buf, len, 0x04, 0x03);
	if (ret == 1){
		printf("BLOB found!\n");
		main_unpack_getbuf(buf, len, membuf, membuf_len, 0x04, 0x03);
		if (membuf_len<=0) {
			printf("unpack_getbuf size error\n");
			exit(-1);
		};
		printf("MEMBUF_LEN: %d bytes\n", *membuf_len);
		show_memory(membuf, *membuf_len, "MEMBUF");

	} else {
		printf("04-03 blob not found\n");
		exit(1);
	};

	return 0;
};


//
// 00-01 chat command
//
int get_00_01_blob(u8 *buf, int buf_len, int *chat_cmd){
	int ret;
	unsigned long data_int;

	printf("Looking for 00-01 (remote cmd) blob...\n");
	ret = main_unpack_checkblob(buf, buf_len, 0x00, 0x01);
	if (ret == 1){
		printf("BLOB found!\n");
		main_unpack_getobj00(buf, buf_len, &data_int, 0x00, 0x01);
		printf("00-01 (CHAT CMD ID): 0x%08X\n", data_int);
		*chat_cmd = data_int;
	} else {
		printf("not found blob 00-01 in 6D --> 05-03 --> 04-04\n");
	};

	return 0;
};


//
// 00-02 (in 04-04 blob) UIC_CRC
//
int get_00_02_blob(u8 *buf, int buf_len, int *uic_crc){
	int ret;
	unsigned long data_int;

	printf("Looking for 00-02 (remote cmd) blob...\n");
	ret = main_unpack_checkblob(buf, buf_len, 0x00, 0x02);
	if (ret == 1){
		printf("BLOB found!\n");
		main_unpack_getobj00(buf, buf_len, &data_int, 0x00, 0x02);
		printf("00-02 (UIC CRC): 0x%08X\n", data_int);
		*uic_crc = data_int;
	} else {
		printf("not found blob 00-02 in 6D --> 05-03 --> 04-04\n");
	};

	return 0;
};


//
// 00-04 (in credentials) Expire Time
//
int get_00_04_blob(u8 *buf, int buf_len, int *expire_time){
	int ret;
	unsigned long data_int;

	printf("Looking for 00-04 blob...\n");
	ret = main_unpack_checkblob(buf, buf_len, 0x00, 0x04);
	if (ret == 1){
		printf("BLOB found!\n");
		main_unpack_getobj00(buf, buf_len, &data_int, 0x00, 0x04);
		printf("00-04 (Expire Time): 0x%08X\n", data_int);
		*expire_time = data_int;
	} else {
		printf("not found blob 00-04 in cert\n");
	};

	return 0;
};


//
// 00-09 (in credentials) Created Time
//
int get_00_09_blob(u8 *buf, int buf_len, int *created_time){
	int ret;
	unsigned long data_int;

	printf("Looking for 00-09 blob...\n");
	ret = main_unpack_checkblob(buf, buf_len, 0x00, 0x09);
	if (ret == 1){
		printf("BLOB found!\n");
		main_unpack_getobj00(buf, buf_len, &data_int, 0x00, 0x09);
		printf("00-09 (Created Time): 0x%08X\n", data_int);
		*created_time = data_int;
	} else {
		printf("not found blob 00-09 in cert\n");
	};

	return 0;
};
