typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

s32 Func_0800bacc(u8 *arg0, s32 arg1)
{
    u8 initial = arg0[0x27];
    void **cursor;
    s32 count;

    if (initial != 0) {
        arg1 <<= 4;
        cursor = (void **)(arg0 + 0x28);
        count = initial;
        do {
            u8 *item = *cursor++;
            if (item != 0 && *(s32 *)(item + 0xc) != 0)
                *(s16 *)(item + 2) = arg1;
            count--;
        } while (count != 0);
    }
    return 0;
}
