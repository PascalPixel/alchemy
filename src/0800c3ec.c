typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
void Func_0800c3ec(void *arg0, unsigned int arg1)
{
  int new_var;
  if ((arg0 != ((void *) 0)) && ((0xF & (*((u8 *) (((u8 *) arg0) + 0x54)))) == 1))
  {
    new_var = 0x1E;
    *((s16 *) (((u8 *) (*((void **) (((u8 *) arg0) + 0x50)))) + new_var)) = arg1;
  }
}

