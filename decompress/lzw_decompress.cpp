#include "logit.hpp"
#include "bsread.hpp"
#include "lzw_decompress.hpp"
#include <stdlib.h>

extern logit l;

lzw_decompress::lzw_decompress(bsread * Bs, unsigned short bitsperpixel)
{
//	l.Enter("lzw_decompress::lzw_decompress");

   minus1 = (short) 0xFFFF;

   bitsPerPixel  = bitsperpixel;
   clearCode     = (short) (1 << bitsPerPixel);
   eofCode       = clearCode + 1;

//   l.PrintX("Clear code = ", clearCode);
//   l.PrintX("eof code = ", eofCode);

   bs = Bs;  /* The bit stream we are to output to. */
   clear();
   stackptr = minus1;
// l.Leave("lzw_decompress::lzw_decompress");
}

void lzw_decompress::clear()
{
//    l.Enter("lzw_decompress::clear");
    unsigned short i;

    for (i=0; i < clearCode; ++i)
    {
	   table[i].prefix = minus1;
	   table[i].c = (unsigned char) i;
    }

    first = true;
    nEntries = clearCode + 2;  /* 1 clear code and 1 eof code. */
    codeSize = bitsPerPixel + 1;

//    l.Print("nEntries = ", nEntries);
//    l.Print("Code size is ", codeSize);

//    l.Leave("lzw_decompress::clear");
}


void lzw_decompress::addcode(short prev, unsigned char X)
{
//	l.Enter("lzw_decompress::addcode");
   if (nEntries > 4096)
   {
   	l.Print("Table overflow!");
      exit(1);
   }

   if (prev > nEntries)
   {
   	l.Print("Forward reference @", prev);
      exit(1);
   }

   table[nEntries].prefix = prev;
   table[nEntries].c = X;

   ++nEntries;

   if ((nEntries+1) > (1 << codeSize))
   {
      if (codeSize < 12)
         ++codeSize;
//      l.Print("Inc codesize to", codeSize);
   }

//   l.Print("nEntries=", nEntries);
//	l.Leave("lzw_decompress::addcode");
}

#include <stdio.h>
void lzw_decompress::buildString(short code)
{
//	char buffer[256];
 //  int i;
//   short save_code;
//   save_code = code;
   while (code != minus1)
   {
      stack[++stackptr] = table[code].c;
      code = table[code].prefix;
   }
}

/*---------------------------------------------------------------\
Read OLD_CODE
output OLD_CODE
While there are still input characters do
   Read NEW_CODE
   if NEW_CODE is in the translation table THEN
     STRING = get translation of NEW_CODE
   ELSE
      STRING =  get translation of OLD_CODE
      STRING = STRING+CHARACTER
   END of IF
   output STRING
   CHARACTER = first character in STRING
   add OLD_CODE + CHARACTER to the translation table
   OLD_CODE = NEW_CODE
END of WHILE
------------------------------------------------------------------*/
Read_Return_Code lzw_decompress::read(unsigned char *c)
{
   bool status;

   if (first)
   {
      status = bs->read(&old_code, codeSize);
      if (status == false)
      {
         return(LZW_BUFFER_EMPTY);
      }
      if (old_code == clearCode)
      {
         clear();
         return(LZW_CLEAR_CODE);
      } else if (old_code == eofCode) {
         return(LZW_END_OF_FILE);
      }
		C = stack[++stackptr] = table[old_code].c;
      first = false;
   }

   if (stackptr == minus1)
   {
      status = bs->read(&new_code, codeSize);
      if (status == false)
      {
         return(LZW_BUFFER_EMPTY);
      }

      if (new_code == clearCode)
      {
         clear();
         return(LZW_CLEAR_CODE);
      } else if (new_code == eofCode) {
         return(LZW_END_OF_FILE);
      }

      if (new_code >= nEntries)
      {
      	stack[++stackptr] = C;
         buildString(old_code);
      }
      else
      {
      	buildString(new_code);
      }

      C = stack[stackptr];

      addcode(old_code, C);
      old_code = new_code;

   }

   if (stackptr != minus1)
   {
      *c = stack[stackptr--];
      return(LZW_CHAR_READ);
   }
//   l.Print("*****lzw_error!");
   return (LZW_ERROR); // Error.
}

lzw_decompress::~lzw_decompress()
{
//   l.Enter("lzw_decompress::~lzw_decompress");
//   l.Leave("lzw_decompress::~lzw_decompress");
}


