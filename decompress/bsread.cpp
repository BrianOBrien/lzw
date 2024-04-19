#include "bsread.hpp"
#include "logit.hpp"
#include <stdlib.h>

//extern logit l;

bsread::bsread()
{
//	l.Enter("bsread::bsread");

   accume = 0;
   bitsInAccume = 0;

   bytesInBuffer = 0;
   buffer = NULL;

//	l.Leave("bsread::bsread");
}

void bsread::setbuff(unsigned char *s, unsigned short si)
{
//	l.Enter("bsread::setbuff");

//   if (bytesInBuffer != 0)
//   	l.Print("Warning buffer wasn't empty!");

//   if (bitsInAccume != 0)
//   	l.Print("Bit stream crossing buffer boundary.");

   // The accumulator is preserved so that bit
   // steams can cross buffer boundaries.

	buffer = s;
   bytesInBuffer = si;

//	l.Leave("bsread::setbuff");
}

bool bsread::read(short * codeptr, unsigned short nbits)
{
//	l.Enter("bsread::read");
   unsigned long mask, tmp;
   unsigned long code;

//   l.Print("Read nbits = ", nbits);
//   l.Print("bytesInBuffer = ", bytesInBuffer);
//   l.Print("bitsInAccume = ", bitsInAccume);

   // Fill the accumulator if possible or necessary.
   while ((bytesInBuffer) && (bitsInAccume < nbits))
   {
     	tmp = (unsigned long) *buffer;
      ++buffer;
      bytesInBuffer--;

      tmp <<= bitsInAccume;
      accume |= tmp;
      bitsInAccume += 8;
   }

//   l.Print("bytesInBuffer = ", bytesInBuffer);
//   l.Print("bitsInAccume = ", bitsInAccume);

   // At this point there should be enough
   // bits in the accumulator to satisfy the
   // read.
   if (bitsInAccume < nbits)
   {
//      l.Print("bitsInAccume = ", bitsInAccume);
//      l.Print("nbits = ", nbits);
//      l.Print("accume = ", accume);
//   	l.Print("bsread Buffer empty.");
//   	l.Leave("bsread::read");
      return(false);
   }

   mask = 0xFFFFFFFF;
   mask <<= nbits;
   mask = ~mask;

//   l.PrintB("Mask   = ", mask);
//   l.PrintB("accume = ", accume);

   code = accume;
   code &= mask;

   (unsigned short) * (unsigned short *) codeptr = (unsigned short) code;

   accume >>= nbits;
   bitsInAccume -= nbits;

//   if (code == 0x101)
//   {
//      l.Print("Returning eof");
//      l.Print("bytesInBuffer = ", bytesInBuffer);
//      l.Print("bitsInAccume = ", bitsInAccume);
//      l.PrintX("Accume = ", accume);
//   }
//   l.PrintX("code = ", code);

//  	l.Leave("bsread::read");

   return(true);
}

bsread::~bsread(void)
{
//   l.Enter("bsread::~bsread");
//   l.Leave("bsread::~bsread");
}

