// near miss: differ 36 (listing 65 instructions, draft 63); as Func_02002b18, and the red channel's + 4 is scheduled apart
#include "../../../../games/tla/src/scene/kibombo/kibombo.h"

extern u8 gData_0200635c[];
extern s16 gData_02007948[];
extern struct KibomboObject *gData_02007950;

void Func_02001318(void);
s32 Func_02002b08(s32 value);
void Func_02002b18(s32 index, s32 entry, s32 delta);
void Func_02002ba4(s32 index, s32 entry, s32 delta);

struct KibomboWork90 {
    u8 unk00[0x18];
    struct KibomboObject *leader;
};

void Func_02002ba4(s32 index, s32 entry, s32 delta)
{
    s16 color = gData_02007948[entry];
    s16 r = (color & 0x1f) + delta + 4;
    s16 g;
    s16 b;

    delta -= 4;
    g = (((u16)color >> 5) & 0x1f) + (delta >> 1);
    b = (((u16)color >> 10) & 0x1f) + (delta >> 2);
    r = Func_02002b08(r);
    g = Func_02002b08(g);
    b = Func_02002b08(b);
    r |= (b << 10) | (g << 5);
    ((u16 *)0x05000000)[index] = r;
}
