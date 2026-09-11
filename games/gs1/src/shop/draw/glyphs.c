#include "types.h"
#include "scene.h"
#include "abi/shop/draw/glyphs.h"

extern u16 gRom[];
extern u8 gRom2[];

/* 4行分の非0バイトを指定配置へ順にコピーする。 */
void Shop_CopyGlyphs(s32 arg0, s32 arg1, u32 arg2)
{
    u8 *src = gRom2 + ((u32)arg0 << 5);
    u8 *dst =
        (u8 *)((u32)arg1 + gRom[arg2] + 2);
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
