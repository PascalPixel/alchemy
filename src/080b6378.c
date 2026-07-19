typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

extern u8 *Data_03001e74;
s32 Func_080b6a60(u16 *);

s32 Func_080b6378(void)
{
    u16 values[8];
    u8 *base = Data_03001e74;
    s32 count = Func_080b6a60(values);
    s32 index;
    s32 offset;

    for (index = 0; index < count; index++) {
        offset = values[index];
        offset += 72;
        base[offset] = (s8)(index - 128);
    }
}
