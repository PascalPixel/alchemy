typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

s32 Func_08004458(void);
void Func_080030f8(u32);
s32 Func_080b6b40(s32, s16 *);
void Func_080b8064(s32);
void Func_080151c8(s32);

s32 Func_080b8824(void)
{
    s16 values[14];
    s32 count;
    s32 index;

    if (((u32)(Func_08004458() << 4) >> 16) != 0) {
        count = Func_080b6b40(1, values);
        index = 0;
        if (count != 0) {
            do {
                Func_080b8064(values[index]);
                index++;
                Func_080030f8(8);
            } while (index != count);
        }
        Func_080030f8(22);
        return 1;
    }
    Func_080151c8(0x844);
    return 0;
}
