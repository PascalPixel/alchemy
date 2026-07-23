typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;

void Func_080fb410(u8 *entry);

void Func_080fb4a4(u8 *object, u32 selected, u32 value)
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

        if (count > 0) {
            u8 testValue = storedValue;

            do {
                if ((selectedBits & mask) != 0 && (entry[0] & 0x80) != 0) {
                    entry[25] = storedValue;
                    if (testValue == 0) {
                        Func_080fb410(entry);
                    }
                }
                count--;
                entry += 80;
                mask <<= 1;
            } while (count > 0);
        }

        *(u32 *)(object + 52) = 0x68736D53;
    }
}
