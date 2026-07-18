typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
u32 Func_080fb2cc(s32, s32, u16);
void Func_080f950c(s16 arg0)
{
  s16 temp_r5;
  s16 *new_var;
  temp_r5 = arg0;
  Func_080fb2cc(0x02004290, 0xFF, (u16) arg0);
  *((s16 *) 0x02003034) = temp_r5;
 dummy_label_303982: ;
  if (temp_r5)
  {
    *(new_var = (s16 *) 0x02003008) = temp_r5;
  }
  else
  {
    *(new_var = (s16 *) 0x02003008) = temp_r5;
  }
}
