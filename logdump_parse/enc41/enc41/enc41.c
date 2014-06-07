// enc41.cpp : Defines the exported functions for the DLL application.
//


#include <stdio.h>


#define EXTERN_DLL_EXPORT __declspec(dllexport)

EXTERN_DLL_EXPORT int __stdcall sayhello(char *lol, char *outbuf) {
	int ret;

	//printf("Input Data:\n%s\n", lol);

	ret = main(lol, outbuf);

	//printf("Output Data:\n%s\n", outbuf);

	return ret;
}

