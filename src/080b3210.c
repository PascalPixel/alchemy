typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

extern u8 *Data_03001f2c;
extern s8 Data_080b4ab6[];

u8 *Func_08077008(s32);

s32 Func_080b3210(s32 arg0)
{
    u8 *global = Data_03001f2c;
    u8 *base;
    s32 active = 0;
    s32 factor = Data_080b4ab6[arg0];
    s32 index = 0;
    s32 offset;

    if (active < *(s8 *)(global + 0x3A7)) {
        base = global + 2;
        offset = 0x36C;
        do {
            if (*(s16 *)(Func_08077008(
                    *(s16 *)(base + offset)) + 56) != 0)
                active++;
            index++;
            offset += 2;
        } while (index < *(s8 *)(global + 0x3A7));
    }

    return factor * active;
}
