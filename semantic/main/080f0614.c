typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

u32 Func_080f07f0(u32, u32, u32);

void Func_080f0614(void)
{
    s16 *busy = (s16 *)0x02004c04;

    if (*busy == 0) {
        s16 *currentAddress = (s16 *)0x02004c00;
        s16 *previousAddress = (s16 *)0x02004c08;
        s32 current = *currentAddress;
        u16 saved = *(u16 *)currentAddress;
        s32 rounded = current;
        s32 previous;
        s32 index;

        if (rounded < 0)
            rounded += 7;
        index = rounded >> 3;
        previous = *previousAddress;
        if (previous < 0)
            previous += 7;
        if (index != (previous >> 3)) {
            u32 argument = ((index + 16) & 31) * 24;
            u32 *table = (u32 *)0x080f1220;
            *previousAddress = saved;
            *busy = Func_080f07f0(table[index], argument, 1);
        }
    }
}
