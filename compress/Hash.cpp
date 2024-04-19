#include "hash.hpp"

hash::hash(void)
{
}

hash::~hash(void)
{
}

void hash::add(unsigned long hashv, unsigned short index)
{
    short current;

    if (tofirst[hashv] == -1)
    {
       tofirst[hashv]=index;
    } else {
       current = tofirst[hashv];
       while (tonext[current] != -1)
	       current = tonext[current];
       tonext[current]=index;
    }
}

void hash::init()
{
    int i;

    for (i=0; i < 4096; ++i)
       tofirst[i] = tonext[i] = -1;
}

short hash::search(unsigned int z)
{
   return(tofirst[z]);
}

short hash::getNext(unsigned int z)
{
   return(tonext[z]);
}

unsigned long hash::value(short p, unsigned char c)
{
   return((((unsigned long)p<<8UL)|(unsigned long) c)%4093UL);
}


