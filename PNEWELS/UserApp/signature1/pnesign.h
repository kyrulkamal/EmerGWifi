/**
 * md5.h
 * Part of PNEWELS Project
 * 25 November 2015 
 */
 
#include <stdint.h>

#ifndef PNEUTM_MD5_H
#define PNEUTM_MD5_H

typedef struct
{
    unsigned long total[2];     
    unsigned long state[4];     
    unsigned char buffer[64];   
    unsigned char ipad[64];     
    unsigned char opad[64];     
}
md5_context;

#ifdef __cplusplus
extern "C" {
#endif

void md5_starts( md5_context *ctx );

void md5_update( md5_context *ctx, const unsigned char *input, int ilen );

void md5_finish( md5_context *ctx, unsigned char output[16] );

void md5( const unsigned char *input, int ilen, unsigned char output[16] );

int md5_file( const char *path, unsigned char output[16] );

void md5_hmac_starts( md5_context *ctx, const unsigned char *key, int keylen );

void md5_hmac_update( md5_context *ctx, const unsigned char *input, int ilen );

void md5_hmac_finish( md5_context *ctx, unsigned char output[16] );

void md5_hmac_reset( md5_context *ctx );

void md5_hmac( const unsigned char *key, int keylen, const unsigned char *input, int ilen, unsigned char output[16] );

uint16_t pnesign(const unsigned char* data, uint16_t dataLen, const unsigned char* key, uint16_t keyLen);

#if defined(PNEUTM_SELF_TEST)
int md5_self_test( int verbose );
#endif

#ifdef __cplusplus
}
#endif

#endif /* md5.h */