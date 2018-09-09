//compile with clang:
//clang -O3 md5c.c md5test.c -o ./bin/md5test.exe

//compile with msvc:
//cl.exe /Fo.\bin\ /Ox md5c.c md5test.c /link /out:.\bin\md5test.exe

//compile with gcc:
//gcc -O3 md5c.c md5test.c -o ./bin/md5test.exe

#include <stdio.h>
#include <string.h>
#include "md5-rfc1321/global.h"
#include "md5-rfc1321/md5.h"

static void Print(digest)
        unsigned char digest[16];
{
    unsigned int i;
    for (i = 0; i < 16; i++)
        printf ("%02x", digest[i]);
    printf("\n");
}

static void MD5String(string)
        char *string;
{
    MD5_CTX context;
    unsigned char digest[16];
    unsigned int len = strlen(string);

    MD5Init(&context);
    MD5Update(&context, string, len);
    MD5Final(digest, &context);

    printf("MD5(\"%s\") = ", string);
    Print(digest);
}

//declared and defined at md5c.c
void compute_md5(unsigned char* input_text, unsigned int input_size, unsigned char output_digest[16]);

int main() 
{
    printf("tests:\n");
    MD5String("");
    MD5String("The quick brown fox jumps over the lazy dog");
    MD5String("The quick brown fox jumps over the lazy dog.");
    MD5String("njfxhljp");

    unsigned char digest[16];
    compute_md5((unsigned char*)"", 0, digest); Print(digest);
    compute_md5((unsigned char*)"The quick brown fox jumps over the lazy dog", 43, digest); Print(digest);
    compute_md5((unsigned char*)"The quick brown fox jumps over the lazy dog.", 44, digest); Print(digest);
    compute_md5((unsigned char*)"njfxhljp", 8, digest); Print(digest);
    return 0;
}