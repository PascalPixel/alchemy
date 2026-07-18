typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
volatile short Func_08015e8c(void)
{
  void **temp_r0;
  void **temp_r2;
  void *temp_r3;
  temp_r3 = *((void **) 0x03001E8C);
  temp_r0 = *((void ***) (((u8 *) temp_r3) + 0xD98));
  if (temp_r0 != ((void *) 0))
  {
    ;
    if ((*temp_r0) == ((void *) 0))
    {
      *((s32 *) (((u8 *) temp_r3) + 0xD9C)) = (s32) (temp_r3 + 0xD98);
    }
    *((void ***) (((u8 *) temp_r3) + 0xD98)) = *temp_r0;
    *temp_r0 = (void *) 0;
  }
}

