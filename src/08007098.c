typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

extern s32 Func_080072f0(s32 mode, u8 *destination, u8 value, s32 status);

u16 Func_08007098(u16 sector)
{
    u8 *info = (u8 *)0x08007C10;
    u8 *cursor;
    u16 saved;
    s32 count;
    u16 result;

    cursor = (u8 *)(0x0E000000 + (sector << info[28]));
    saved = *(volatile u16 *)0x04000208;
    *(volatile u16 *)0x04000208 = 0;
    *(volatile u8 *)0x0E005555 = 0xAA;
    *(volatile u8 *)0x0E002AAA = 0x55;
    *(volatile u8 *)0x0E005555 = 0xA0;
    count = *(s32 *)(info + 24);
    while (count != 0) {
        *cursor = 0xFF;
        cursor++;
        count--;
    }
    cursor--;
    *(volatile u16 *)0x04000208 = saved;
    result = Func_080072f0(1, cursor, 0xFF, *(s32 *)0x02004C00);
    if (result != 0) {
        result = (result & 0xFF00) | 2;
    }
    return result;
}
