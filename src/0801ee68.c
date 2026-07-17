typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

void Func_0801ee68(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *destination = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *destination = value;
                    destination++;
                } while (column < width);
            }
            row++;
            destination += 32 - width;
        } while (row < height);
    }
}
