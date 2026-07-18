typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080030f8(s32);
s32 Func_08009020(void *, s32);
s32 Func_08009100(void *);
void *Func_08077008();
void **Func_080b7dd0(s32);
s32 Func_080b7e60(s32);
void Func_080b8ec4(s32 arg0)
{
  int new_var;
  void *temp_r2;
  void *temp_r5;
  if (((s32) (*((s16 *) (((u8 *) Func_08077008()) + 0x38)))) <= 0)
  {
    temp_r5 = *((void **) (((u8 *) (*Func_080b7dd0(arg0))) + 0x50));
    Func_08009020(temp_r5, 5);
    temp_r2 = *((void **) (((u8 *) temp_r5) + 0x28));
    *((s8 *) (((u8 *) temp_r2) + 5)) = 6;
    new_var = 0xFF;
    *((s8 *) (((u8 *) temp_r2) + 0x16)) = new_var;
    Func_080030f8(4);
    Func_08009100(temp_r5);
    Func_080b7e60(arg0);
  }
}

