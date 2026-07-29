typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16; typedef signed int s32; typedef unsigned int u32;

void Func_02000104(s32 *o)
{
    u16 *q;
    o[2] = o[2] + o[17];
    o[3] = o[3] + o[18];
    o[4] = o[4] + o[19];
    o[6] = o[6] + o[12];
    o[7] = o[7] + o[13];
    q = (u16 *)o[20];
    q[15] = q[15] + *(u16 *)((u8 *)o + 0x64);
}
