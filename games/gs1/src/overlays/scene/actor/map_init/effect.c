#include "types.h"

#define OverlayObject_CreateAndInitialize Func_020009f4
#define SceneEffect_SpawnNineRadialEffects Func_020011e0
struct SceneObject {
    u8 filler00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 unk00;
    s32 mode;
    u8 filler08[0x1c];
    s32 callback;
};

u8 *Func_02003a6a(s32 kind, s32 x, s32 y, s32 z);
void Func_02003ad8(u8 *obj, s32 mode);
void Func_02003b98(u8 *obj, s32 mode);
void Func_02003af0(u8 *obj, s32 mode);
struct SceneObject *Func_020042fa(void);
s32 Func_02004254(s32);
s32 Func_02004258(s32);
s32 Func_0200423e(s32, s32);
void Func_02001d32(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

/*
 * Create an object, clear the low bits of its owner record, and initialise it.
 * The four callees are declared at their in-overlay entry points, not at
 * veneers.  The +9 flag clear takes its mask from a local that is initialised
 * first and only afterwards ANDed with the loaded byte; written as one
 * expression it swaps which register holds the mask and which the byte.
 */

u8 *OverlayObject_CreateAndInitialize(s32 x, s32 y, s32 z, s32 kind)
{
    u8 *ret;
    u8 *obj = Func_02003a6a(kind, x, y, z);

    if (obj != 0) {
        u8 *owner = *(u8 **)(obj + 80);
        u32 mask = ~12;
        mask = mask & owner[9];
        owner[9] = (u8)mask;
        obj[85] = 0;
        obj[89] = 8;
        Func_02003ad8(obj, 0);
        Func_02003b98(obj, 14);
        Func_02003af0(obj, 1);
        ret = obj;
    } else {
        ret = 0;
    }
    return ret;
}

void SceneEffect_SpawnNineRadialEffects(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Func_020042fa();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x02009069;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Func_02004254(v);
        vec.y = 0;
        z = Func_02004258(v);
        x = vec.x;
        vec.z = z;
        x = x + Func_0200423e(x, 3);
        vec.x = x;
        Func_02001d32(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}
