#ifndef BSREAD_HPP
#define BSREAD_HPP

class bsread
{
private:

   unsigned long accume;
   unsigned short bitsInAccume;

   unsigned short bytesInBuffer;
   unsigned char *buffer;

public:

	bsread();
   void setbuff(unsigned char *s, unsigned short si);
	bool read(short *codeptr, unsigned short nbits);
   ~bsread();
};


#endif

