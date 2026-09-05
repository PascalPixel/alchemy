#include "types.h"

typedef struct {
    u8 filler0[0x28];
    s16 *unk28;
} Obj;

typedef struct {
    u8 filler0[0x50];
    Obj *obj;
} Spr;

Spr *Func_0200621c(s32);
s32 Func_02000976(u8 *, u8 *);

s32 SceneActor_FindNearestSlotOfKindF2(void)
{
    u8 *work = *(u8 **)0x03001ebc;
    Spr **p;
    s32 best = 0;
    Spr *ref;
    s32 limit;
    u32 i;

    limit = 640;
    ref = Func_0200621c(0);
    i = 8;
    p = (Spr **)(work + 0x34);
    do {
        Spr *spr = *p++;
        if (spr != 0) {
            if (*spr->obj->unk28 == 0xf2) {
                s32 dist = Func_02000976((u8 *)ref + 8, (u8 *)spr + 8);
                if (dist < limit) {
                    limit = dist;
                    best = i;
                }
            }
        }
        i++;
    } while (i <= 65);
    return best;
}
