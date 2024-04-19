#ifndef lzw_decompress_hpp
#define lzw_decompress_hpp

#include "bsread.hpp"

#pragma pack(push, 1)

typedef struct
{
    short prefix;
    unsigned char c;
} code;

typedef enum {LZW_CLEAR_CODE, LZW_CHAR_READ, LZW_BUFFER_EMPTY, LZW_END_OF_FILE, LZW_ERROR} Read_Return_Code ;

class lzw_decompress
{
    private:

    unsigned short bitsPerPixel;
    short clearCode;
    short eofCode;

    unsigned char codeSize;
    unsigned short nEntries;

    code table[4096];
    unsigned char stack[4096];
    short stackptr;
    short minus1;

    bool first;
    short new_code;
    short old_code;
    unsigned char C;

    bsread * bs;

    void clear(void);
    void addcode(short prev, unsigned char c);
    void buildString(short code);
    
    public:

    lzw_decompress(bsread * bs, unsigned short bitsPerPixel);
    Read_Return_Code read(unsigned char *c);
    ~lzw_decompress(void);
};

#pragma pack(pop)
#endif

