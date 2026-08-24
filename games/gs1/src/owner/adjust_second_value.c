#include "types.h"

struct OwnerSecondValueState { u8 padding[0x36]; s16 max; s16 pad2; s16 pos; };
struct OwnerSecondValueState *Owner_GetState(s32);
void Owner_RecalculateRatios(s32 arg0);
s16 Owner_AdjustSecondValue(s32 o, s32 d){ struct OwnerSecondValueState *s=Owner_GetState(o);
 s32 cur=s->pos; s32 mx=s->max; s32 p=cur+d;
 s32 r; if(p>mx) r=mx; else { r=0; if(p>=0) r=p; }
 s->pos=r; Owner_RecalculateRatios(o); return s->pos; }
