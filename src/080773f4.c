typedef unsigned char u8;
typedef signed int s32;

void Func_080773f4(u8 *first, u8 *second, s32 count, s32 direction)
{
    s32 index;

    if (direction != 0) {
        index = 0;
        if (index < count) {
            do {
                *second++ = *first++;
                index++;
            } while (index < count);
        }
    } else if (count > 0) {
        index = count;
        do {
            *first++ = *second++;
            index--;
        } while (index != 0);
    }
}
