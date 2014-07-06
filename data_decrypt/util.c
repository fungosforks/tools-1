/*
 *
 *
 * Some useful functions
 *
 *
 */


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


