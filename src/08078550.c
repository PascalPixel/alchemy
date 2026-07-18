typedef signed short s16;
typedef signed int s32;

s32 Func_080796c4(s16 *);
s32 Func_080784d8(s32);

s32 Func_08078550(void)
{
    s16 owners[10];
    s32 count = Func_080796c4(owners);
    s32 result = 0;
    s16 *owner = owners;

    if (result < count) {
        s32 remaining = count;

        do {
            result = result - Func_080784d8(*owner++) + 15;
            remaining--;
        } while (remaining != 0);
    }
    return result;
}
