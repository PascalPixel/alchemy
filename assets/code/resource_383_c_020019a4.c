typedef unsigned char u8;
typedef signed int s32;

extern u8 *Data_03001ebc;

extern void Func_02006736(s32, s32, s32);
extern void Func_0200676c(s32, s32, s32);
extern void Func_020068f8(s32);
extern void Func_02006846(s32);

void Func_020019a4(void)
{
    Func_02006736(0, 0x8000, 0x4000);
    Func_0200676c(0, 728, 408);
    *(s32 *)(Data_03001ebc + 456) = 16;
    Func_020068f8(123);
    Func_02006846(15);
}
