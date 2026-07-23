typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;

void Func_080fb334(u8 *state, u32 maskArg, u32 valueArg)
{
    u16 mask = maskArg;
    u16 value = valueArg;
    u32 current = *(u32 *)(state + 52);

    if (current == 0x68736d53) {
        s32 count;
        u8 *entry;
        u32 bit;

        *(u32 *)(state + 52) = current + 1;
        count = state[8];
        entry = *(u8 **)(state + 44);
        bit = 1;
        if (count > 0) {
            do {
                if ((mask & bit) != 0 && (entry[0] & 0x80) != 0) {
                    entry[11] = (s8)((s16)value >> 8);
                    entry[13] = value;
                    entry[0] |= 12;
                }
                count--;
                entry += 80;
                bit <<= 1;
            } while (count > 0);
        }
        *(u32 *)(state + 52) = 0x68736d53;
    }
}
