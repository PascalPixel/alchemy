typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;

extern u8 *Data_03001ebc;
extern void Func_02002038(s32);

void Func_020010a0(void)
{
    Func_02002038(*(s16 *)(Data_03001ebc + 0x16c) - 50);
}
