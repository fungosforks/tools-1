//
// 7 bit routines
//

#include <stdio.h>
#include <stdlib.h>



int get_packet_size2(char *data, int len, int *header_len){
	unsigned int ebx, edi, esi, eax, ecx, ebp, edx;
	char *buf_eax;
	int buf_count;

	buf_count = 0;
	//printf("ENTER unpack_7_bit_encoded_to_dword \n");
	
	ebx=len;

	esi=0;
	edi=0;

	eax=ebx;

	// if len == 0 
	if (eax==0){
			printf("konchilsya buffer,smth like terra nova here, jmp hz kuda\n");
			exit(-1);
	};


	// len - 1 
	ecx=eax-1;
	ebx=ecx;
    
	//ptr on data buffer
	ebp=(int )data;

	do{
		eax=ebp;

		ecx=esi;

		esi=esi+7;

		buf_eax=(char *)eax;
		edx=buf_eax[0] & 0xff; //ptr

		//printf("readed byte edx=%X\n",edx);

		eax++;

		ebp=eax;

		buf_count++;

		eax=edx;

		eax=eax & 0x7f;
		eax=eax << ecx;

		ecx=edi;

		ecx=ecx | eax;

		edi=ecx;

	    //printf("accamulated int ecx=%X\n",ecx);

	}while(edx >= 0x80);  
	//loop, while byte readed from buf >=0x80


	// size specific
	// diveded by 2
	edi=edi>>1;

	printf("PKT SIZE=0x%08X\n",edi);
	printf("HEADER_LEN SIZE=0x%08X\n",buf_count);

	if (1) {
		int h_len = buf_count;
		int t_len = edi;

		if (h_len == 0){
			printf("pkt header_len wrong, len: 0x%08X\n", h_len);
			exit(1);
		};
		if (h_len > 3){
			printf("pkt header_len wrong, len: 0x%08X\n", h_len);
			exit(1);
		};
		if (t_len > 0x1000){
			printf("pkt block size too big, len: 0x%08X\n",t_len);
			exit(1);
		};
		if (t_len <= 0){
			printf("pkt block size too small, len: 0x%08X\n",t_len);
			exit(1);
		};
	};


	//printf("LEAVE unpack_7_bit_encoded_to_dword \n");

	*header_len = buf_count;

	return edi;
};


int main() {
	unsigned char membuf[0x1000];
	int header_len;
	int tmplen;

	//memcpy(membuf, "\xA1\x85\x02", 3);

	memcpy(membuf, "\x93\x8D\x02", 3);
	
	memcpy(membuf, "\xC9\x6E", 2);

	
	show_memory(membuf, 2, "Input:");

    // pass 1-3 bytes of header
    header_len = 0;
    tmplen = get_packet_size2(membuf, 4, &header_len);
    //tmplen = tmplen-1;
    
    printf("7bit decoded len: %d\n", tmplen);

};


int show_memory(char *mem, int len, char *text) {
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


