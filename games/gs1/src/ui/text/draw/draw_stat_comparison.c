#include "types.h"
#include "scene.h"
#include "abi/ui/text/draw/draw_stat_comparison.h"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 gVal[];

void UiText_DrawStatComparison(s32 alt, s32 base, s32 work)
{
    u32 i;
    s32 tmp2;
    s32 p;
    s32 tmp;
    s32 rec;

    p = alt;
    Ui_Run(0xb1c, work, 0, 32);
    Ui_Run2(*(u16 *)(base + 60), 3, work, 16, 40);
    if (*(u16 *)(p + 60) != *(u16 *)(base + 60)) {
        Ui_Run2(*(u16 *)(p + 60), 3, work, 64, 40);
        if (*(u16 *)(p + 60) > *(u16 *)(base + 60)) {
            Ui_Run3(work, 44, 36, 0);
        } else {
            Ui_Run3(work, 44, 36, 1);
        }
    }
    Ui_Run(0xb1d, work, 0, 48);
    Ui_Run2(*(u16 *)(base + 62), 3, work, 16, 56);
    if (*(u16 *)(p + 62) != *(u16 *)(base + 62)) {
        Ui_Run2(*(u16 *)(p + 62), 3, work, 64, 56);
        if (*(u16 *)(p + 62) > *(u16 *)(base + 62)) {
            Ui_Run3(work, 44, 52, 0);
        } else {
            Ui_Run3(work, 44, 52, 1);
        }
    }
    Ui_Run4((s32)gVal, work, 0, 64);
    Ui_Run2(*(u16 *)(base + 64), 3, work, 16, 72);
    if (*(u16 *)(p + 64) != *(u16 *)(base + 64)) {
        Ui_Run2(*(u16 *)(p + 64), 3, work, 64, 72);
        if (*(u16 *)(p + 64) > *(u16 *)(base + 64)) {
            Ui_Run3(work, 44, 68, 0);
        } else {
            Ui_Run3(work, 44, 68, 1);
        }
    }
    tmp = *(u16 *)(base + 64);
    tmp2 = p + 64;
}
