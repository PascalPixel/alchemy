typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

u8 *Func_080150c8(u16, u32, s32, s32, s32);

extern u8 Value_00000392;

s32 Func_080ae99c(s32 a, s32 b, s32 c, s32 flag)
{
    u8 *table = *(u8 **)0x03001f2c;
    u32 offset;
    u8 *object;

    if (flag == 0) {
        offset = (u32)&Value_00000392;
    } else {
        offset = 916;
    }
    object = Func_080150c8(*(u16 *)(table + offset), 0x40000000, a, b, c);
    if (object == 0) {
        return -1;
    }
    object[4] = 0;
    *(u16 *)(object + 12) = 0;
    object[5] = 1;
    return 1;
}
