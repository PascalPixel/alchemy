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

extern struct SceneObject *Func_020059c2(void);
extern s32 Func_020058fc(s32);
extern s32 Func_02005900(s32);
extern s32 Func_020058ee(s32, s32);
extern void Func_02000c8a(s32, s32, s32, s32, s32, s32, s32, struct EffectParams *);

void Func_02000b08(void)
{
    struct SceneObject *object;
    struct Vec vec;
    struct EffectParams params;
    u32 i;
    s32 v;
    s32 x;
    s32 z;

    object = Func_020059c2();
    params.unk00 = 1;
    params.mode = 7;
    params.callback = 0x0200896d;
    for (i = 0; i <= 16; i += 2) {
        v = i << 12;
        vec.x = Func_020058fc(v);
        vec.y = 0;
        z = Func_02005900(v);
        x = vec.x;
        vec.z = z;
        x = x + Func_020058ee(x, 3);
        vec.x = x;
        Func_02000c8a(object->x, object->y, object->z, x, vec.y, z, 0x01030001, &params);
    }
}
