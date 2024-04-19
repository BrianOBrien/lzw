#ifndef BITSTR_HPP
#define BITSTR_HPP

#include <fstream.h>

#pragma pack(push, 1)

class bitstream
{
   private:

	unsigned char buffer[255];
	unsigned char index;

	unsigned char bitsInAccume;
	unsigned long accume;

   fstream * f;

   void flush(int closing);

   public:
   bitstream(fstream * fstr);
   void write(unsigned short data, unsigned short nbits);
   ~bitstream(void);
};


#pragma pack(pop)
#endif

