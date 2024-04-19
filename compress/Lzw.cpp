#include "hash.hpp"
#include "bitstr.hpp"
#include "lzw.hpp"

void lzw::clear()
{
    unsigned short i;

    hasht->init();

    for (i=0; i < clearCode; ++i)
    {
	   table[i].prefix = -1;
	   table[i].c = (unsigned char) i;
    }

    nEntries = clearCode + 2;  /* 1 clear code and 1 eof code. */
    codeSize = bitsPerPixel + 1;
}

void lzw::put(unsigned short code)
{
    if (nEntries > (1 << codeSize))
    	++codeSize;
    bs->write(code, codeSize);
}

short lzw::isInTable(void)
{
    short index;

    if (current.prefix != -1)
    {
       hashvalue = hasht->value(current.prefix, current.c);

	    index=hasht->search(hashvalue);

	    while (index != -1)
	    {
	       if (current.prefix == table[index].prefix)
	       {
    		    if (current.c == table[index].c)
	    	    {
		            break;
		       }
	       }
	       index=hasht->getNext(index);
	    }
    } else {
	    index = current.c;
    }
    return(index);
}

/*-----------------------------------\
|    set P = NIL                     |
|    loop                            |
|     read a character c             |
|                                    |
|     if Pc exists in the dictionary |
|        P = Pc                      |
|     else                           |
|        output the code for P       |
|        add Pc to the string table  |
|        P = c                       |
|     endif                          |
|   endloop                          |
\-----------------------------------*/
void lzw::write(unsigned char c)
{
    short index;

    current.c = c;

    index=isInTable();

    if (index != -1)
    {
    	current.prefix = index;
    }
    else
    {
	    put(current.prefix);

       if (nEntries == 4096)
       {
	       put(clearCode);
          clear();
	    }
	    else
	    {
	       table[nEntries].prefix = current.prefix;
	       table[nEntries].c      = current.c;

	       hasht->add(hashvalue, nEntries);

	       nEntries++;
	    }
	    current.prefix = c;
    }
}

lzw::lzw(bitstream * Bs, unsigned short bitsperpixel)
{
    bitsPerPixel  = bitsperpixel;
    clearCode     = 1 << bitsPerPixel;
    eofCode       = clearCode + 1;

    bs = Bs;  /* The bit stream we are to output to. */
    hasht = new hash();
    clear();

    current.prefix = -1;

    put(clearCode);  /* Output a clear code. */

}

lzw::~lzw()
{
    delete hasht;

    put(current.prefix); /* Flush the current.   */
    put(eofCode);        /* Output the end code. */
}

