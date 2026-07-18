typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

extern u8 Data_03001b10[];
void Func_08003f04(s32);

s32 Func_08003f78(u32 arg0)
{
    u16 *entry = (u16 *)(Data_03001b10 + arg0 * 4);

    if (arg0 > 95)
        return -1;
    if (*entry > 16) {
        s32 value;

        Func_08003f04(arg0);
        value = 1;
        *entry = value;
    }
    return 0;
}
