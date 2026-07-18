typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void Func_080f7db4(void) {
    s32 index;
    s32 limit;
    s32 zero;
    u8 *record;
    s32 *slot;

    limit = 0x3FF;
    index = 0;
    zero = 0;
    record = *(u8 **)0x02004C00 + 4;
    do {
        *(s32 *)(record + 4) = index;
        index++;
        *(s32 *)record = zero;
        record += 12;
    } while (index <= limit);
    slot = (s32 *)(*(u8 **)0x02004C00 + 0x3000);
    {
        s32 zero2 = 0;
        for (index = 0xFF; index >= 0; index--) {
            *slot++ = zero2;
        }
    }
}
