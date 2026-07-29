typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_020069a6(s32);
extern s32 Func_020069b4(s32);
extern u8 Data_02000240[];
extern s32 Data_0200ee48[];
extern s32 Data_0200ecc8[];
extern s32 Data_0200eab8[];
extern s32 Data_0200e8a8[];

s32 Func_020009f0(void)
{
    u8 *b = Data_02000240;
    if (*(s16 *)(b + 0x1c2) == 16)
        return (s32)Data_0200ee48;
    if (Func_020069a6(0x87a) != 0)
        return (s32)Data_0200ecc8;
    if (Func_020069b4(0x815) != 0)
        return (s32)Data_0200eab8;
    return (s32)Data_0200e8a8;
}
