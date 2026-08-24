#include "types.h"

struct OwnerFirstValueState { u8 padding[0x34]; s16 max; s16 unk; s16 pos; };
struct OwnerFirstValueState *Owner_GetState(s32);
void Owner_RecalculateRatios(s32 arg0);
s16 Owner_AdjustFirstValue(s32 o, s32 d){
    struct OwnerFirstValueState *s = Owner_GetState(o);
    s32 cur = s->pos;
    s32 mx = s->max;
    s32 p = cur + d;
    s32 r;
    if (p > mx) r = mx;
    else { r = 0; if (p >= 0) r = p; }
    s->pos = (s16)r;
    Owner_RecalculateRatios(o);
    return s->pos;
}
