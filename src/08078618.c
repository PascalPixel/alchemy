typedef signed short s16;
typedef signed int s32;

s32 Func_080796c4(s16 *);
s32 Func_08078588(s32, s32);

s32 Func_08078618(s32 target)
{
    s16 owners[10];
    s32 count;
    s32 index;
    s16 *owner;

    count = Func_080796c4(owners);
    owner = owners;
    index = 0;
    if (index < count) {
        do {
            s16 value = *owner++;

            if (Func_08078588(value, target) >= 0)
                return value;
            index++;
        } while (index < count);
    }
    return -1;
}
