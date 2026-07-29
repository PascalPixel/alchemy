typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern s32 Func_020044e4(s32);
extern s32 Func_020044f0(s32);
extern u8 Data_020081c5[];

s32 Func_02000250(u8 *o)
{
    u8 *p;
    s32 m;

    if (Func_020044e4(0x30) != 0) {
        return 0;
    }
    if (Func_020044f0(0x16E) != 0) {
        return 0;
    }
    *(s32 *)(o + 0x6C) = (s32)Data_020081c5;
    p = o + 0x55;
    *p = 0;
    p += 0xF;
    *(u16 *)p = 0;
    p += 2;
    *(u16 *)p = 0;
    m = 0x8000;
    *(s32 *)(o + 0x18) = m;
    *(s32 *)(o + 0x1C) = m;
    return 0;
}
