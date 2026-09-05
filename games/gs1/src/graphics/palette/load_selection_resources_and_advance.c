#include "types.h"
#include "resource.h"

#define GraphicsPalette_LoadSelectionResourcesAndAdvance Func_0801cf48

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *Data_03001ea0;
extern u8 Data_000000e8[];

void Func_080b0020(void *);
void Func_080b0028(void *);
s32 Resource_CopyData(s32, s32, s32);
void Func_080217a4(void *);

void GraphicsPalette_LoadSelectionResourcesAndAdvance(void)
{
    s32 src0;
    s32 src1;
    s32 sel;
    void *base;

    base = Data_03001ea0;
    sel = FIELD(base, u16, 0x574);
    Func_080b0020(base + 0x5A4);
    Func_080b0028(base + 0x5B4);
    Func_080b0028(base + 0x5C4);

    if (sel == 0) {
        src0 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)Data_000000e8;
    } else {
        src0 = (s32)Data_000000e8;
    }
    Resource_CopyData(
        FIELD(FIELD(base, void *, 0x5B4), u8, 14),
        0x100,
        (s32)GetResource(src0));

    if (sel == 1) {
        src1 = (FIELD(base, u16, 0x57C) & 7) +
            (s32)Data_000000e8;
    } else {
        src1 = (s32)Data_000000e8;
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
        Func_080217a4(*(void **)((u8 *)base + off));
    }
    FIELD(base, u16, 0x57C)++;
}
