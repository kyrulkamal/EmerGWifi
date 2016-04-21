/*
 * main.c
 * Test case for pnesign function
 * 25 November 2015
 */
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "signature/pnesign.h"            // must include this header to use pnesign()

int main()
{
	char* message = "This is a test message aFAdasdaSDA skad kasdbkADAbd kaDASBDKJASB d asdkjasdk sabdk dbkjsaBD KJASbdkj adkasb kjasD BKJBsbdkj";
	char* key = "abc123";
	
	uint16_t sign;						// Signature will be stored here
	
	//
	//usage:
	//
	// <16bit_signature> = pnesign(<pointer to message>, <length of message>, <pointer to key>, <length of key>)
	//
	sign = pnesign(message, strlen(message), key, strlen(key)); //  <--- the sign function
	
	printf("Signature: %4x", sign);
	
	return 0;
}


//note:
//
// make sure that signature/pnesign.c in included in the list of files to compile
//