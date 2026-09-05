#include "types.h"

void UiWindow_FillScreenBlockRect(s32 unused0, s32 unused1, u32 width, u32 height,
                   s32 value)
{
    u32 row = 0;
    s16 *dst = (s16 *)0x06002000;

    if (row < height) {
        do {
            u32 column = 0;
            if (column < width) {
                do {
                    column++;
                    *dst = value;
                    dst++;
                } while (column < width);
            }
            row++;
            dst += 32 - width;
        } while (row < height);
    }
}
