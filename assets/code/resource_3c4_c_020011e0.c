#include "types.h"

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

extern struct SceneObject *Func_020042fa(void);
extern s32 Func_02004254(s32);
extern s32 Func_02004258(s32);
extern s32 Func_0200423e(s32, s32);
extern void Func_02001d32(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

void Func_020011e0(void)
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
