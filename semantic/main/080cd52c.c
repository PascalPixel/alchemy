typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

void Func_080d6888(s32, s32, s32, s32, s32, s32);

void Func_080cd52c(void)
{
    u8 *base = *(u8 **)0x03001eec;
    u8 *timer = base + 0x7818;
    s32 index = 0;
    s32 offset = 36;
    u8 value;

    do {
        value = *timer;
        if (value != 0) {
            *timer = value - 1;
            if ((u8)(value - 1) == 0) {
                u8 *table = *(u8 **)(base + 0x7828);

                Func_080d6888(*(s16 *)(table + offset), 0, -1, -1, 0, index);
            }
        }
        index++;
        offset += 2;
        timer++;
    } while (index != 8);
}
