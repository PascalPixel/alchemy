typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

u8 *Func_08077008(s32);
s32 Func_080b6b40(s32, s16 *);
u32 Func_08004458(void);

s32 Func_080b8f08(u8 *arg0)
{
    s16 values[14];
    s32 index;
    s32 count;

    index = *(s16 *)(arg0 + 0xA);
    if (*(s16 *)(Func_08077008(index) + 0x38) != 0) {
        return index;
    }

    if (index > 0x7F) {
        count = Func_080b6b40(2, values);
    } else {
        count = Func_080b6b40(1, values);
    }

    if (count == 0) {
        return 0x100;
    }

    return values[(u32)(Func_08004458() * count) >> 0x10];
}
