typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern s32 Func_020019be(s32);
extern s32 Func_020019cc(s32);
extern s32 Func_02001a0c(s32);
extern u8 Data_02000240[];
extern s32 Data_0200a518[];
extern s32 Data_0200a410[];
extern s32 Data_0200a338[];
extern s32 Data_0200a218[];

s32 Func_020000b8(void)
{
    u8 *b = Data_02000240;
    s32 *result;

    if (*(s16 *)(b + 0x1c2) == 19)
        return (s32)Data_0200a518;
    if (Func_020019be(0x87a) != 0)
        result = Data_0200a410;
    else if (Func_020019cc(0x815) != 0)
        result = Data_0200a338;
    else
        result = Data_0200a218;
    Func_02001a0c((s32)result);
    return (s32)result;
}
