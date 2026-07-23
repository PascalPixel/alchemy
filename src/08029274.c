typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

void Func_08029274(u32 value, u32 count, u8 *output) {
    u8 digits[8];
    u8 *cursor;
    u32 index;
    u32 mask;

    if (count > 5)
        count = 5;

    index = 0;
    if (count != 0) {
        mask = 15;
        cursor = digits;
        do {
            u32 digit = value & mask;
            if (digit <= 9)
                digit += '0';
            else
                digit += 'A' - 10;
            *cursor = digit;
            index++;
            value >>= 4;
            cursor++;
        } while (index != count);
    }

    for (index = count - 1; (s32)index >= 0; index--)
        *output++ = digits[index];
}
