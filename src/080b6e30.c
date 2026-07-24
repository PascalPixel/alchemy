typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

extern s32 Func_08009270(s32, s32, s32, s32);

s32 Func_080b6e30(s32 value)
{
    u8 *base;
    s32 offset;
    s32 index;
    s16 item;

    base = *(u8 **)0x03001e74;
    index = 0;
    offset = 4;
    do {
        item = *(s16 *)(offset + (u32)base);
        if (item == value) {
            Func_08009270(index, 0, 0, 0);
            *(s16 *)(offset + (u32)base) = 0;
        }
        offset += 2;
        index++;
    } while (index <= 5);
}
