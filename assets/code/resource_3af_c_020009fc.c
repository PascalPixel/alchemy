typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef signed char s8;
typedef unsigned char u8;

extern s32 Func_02004cce(s32);
extern s32 Func_02004cde(s32);
extern s32 Func_02004cec(s32);

extern u8 Value_0000093e;
extern u8 Value_00000928;

extern u8 Data_0200d9d0[];
extern u8 Data_0200da54[];
extern u8 Data_0200d958[];
extern u8 Data_0200d778[];

u8 *Func_020009fc(void)
{
    if (Func_02004cce((s32)&Value_0000093e))
        return Data_0200d9d0;
    if (Func_02004cde(0x8A0))
        return Data_0200da54;
    if (Func_02004cec((s32)&Value_00000928))
        return Data_0200d958;
    return Data_0200d778;
}
