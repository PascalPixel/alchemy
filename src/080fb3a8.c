typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_080fb3a8(u8 *object, u32 selected, u32 value)
{
    u16 selectedBits = selected;
    u8 storedValue = value;

    if (*(u32 *)(object + 52) == 0x68736D53) {
        s32 count;
        u8 *entry;
        u32 mask;

        *(u32 *)(object + 52) = *(u32 *)(object + 52) + 1;
        count = object[8];
        entry = *(u8 **)(object + 44);
        mask = 1;

        while (count > 0) {
            if ((selectedBits & mask) != 0 && (entry[0] & 0x80) != 0) {
                entry[21] = storedValue;
                entry[0] |= 3;
            }
            count--;
            entry += 80;
            mask <<= 1;
        }
        *(u32 *)(object + 52) = 0x68736D53;
    }
}
