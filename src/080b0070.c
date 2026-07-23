typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;

s32 Func_08077238(s32);
s32 Func_08077240(s32, s32);

extern void *Data_03001f2c;

s32 Func_080b0070(void)
{
    u8 *state;
    s16 *output;
    s32 index;
    s32 count;
    s32 offset;
    s8 mode;

    state = Data_03001f2c;
    count = 0;
    index = 0;
    output = (s16 *)(state + 0x26c);
    do {
        mode = *(s8 *)(state + 0x3a9);
        if (mode == Func_08077238(index) &&
            Func_08077240(index, 0) != 0) {
            *output = index;
            count++;
            output++;
        }
        index++;
    } while (index <= 0x1ff);
    offset = count << 1;
    offset += 0x26c;
    *(u16 *)(state + offset) = 0;
    *(u8 *)(state + 0x3a6) = count;
    return count;
}
