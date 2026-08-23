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
    u8 filler00[0x24];
    s32 callback;
};

extern struct SceneObject *Func_02003e96(void);
extern s32 Func_02003df8(s32);
extern s32 Func_02003dfc(s32);
extern s32 Func_02003df2(s32, s32);
extern void Func_0200194a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

#define SpawnRadialEffectBurst Func_02000e00

void SpawnRadialEffectBurst(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 angle_step;
    s32 angle;
    s32 x;
    s32 z;

    object = Func_02003e96();
    params.callback = 0x02008da5;
    for (angle_step = 0; angle_step <= 16; angle_step += 2) {
        angle = angle_step << 12;
        vec.x = Func_02003df8(angle);
        vec.y = 0;
        z = Func_02003dfc(angle);
        x = vec.x;
        vec.z = z;
        x = x + Func_02003df2(x, 3);
        vec.x = x;
        Func_0200194a(object->x, object->y, object->z, x, vec.y, z, 0x01000001, &params);
    }
}
