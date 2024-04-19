#ifndef hash_h
#define hash_h

#pragma pack(push, 1)
class hash
{
   private:
      short tofirst[4096];
      short tonext[4096];

   public:

      hash(void);
      ~hash(void);
      void init(void);
      void add(unsigned long hashv, unsigned short value);
      short search(unsigned int z);
      short getNext(unsigned int z);
      unsigned long value(short p, unsigned char c);
};

#pragma pack(pop)
#endif
