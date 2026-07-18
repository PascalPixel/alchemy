typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
s32 Func_080022ec(s32, s32);
void Func_080925e0(void *arg0)
{
  s32 temp_r3;
  s32 temp_r3_2;
  s32 temp_r3_3;
  s32 temp_r5;
  s32 temp_r7;
  s32 var_r3;
  temp_r5 = *((s32 *) (((u8 *) arg0) + 0x30));
  temp_r3 = (*((s32 *) (((u8 *) arg0) + 8))) + temp_r5;
  *((s32 *) (((u8 *) arg0) + 8)) = temp_r3;
  *((s32 *) (((u8 *) arg0) + 0x38)) = temp_r3;
  temp_r7 = *((s32 *) (((u8 *) arg0) + 0x34));
  temp_r3_2 = (*((s32 *) (((u8 *) arg0) + 0x10))) + temp_r7;
  *((s32 *) (((u8 *) arg0) + 0x10)) = temp_r3_2;
  *((s32 *) (((u8 *) arg0) + 0x40)) = temp_r3_2;
  temp_r3_3 = (*((s32 *) (((u8 *) arg0) + 0xC))) + 0x400;
  *((s32 *) (((u8 *) arg0) + 0xC)) = temp_r3_3;
  *((s32 *) (((u8 *) arg0) + 0x3C)) = temp_r3_3;
  *((s32 *) (((u8 *) arg0) + 0x30)) = (s32) (temp_r5 - Func_080022ec(temp_r5, 0x12));
 do { var_r3 = temp_r7; if (temp_r7 < 0) { var_r3 += 0xF; } *((s32 *) (((u8 *) arg0) + 0x34)) = (s32) (temp_r7 - (var_r3 >> 4)); } while (0);
}
