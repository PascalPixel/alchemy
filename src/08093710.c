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
void *Func_080048f4(s32, s32);
void Func_08093710(void)
{
  s32 var_r5;
  void *temp_r6;
  u8 *new_var;
  temp_r6 = *((void **) 0x03001E70);
  if ((*((s16 *) (((u8 *) Func_080048f4(0x1B, 0xCCC)) + 0x19E))) == 3)
  {
    var_r5 = 0;
    if ((*((s16 *) (((u8 *) temp_r6) + 0x358))) != 0)
    {
 do { loop_3: Func_080030f8(1); var_r5 += 1; if (var_r5 <= 0x12B) { if ((*((s16 *) ((new_var = (u8 *) temp_r6) + 0x358))) != 0) { goto loop_3; } } } while (0);
    }
  }
}
