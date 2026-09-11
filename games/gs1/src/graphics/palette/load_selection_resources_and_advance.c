#include "types.h"
#include "scene.h"
#include "abi/graphics/palette/load_selection_resources_and_advance.h"
#include "resource.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *gIw;
extern u8 gVal[];

s32 Resource_CopyData(s32, s32, s32);

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    void *base;

    base = gIw;
    sel = FIELD(base, u16, 0x574);
    Sys_Do(base + 0x5A4);
    Sys_Do2(base + 0x5B4);
    Sys_Do2(base + 0x5C4);

    if (sel == 0) {
        src0 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)gVal;
    } else {
        src0 = (s32)gVal;
    }
    Resource_CopyData(
        FIELD(FIELD(base, void *, 0x5B4), u8, 14),
        0x100,
        (s32)GetResource(src0));

    if (sel == 1) {
        src1 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)gVal;
    } else {
        src1 = (s32)gVal;
    }
    Resource_CopyData(
        FIELD(FIELD(base, void *, 0x5C4), u8, 14),
        0x100,
        (s32)GetResource(src1));

    if (sel > 1) {
        s32 idx = sel * 3;
        s32 adj = 0x594 + sel;
        s32 off;

        adj = ((s8 *)base)[adj];
        idx += adj;
        off = 0x5D4 + idx * 4;
        Sys_Do3(*(void **)((u8 *)base + off));
    }
    FIELD(base, u16, 0x57C)++;
}
