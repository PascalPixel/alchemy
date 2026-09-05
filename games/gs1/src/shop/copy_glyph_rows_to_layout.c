#include "types.h"

#define Shop_CopyGlyphRowsToLayout Func_080b06ec

extern u16 Data_080b413c[];
extern u8 Data_080b3d40[];

/* 4行分の非0バイトを指定配置へ順にコピーする。 */
void Shop_CopyGlyphRowsToLayout(s32 arg0, s32 arg1, u32 arg2) {
    u8 *src = Data_080b3d40 + ((u32)arg0 << 5);
    u8 *dst =
        (u8 *)((u32)arg1 + Data_080b413c[arg2] + 2);
    s32 count = 3;

    do {
        if (*src != 0) {
            dst[0] = *src++;
            if (*src != 0) {
                dst[1] = *src++;
                if (*src != 0) {
                    dst[30] = *src++;
                    if (*src != 0) dst[31] = *src++;
                }
            }
        }
        dst += 4;
        count--;
    } while (count >= 0);
}
