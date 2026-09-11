#include "types.h"
#include "scene.h"
#include "global_cells.h"

/* shop/sel/fill.c */
struct Record_080b06c0 {
    u8 filler0[4];
    u8 values[21];
};

extern u16 gRom[];

void Shop_FillSelector(s32 count, s32 selector, u8 *base)
{
    u32 shifted = selector << 4;
    u16 *offset;

    selector = shifted + 1;

    if (count > 0) {
        offset = gRom;
        do {
            struct Record_080b06c0 *record = (struct Record_080b06c0 *)(base + *offset++);
            record->values[0] = selector;
            record->values[4] = selector;
            record->values[8] = selector;
            record->values[12] = selector;
            record->values[16] = selector;
            record->values[20] = selector;
            count--;
        } while (count != 0);
    }
}

/* shop/draw/glyphs.c */
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

/* shop/effect/init.c */
void Shop_InitEffect(void)
{
    Effect_Apply(*(s32 *)ADDR_03001EBC + 0x236, 1);
    Effect_Check(0x10);
}
