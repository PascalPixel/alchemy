#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

extern u8 *Data_03001ea0;
extern u8 Data_000000e8[];

void Func_080b0020(void *);
void Func_080b0028(void *);
s32 Func_08002f40(s32);
s32 Func_08003fa4(s32, s32, s32);
void Func_080217a4(void *);

void Func_0801cf48(void)
{
    s32 first_source;
    s32 second_source;
    s32 selection;
    void *base;

    base = Data_03001ea0;
    selection = FIELD(base, u16, 0x574);
    Func_080b0020(base + 0x5A4);
    Func_080b0028(base + 0x5B4);
    Func_080b0028(base + 0x5C4);

    if (selection == 0) {
        first_source = (FIELD(base, u16, 0x57C) & 7) +
            (s32)Data_000000e8;
    } else {
        first_source = (s32)Data_000000e8;
    }
    Func_08003fa4(
        FIELD(FIELD(base, void *, 0x5B4), u8, 14),
        0x100,
        Func_08002f40(first_source));

    if (selection == 1) {
        second_source = (FIELD(base, u16, 0x57C) & 7) +
            (s32)Data_000000e8;
    } else {
        second_source = (s32)Data_000000e8;
    }
    Func_08003fa4(
        FIELD(FIELD(base, void *, 0x5C4), u8, 14),
        0x100,
        Func_08002f40(second_source));

    if (selection > 1) {
        s32 index = selection * 3;
        s32 adjustment = 0x594 + selection;
        s32 object_offset;

        adjustment = ((s8 *)base)[adjustment];
        index += adjustment;
        object_offset = 0x5D4 + index * 4;
        Func_080217a4(*(void **)((u8 *)base + object_offset));
    }
    FIELD(base, u16, 0x57C)++;
}
