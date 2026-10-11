// near miss: differ 22 (listing 63 instructions, draft 63); the native builds index * 2 in r10 before the three channels are joined, and stores from r0
#include "../../../../games/tla/src/field/overlay_670/ov670.h"

extern u8 Data_0200635c[];
extern s16 Data_02007948[];
extern struct Ov670Object *Data_02007950;

void Func_02001318(void);
s32 Func_02002b08(s32 value);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);

struct Ov670Work90 {
    u8 unknown_00[0x18];
    struct Ov670Object *leader;
};

void Func_02002b18(s32 index, s32 entry, s32 delta)
{
    u16 color = Data_02007948[entry];
    s16 r = color & 0x1f;
    s16 g = (color >> 5) & 0x1f;
    s16 b = (color >> 10) & 0x1f;

    r += delta;
    g += delta >> 1;
    b += delta >> 2;

    r = Func_02002b08(r);
    g = Func_02002b08(g);
    b = Func_02002b08(b);
    r |= (b << 10) | (g << 5);
    ((u16 *)0x05000000)[index] = r;
}
