#ifndef _OPENSSL_MD5_WRAPPER_H_INCLUDED_
#define _OPENSSL_MD5_WRAPPER_H_INCLUDED_

#include <openssl/md5.h>

inline void md5(const char* data, size_t len, std::string& hexmd5)
{
    int m = 0;
    uint8_t bin16[16];

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, len);
    MD5_Final(bin16, &ctx); 

    hexmd5.resize(32);
    char* hex32 = &hexmd5[0];
    // bin2hex
    for (size_t i = 0; i < 16; ++i) {
        m = (bin16[i] & 0xF0) >> 4;
        if (m <= 9) {
            hex32[i << 1] = m + '0';
        } else {
            hex32[i << 1] = m + 'a' - 10;
        }

        m = (bin16[i]) & 0x0F;
        if (m <= 9) {
            hex32[(i << 1) + 1] = m + '0';
        } else {
            hex32[(i << 1) + 1] = m + 'a' - 10;
        }
    }
}

inline void md5(const char* data, size_t len, char* hexmd5)
{
    int m = 0;
    uint8_t bin16[16];

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, len);
    MD5_Final(bin16, &ctx); 

    char* hex32 = &hexmd5[0];
    // bin2hex
    for (size_t i = 0; i < 16; ++i) {
        m = (bin16[i] & 0xF0) >> 4;
        if (m <= 9) {
            hex32[i << 1] = m + '0';
        } else {
            hex32[i << 1] = m + 'a' - 10;
        }

        m = (bin16[i]) & 0x0F;
        if (m <= 9) {
            hex32[(i << 1) + 1] = m + '0';
        } else {
            hex32[(i << 1) + 1] = m + 'a' - 10;
        }
    }
}

inline void md5(const char* data, size_t len, uint64_t &h64, uint64_t &l64)
{
    uint8_t bin16[16];

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, data, len);
    MD5_Final(bin16, &ctx); 

    h64 = 0;
    for (size_t i = 0; i < 8; ++i) {
        h64 = (h64 << 8) | bin16[i];
    }
    l64 = 0;
    for (size_t i = 8; i < 16; ++i) {
        l64 = (l64 << 8) | bin16[i];
    }
}


#endif //_OPENSSL_MD5_WRAPPER_H_INCLUDED_

