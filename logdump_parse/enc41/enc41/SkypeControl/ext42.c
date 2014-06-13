// example of 42 usage
//

#include "skype_basics.h"
#include "skype_rc4.h"
#pragma warning(disable:4311 4312)

#define MAX_MEM		4096

extern int OUTPUT_LEN;


static void errdump (const u8 * const mem, const u32 n, char *outdata)
{
	u32		i, j, m = (__min(n,MAX_MEM)+15)&~15;
	char	s[512], *z;
	
	for (i = 0, z = s, *s = '\0'; i < m; i++)
	{
		if ((i&15)==0) z += sprintf (z, "%04X:", i);
		if (i < n) z += sprintf (z, " %02X", mem[i]); else z += sprintf (z, "   ");
		if ((i&15)==15)
		{
			*z++ =0x20; *z++ ='|'; *z++ =0x20;
			for (j = 0; j < 16; j++) *z++ = (i-15+j >= n) ? 0x20 : (mem[i-15+j]<0x20)||(mem[i-15+j]>0x7E) ? '.' : mem[i-15+j];
			*z++ =0x20; *z++ ='|'; *z++ ='\n'; *z = '\0';
			OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%s", z = s); *s = '\0';
		}
	}
	if (n >= MAX_MEM) OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN, "...\n");
	OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"\n");
}

static void dump_blob (const char *header, const u32 type, const u32 m, const u32 n, char *outdata)
{
	u32			i;
	char		*s, out[1024];
	
	switch (type)
	{
	case 0:	// 32-bit
		OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%s: %02X %02X %02X %02X\n", header, m&0xFF, (m>>8)&0xFF, (m>>16)&0xFF, m>>24);
		break;
	case 1: // 64-bit
		OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%s: %02X %02X %02X %02X %02X %02X %02X %02X\n", header, m&0xFF, (m>>8)&0xFF, (m>>16)&0xFF, m>>24, n&0xFF, (n>>8)&0xFF, (n>>16)&0xFF, n>>24);
		break;
	case 2:	// IP:port
		OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%s: %u.%u.%u.%u:%u\n", header, m>>24, (m>>16)&0xFF, (m>>8)&0xFF, m&0xFF, n);
		break;
	case 3:	// ASCIIZ
		if (byte(m,n-1) != 0) __asm int 3;	// just in case
		OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%s: \"%s\"\n", header, m);
		break;
	case 4:	// BINARY
		OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%s: %d bytes\n", header, n);
		errdump ((void*)m, n, outdata);
		break;
	case 5:	// recursion, gotta handle it upstairs
		__asm int 3;
		break;
	case 6:	// 32-bit words
		s += sprintf (s = out, "%s: ", header);
		for (i = 0; i < n; i += 4) s += sprintf (s, "%02X %02X %02X %02X%s", dword(m,i)&0xFF, (dword(m,i)>>8)&0xFF, (dword(m,i)>>16)&0xFF, dword(m,i)>>24, (i+4<n)?", ":"");
		OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%s\n", out);
		break;
	default:
		__asm int 3;
	}
}

static void dump_41 (const u32 type, const u32 id, const u32 m, const u32 n, char *outdata)
{
	char		aid[12];
	
	switch (id)
	{
//	case 0x00:	report ("SuperNode");
//	case 0x01:	report ("Command");
//	case 0x05:	report ("My Password MD5");
//	case 0x09:	report ("My Credentials Expiry Time");
//	case 0x0D:	report ("Skype Version");
//	case 0x0E:	report ("Login/Key Time/ID?");
//	case 0x20:	report ("My Email");
//	case 0x21:	report ("My Public Key");
//	case 0x24:	report ("My Credentials");
//	case 0x31:	report ("Host ID");
//	case 0x33:	report ("Host IDs");
//	case 0x37:	report ("My Name");
	default:	
		sprintf (aid, "%02X-%02X", type, id); 
		dump_blob(aid, type, m, n, outdata); 
		break;
	}
}

typedef struct _skype_thing
{
	u32				type, id, m, n;
} skype_thing;

typedef struct _skype_list
{
	struct _skype_list	*next;
	skype_thing			*thing;
	u32					allocated_things;
	u32					things;
} skype_list;

static void dump_41_list (const skype_list *list, char *outdata)
{
	u32				i, l;
	
	if (!list) { OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN, "<empty>\n"); return; }
	if (!list->things || !list->thing) { OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN, "<empty>\n"); return; }
	OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"{\n");
	for (i = 0, l = 0; i < list->things; i++)
	{
		if (list->thing[i].type == 5)
		{
			OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"%02X-%02X: ",list->thing[i].type, list->thing[i].id);
			dump_41_list ((skype_list *)list->thing[i].m, outdata);
			continue;
		}
		dump_41 (list->thing[i].type, list->thing[i].id, list->thing[i].m, list->thing[i].n, outdata);
	}
	OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"}\n");
}

/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////

int main_unpack42 (u8 *indata, u32 inlen, char *outdata) {
	u32				list_size;
	u8				*blob_pos = indata;
	u32				packed_bytes = inlen;
	skype_list		new_list = {&new_list, 0, 0, 0};
	int				ret;
	int				myvar=0;
	int				is_pack42;
	int i;


	list_size = 0x5000;

	// do positionning on 42 encoding start
	while (packed_bytes>0){ 			
		if (blob_pos[0]==0x42) {
			break;
		};
		packed_bytes--;
		blob_pos++;
	};

	is_pack42 = 1;
	ret=unpack_4142 ((u32*)&new_list, &blob_pos, &packed_bytes, &is_pack42, 8, &list_size);

	dump_41_list (&new_list, outdata);
	//printf("\n");
	
	return 0;
};


/////////////////////////////////////////////
/////////////////////////////////////////////
/////////////////////////////////////////////


int main_unpack (u8 *indata, u32 inlen, u8 *outdata) {
	u32				list_size;
	u8				*blob_pos = indata;
	u32				packed_bytes = inlen;
	skype_list		new_list = {&new_list, 0, 0, 0};
	int				ret;
	int				myvar=0;
	int				is_pack42;
	int				flag_pack42;
	int				flag_pack41;
	int i;


	list_size = 0x5000;
	
	// check encoding
	// checking first 16 bytes
	flag_pack42=0;
	flag_pack41=0;
	for (i=0;i<16;i++){
		if (blob_pos[i]==0x41) {
			flag_pack41=1;
		};	
		if (blob_pos[i]==0x42) {
			flag_pack42=1;
		};	
	};

	if ((flag_pack42==1) && (flag_pack41==0)) {
		//printf("42 encoding\n");
		OUTPUT_LEN+=sprintf(outdata+OUTPUT_LEN,"42 encoding\n");
		return OUTPUT_LEN;
	};
	// end of encoding check



	// do positionning on 41 encoding start
	while (packed_bytes>0){ 			
		if (blob_pos[0]==0x41) {
			break;
		};
		packed_bytes--;
		blob_pos++;
	};

	is_pack42 = 0;
	ret=unpack_4142 ((u32*)&new_list, &blob_pos, &packed_bytes, is_pack42, 8, &list_size);

	dump_41_list (&new_list, outdata);
	//printf("\n");

	return OUTPUT_LEN;
	//return 0;
};




/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////



static int dump_41_list_getbuf (const skype_list *list, u8 *membuf, int *membuf_len, int type, int id) {
	u32				i, l;
	
	if (!list) { 
		return 0; 
	}
	if (!list->things || !list->thing) { 
		return 0; 
	}
	for (i = 0, l = 0; i < list->things; i++) {
		
		if (list->thing[i].type == 5) {
			dump_41_list_getbuf((skype_list *)list->thing[i].m, membuf, membuf_len, type, id);
			continue;
		};
		

		// list->thing[i].n -- buf size

		if ((list->thing[i].type == type) && (list->thing[i].id == id)) {
			memcpy(membuf,(char *)list->thing[i].m, list->thing[i].n);
			*membuf_len = list->thing[i].n;
			//printf("list->thing[i].n: %08X\n",list->thing[i].n);
			//printf("list->thing[i].m: %08X\n",list->thing[i].m);
			//printf("list->thing[i].type: %08X\n",list->thing[i].type);
			//printf("list->thing[i].id: %08X\n",list->thing[i].id);
			return 1;
		};

	}

	return 0;
}




int main_unpack_getbuf (u8 *indata, u32 inlen, u8 *membuf, int *membuf_len, int type, int id) {
	u32				list_size;
	u8				*blob_pos = indata;
	skype_list		new_list = {&new_list, 0, 0, 0};
	u32				packed_bytes;
	int				ret;

	// stack mess
	int				myvar1=1;
	int				myvar2=1;
	int				myvar3=1;
	int				myvar4=1;
	int				myvar5=1;
	int				myvar6=1;
	int				myvar7=1;
	int				myvar8=0;


	packed_bytes=inlen;

	list_size = 0x5000;

	while ((packed_bytes>0) && (blob_pos[0]!=0x41)){
		packed_bytes--;
		blob_pos++;
	};


	ret=1;
	while( (packed_bytes>0) && (ret==1) ){
		
		ret=unpack_4142((u32*)&new_list, &blob_pos, &packed_bytes, 0, 8, &list_size);

		while ((packed_bytes>0) && (blob_pos[0]!=0x41)){
			packed_bytes--;
			blob_pos++;
		};

	};

	dump_41_list_getbuf (&new_list, membuf, membuf_len, type, id);
	
	return 0;
};


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////


static int dump_41_list_checkblob (const skype_list *list, int type, int id) {
	u32	i, l;
	int ret;
	
	if (!list) { 
		return 0; 
	}
	if (!list->things || !list->thing) { 
		return 0; 
	}
	for (i = 0, l = 0; i < list->things; i++) {
		
		if (list->thing[i].type == 5) {
			ret = dump_41_list_checkblob((skype_list *)list->thing[i].m, type, id);
			if (ret==1){
				return 1;
			}

			continue;
		};

		if ((list->thing[i].type == type) && (list->thing[i].id == id)) {
			return 1;
		};

	}


	return 0;
}




int main_unpack_checkblob (u8 *indata, u32 inlen, int type, int id) {
	u32				list_size;
	u8				*blob_pos = indata;
	skype_list		new_list = {&new_list, 0, 0, 0};
	u32				packed_bytes;
	int				ret;

	// stack mess
	int				myvar1=1;
	int				myvar2=1;
	int				myvar3=1;
	int				myvar4=1;
	int				myvar5=1;
	int				myvar6=1;
	int				myvar7=1;
	int				myvar8=0;


	packed_bytes=inlen;

	list_size = 0x5000;

	while ((packed_bytes>0) && (blob_pos[0]!=0x41)){
		packed_bytes--;
		blob_pos++;
	};


	ret=1;
	while( (packed_bytes>0) && (ret==1) ){
		
		ret=unpack_4142 ((u32*)&new_list, &blob_pos, &packed_bytes, 0, 8, &list_size);

		while ((packed_bytes>0) && (blob_pos[0]!=0x41)){
			packed_bytes--;
			blob_pos++;
		};

	};

	ret = dump_41_list_checkblob(&new_list, type, id);
	
	return ret;
};


/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////

