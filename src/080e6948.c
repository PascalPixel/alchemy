typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef int bool;
volatile int Func_080cd594(s32);
s32 Func_080cdbc0();
s32 Func_080de2f8(void *, s32, s32, s32, s32 *, s32 *);
void Func_080e6948(void *arg0)
{
  s32 sp8;
  s32 spC;
  void *temp_r5;
  temp_r5 = *((void **) 0x03001EEC);
  *((void **) (((u8 *) temp_r5) + 0x7828)) = arg0;
  Func_080cd594(0);
  Func_080de2f8(arg0, 7, (*((s32 *) (((u8 *) (*((void **) (((u8 *) temp_r5) + 0x7828)))) + 4))) ^ 1, 0, &spC, &sp8);
 do { Func_080cdbc0(); } while (0);
}
