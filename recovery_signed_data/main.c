/*
 *   Program to encode text using RSA public key.
 *
 *   *** For Demonstration use only *****
 *
 *   Copyright (c) 1988-1997 Shamus Software Ltd.
 *
 *
 *   Decoding 04-04 signed blocks by Efim Bushmanov
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>


// for PathStripPath
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
// end of PathStripPath include


#include "include/miracl.h"

#include "short_types.h"

extern int show_memory(char *mem, int len, char *text);

extern int get_04_03_blob(u8 *buf, int len, u8 *membuf, int *membuf_len);
extern int get_04_04_blob(u8 *buf, int len, u8 *membuf, int *membuf_len);

extern int get_00_01_blob(u8 *buf, int buf_len, int *chat_cmd);
extern int get_00_02_blob(u8 *buf, int buf_len, int *uic_crc);
extern int get_00_04_blob(u8 *buf, int buf_len, int *expire_time);
extern int get_00_09_blob(u8 *buf, int buf_len, int *created_time);


miracl *mip;

char user_pubkey[0x81];

///////////////////////
// needed for some operations
///////////////////////
char skype_pub[]=
"\xB8\x50\x6A\xEE\xD8\xED\x30\xFE\x1C\x0E\x67\x74\x87\x4B\x59\x20"
"\x6A\x77\x32\x90\x42\xA4\x9B\xE2\x40\x3D\xA4\x7D\x50\x05\x24\x41"
"\x06\x7F\x87\xBC\xD5\x7E\x65\x79\xB8\x3D\xF0\xBA\xDE\x2B\xEF\xF5"
"\xB5\xCD\x8D\x87\xE8\xB3\xED\xAC\x5F\x57\xFA\xBC\xCD\x49\x69\x59"
"\x74\xE2\xB5\xE5\xF0\x28\x7D\x6C\x19\xEC\xC3\x1B\x45\x04\xA9\xF8"
"\xBE\x25\xDA\x78\xFA\x4E\xF3\x45\xF9\x1D\x33\x9B\x73\xCC\x2D\x70"
"\xB3\x90\x4E\x11\xCA\x57\x0C\xE9\xB5\xDC\x4B\x08\xB3\xC4\x4B\x74"
"\xDC\x46\x35\x87\xEA\x63\x7E\xF4\x45\x6E\x61\x46\x2B\x72\x04\x2F"
"\xC2\xF4\xAD\x55\x10\xA9\x85\x0C\x06\xDC\x9A\x73\x74\x41\x2F\xCA"
"\xDD\xA9\x55\xBD\x98\x00\xF9\x75\x4C\xB3\xB8\xCC\x62\xD0\xE9\x8D"
"\x82\x82\x18\x09\x71\x05\x5B\x45\x7C\x06\xF3\x51\xE6\x11\x64\xFC"
"\x5A\x9D\xE9\xD8\x3D\x1D\x13\x78\x96\x40\x01\x38\x0B\x5B\x99\xEE"
"\x4C\x5C\x7D\x50\xAC\x24\x62\xA4\xB7\xEA\x34\xFD\x32\xD9\x0B\xD8"
"\xD4\xB4\x64\x10\x26\x36\x73\xF9\x00\xD1\xC6\x04\x70\x16\x5D\xF9"
"\xF3\xCB\x48\x01\x6A\xB8\xCA\x45\xCE\x68\x75\xA7\x1D\x97\x79\x15"
"\xCA\x82\x51\xB5\x02\x58\x74\x8D\xBC\x37\xFE\x33\x2E\xDC\x28\x55"
;
////////////////////////





struct pubkey {
	char credentials[0x105];
	char user_pubkey[0x81];
	int UIC_CRC;
};

//
// public key to 'unsgin' signed data
//
//themagic_pub[0x80+1]=


struct pubkey pubkeys_db[10] = { 
	{
"\x00\x00\x00\x01\xA6\x60\x57\xB4\xC5\x2B\x48\x0C\xBA\xF6\x0D\x1F"
"\x60\xCD\xFD\x53\xCA\x42\x40\x54\x47\x73\x03\xDD\x7D\xE9\x9B\x76"
"\x40\xA5\x34\x7E\xE0\x72\x23\x7D\x21\x4A\x68\x10\xE3\xD6\x78\x2D"
"\xDB\x3E\xFB\xDA\x71\xE9\x13\x8D\xF1\xDC\x9B\x04\x03\x00\x8F\x4F"
"\x04\xA5\x44\xBE\x33\xFD\xF1\xE9\xB3\xC1\xBB\xB7\xDE\x08\x58\xD1"
"\x98\x24\x30\xFA\x50\xE8\x91\xAE\xC5\xD0\x20\xDA\x8D\x54\x3C\xEB"
"\xC7\x8F\x16\x22\x4D\xE8\x59\xCB\xEB\xED\x56\xA2\xCE\xDF\xD2\xA6"
"\xB6\xA4\xAB\xCA\x36\x00\x4C\x85\xDB\xE4\x44\x6A\x6A\xD7\x19\x7B"
"\xA5\x33\xF4\x03\xAC\x0C\x68\x81\x51\xFC\xAF\x47\x8B\x5A\x18\x78"
"\x76\x04\xF4\xA5\xD9\x49\x96\x6A\x96\x0E\x8B\xE2\xAB\xF6\xE7\xF2"
"\x3D\xF7\x7B\x11\x82\xBD\x77\x85\xF8\x6D\xDF\xC6\xBA\x9F\xE7\xF3"
"\x6C\x4F\x23\xFB\x76\x6E\xE8\x12\x37\x32\x67\xC2\x92\xC0\x45\x67"
"\x90\xAB\x22\x96\x32\xC2\xFB\x64\x5E\x9D\x7F\x52\x23\x94\x03\xDB"
"\x42\xC1\x55\x83\xF6\x59\xDF\xDF\x6A\x3E\x57\x27\x48\x92\xD4\xBC"
"\xCA\x62\x3E\x46\x68\x02\xBF\x93\x9E\xB3\x2E\xA9\xC9\x06\xE7\x60"
"\xC2\x0B\x33\x40\x45\xAD\x81\x37\x1C\x63\x96\xD2\x45\x7C\x77\xDF"
"\xDF\x41\x85\x61"
,
"\xD8\x81\xDE\x8F\x79\x18\xDD\xFE\x68\x46\x80\xD7\xD0\xEB\x0C\x0A"
"\xAA\xF3\xED\xAC\xA9\xA4\x66\x7B\x7C\xFF\x16\xC3\x5A\x65\xDB\xA9"
"\x7C\xA2\xD6\xB7\x11\xE1\xFF\xA3\xEE\x85\xDF\xF1\x0D\x47\x5E\x31"
"\xE7\xF2\x91\x62\x20\x07\x00\x6B\x18\x31\xB7\x50\xC7\x21\xF4\xCB"
"\x6E\x5E\x51\xCE\xFD\x53\x16\x87\x03\xFB\x2E\xC0\x96\xDD\xFD\x7E"
"\x2D\xB4\xBC\xC4\xA4\x2D\x0E\x52\x9A\x2B\xBE\xCC\x34\xDC\x9C\x17"
"\xEB\xE3\x83\x1F\x55\x13\x75\xFF\xD0\xF1\x08\xA7\x8F\xFF\x81\xD5"
"\xC6\x95\x39\x12\xF0\xF0\x27\xF5\x21\xFA\xD8\x94\x65\x52\xDE\xF5"
,
0x00
	},
	{
		0x00, 0x00, 0x00
	},
	{
		0x00, 0x00, 0x00
	},
	{
		0x00, 0x00, 0x00
	},
};




int rsa_unsign_profile(char *buf, int len, char *outbuf) {  

    big e,m,kn;
	
	mip=mirsys(100,0);

    e=mirvar(0);
    m=mirvar(0);
    kn=mirvar(0);

    bytes_to_big(0x100,buf,m);
	bytes_to_big(0x100,skype_pub,kn);

	power(m,65537,kn,e);
	
	big_to_bytes (0x100, e, outbuf, TRUE);

    return 0;
}


int rsa_unsign_profile_data(char *buf, int len, char *outbuf) {  

    big e,m,kn;
	
	mip=mirsys(100,0);

    e=mirvar(0);
    m=mirvar(0);
    kn=mirvar(0);

    bytes_to_big(0x80,buf,m);
	bytes_to_big(0x80,user_pubkey,kn);

	power(m,65537,kn,e);
	
	big_to_bytes (0x80, e, outbuf, TRUE);

    return 0;
}


// check if credentials expired
int do_time_check(char *outbuf, int outbuf_len){
	int expire_time;
	int created_time;
	time_t lexpire_time;
	time_t lcreated_time;
	struct tm *newtime;
	time_t lcurrent_time;

	printf("\n");

	// add to get blob of Expire time:
	get_00_04_blob(outbuf, 0x100, &expire_time);
	expire_time = expire_time * 60;
	lexpire_time = expire_time;
	printf("Cert expire time: %d\n", lexpire_time);
	newtime = localtime( &lexpire_time );
	printf("%s\n", asctime(newtime));

	// add to get blob of Created Time:
	get_00_09_blob(outbuf, 0x100, &created_time);
	// *60 for get seconds,
	// and minus year for unknown reason...
	created_time = created_time * 60 - 31536000;
	lcreated_time = created_time;
	printf("Cert created time: %d\n", lcreated_time);
	newtime = localtime( &lcreated_time );
	printf("%s\n", asctime(newtime));

	// checking current time
	time(&lcurrent_time);
	if (lcurrent_time > lexpire_time) {
		printf("Certificate expired!\n");
		printf("(!!!)\n");
		//exit(1);
	};

	printf("\n");

	return 0;
};


int decode_profile(char *remote_credentials, int remote_credentials_len, char *remote_signblock, int remote_signblock_len){
	u8 outbuf[0x100];
	u8 signed_text[0x200];
	int signed_text_len;
	int i;

	printf("Unsigning first 0x100 bytes...\n");
	rsa_unsign_profile(remote_credentials+4,0x100,outbuf);
	show_memory(outbuf,0x100,"unsign cred:");

	// save remote user pubkey
	for(i=0;i<0x100;i++){
		if((outbuf[i]==0x80) && (outbuf[i+1]==0x01)){
			memcpy(user_pubkey,outbuf+i+2,0x80);
		};
	};
	
	show_memory(user_pubkey,0x80,"user pubkey data:");
	
	printf("remote credentials:\n");
	main_unpack(outbuf, 0x100);

	do_time_check(outbuf, 0x100);

	printf("Unsigning 04-03 signblock 0x80 bytes...\n");
	rsa_unsign_profile_data(remote_signblock,0x80,outbuf);
	show_memory(outbuf,0x80,"unsign data:");

	// need to concate with last bytes after 0x80, and remove sha1 in decrypted.
	signed_text_len = 0;
	
	// remove sha1
	memcpy(signed_text, outbuf, 0x80-0x15);
	signed_text_len += 0x80-0x15;

	// move data (rest of bytes in signblock) right after signed text
	memcpy(signed_text+0x80-0x15, remote_signblock+0x80, remote_signblock_len - 0x80);
	signed_text_len += remote_signblock_len - 0x80;

	printf("===\n");
	printf("SIGNED TEXT\n");
	printf("===\n");

	show_memory(signed_text, signed_text_len, "recovered signed data:");
	main_unpack(signed_text, signed_text_len);

	return 0;
};

int init_crypto(){
	int UIC_CRC;
	int i;
	int arr_size;

	arr_size = sizeof(pubkeys_db) / sizeof(pubkeys_db[0]);
	printf("Cert array size: %d\n", arr_size);
	
	printf("Calculate UIC_CRC for all certs...\n");
	for(i=0;i<arr_size;i++) {
		printf("%d\n",i);
		UIC_CRC=Calculate_CRC32( (char *)pubkeys_db[i].credentials, 0x104);
		pubkeys_db[i].UIC_CRC = UIC_CRC;
		printf("UIC_CRC = %08X\n",UIC_CRC);
	};

	return 0;
};


int main(int argc, char* argv[]) {
	char *progname;
	char *filename;	
	int CHAT_CMD;
	int UIC_CRC;
	int i;
	int arr_size;

	char buf41[42000];
	int buf41_len;
	char membuf[0x1000];
	int membuf_len;

	char remote_credentials[0x104+1];
	int remote_credentials_len;
	char remote_signblock[0x1000];
	int remote_signblock_len;


	if (argc < 2){
		progname = argv[0];
		PathStripPathA(progname);
		printf("Error! No input file provided.\n");
		printf("Usage: %s <filename>\n", progname);
		exit(1);
	};

	filename = argv[1];
	printf("Processing file: %s\n", filename);
	
	init_crypto();

	buf41_len = fill_data_buf(filename, buf41);

	membuf_len = 0;
	get_04_04_blob(buf41, buf41_len, membuf, &membuf_len);
	main_unpack(membuf, membuf_len);


	CHAT_CMD = 0;
	get_00_01_blob(membuf, membuf_len, &CHAT_CMD);
	printf("CHAT_COMMAND: 0x%02X\n", CHAT_CMD);

	if (CHAT_CMD == 0){
		printf("No found 00-01 blob with CHAT_COMMAND!\n");
		exit(1);
	};

	// chatsign or headersign
	if ((CHAT_CMD == 0x24) || (CHAT_CMD == 0x2A)){
		remote_signblock_len = 0;
		get_04_03_blob(membuf, membuf_len, remote_signblock, &remote_signblock_len);
		printf("Remote signblock len: 0x%08X\n", remote_signblock_len);

		remote_credentials_len = 0;
		get_04_04_blob(membuf, membuf_len, remote_credentials, &remote_credentials_len);
		printf("Remote credentials len: 0x%08X\n", remote_credentials_len);
	} else {

		if (CHAT_CMD == 0x2B){
			// msgsign
			// get uic_crc from blob, cmp with our base
			// make struct-es { char *pubkey, uic_crc } etc...
			UIC_CRC = 0;
			get_00_02_blob(membuf, membuf_len, &UIC_CRC);
			printf("UIC_CRC: 0x%08X\n", UIC_CRC);
			printf("Pubkey CRC: 0x%08X\n", pubkeys_db[0].UIC_CRC);

			arr_size = sizeof(pubkeys_db) / sizeof(pubkeys_db[0]);
			printf("Cert array size: %d\n", arr_size);
			
			remote_credentials_len = 0;
			printf("Try to find needed keys...\n");
			for(i=0;i<arr_size;i++) {
				printf("%d\n",i);
				if (pubkeys_db[i].UIC_CRC == UIC_CRC){
					printf("Cert and pubkey found!\n");
					printf("Cert index: %d\n", i);
					printf("UIC_CRC: 0x%08X\n", UIC_CRC);
					// fill remote_credentials
					memcpy(remote_credentials, pubkeys_db[i].credentials, 0x104);
					remote_credentials_len = 0x104;
					break;
				};
			};
			if (remote_credentials_len == 0) {
				printf("ERROR: Cert and pubkey not found!\n");
				printf("Needed UIC_CRC: 0x%08X\n", UIC_CRC);
				exit(1);
			};
			
			// fill remote_signblock
			remote_signblock_len = 0;
			get_04_03_blob(membuf, membuf_len, remote_signblock, &remote_signblock_len);
			printf("Remote signblock len: 0x%08X\n", remote_signblock_len);

		} else {
			// unknown command (not signed data)
			printf("CHAT_COMMAND unknow: 0x08X\n", CHAT_CMD);
			exit(1);
		};

	};
	

	decode_profile(remote_credentials, remote_credentials_len, remote_signblock, remote_signblock_len);


	return 0;
}
