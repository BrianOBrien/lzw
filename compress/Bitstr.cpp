#include "logit.hpp"
#include "bitstr.hpp"

#define MAXBYTES (255)

void bitstream::flush(int closing)
{
    unsigned char nbytes;

    nbytes = index;

    if (closing)
    {
        if (bitsInAccume)
        {
            buffer[nbytes] = (unsigned char) accume;
            bitsInAccume = 0;
            accume = 0;

            ++nbytes;
        }
    }

    if (nbytes)
    {
        f->write(&nbytes,    1);
        f->write(buffer, nbytes);
    }
    index = 0;
}

bitstream::bitstream(fstream * fstr)
{
    f            = fstr;
    index        = 0;
    accume       = 0;
    bitsInAccume = 0;
}

void bitstream::write(unsigned short data, unsigned short nbits)
{
    unsigned long ldata;

    ldata = (unsigned long) data;
    ldata <<= bitsInAccume;

    accume |= ldata;
    bitsInAccume += nbits;

    while (bitsInAccume >= 8)
    {
       buffer[index] = (unsigned char) accume;

       accume >>= 8;
       bitsInAccume -= 8;
       index++;

       if (index == MAXBYTES)
	       flush(0);
    }
}

bitstream::~bitstream(void)
{
    unsigned char z=0;

    flush(1);
    f->write(&z, 1);
}

