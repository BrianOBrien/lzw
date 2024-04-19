#ifndef lzw_hpp
#define lzw_hpp

#include "bitstr.hpp"
#include "hash.hpp"

#pragma pack(push, 1)

typedef struct
{
    short prefix;
    unsigned char  c;
} code;

class lzw
{
    private:

    unsigned short bitsPerPixel;
    unsigned short clearCode;
    unsigned short eofCode;

    unsigned char codeSize;
    unsigned short nEntries;

    hash * hasht;

    code table[4096];
    code current;
    code previous;

    unsigned long hashvalue;

    bitstream * bs;

    void clear(void);
    void put(unsigned short code);
    short isInTable(void);

    public:

    lzw(bitstream * bs, unsigned short bitsPerPixel);
    void write(unsigned char c);
    ~lzw(void);
};

#pragma pack(pop)
#endif

