#include "types.h"

struct EffectObject {
    u8 filler00[0xc];
    s32 timer;
    u8 filler10[0x13];
    u8 mode;
    u8 filler24[0xc];
    s32 scale_x;
    s32 scale_y;
    u8 filler38[0x1d];
    u8 state;
    u8 filler56[3];
    u8 phase;
    u8 filler5a[9];
    u8 flag;
    u8 filler64[8];
    s32 callback;
};

extern struct EffectObject *Func_02005a56(s32, s32, s32, s32);
extern void Func_02005ace(struct EffectObject *, s32);
extern void Func_02005a66(struct EffectObject *, s32);
extern void Func_02005a96(struct EffectObject *, s32);

struct EffectObject *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, s32 arg2)
{
    struct EffectObject *obj;
    s32 sx;
    s32 sz;

    sx = x << 16;
    sz = z << 16;
    obj = Func_02005a56(284, sx, 0, sz);
    if (obj == 0) {
        return 0;
    }
    obj->scale_x = 0x10000;
    obj->scale_y = 0x10000;
    Func_02005ace(obj, 0);
    Func_02005a66(obj, 7);
    obj->state = 0;
    obj->timer = 0;
    obj->phase = 0;
    obj->mode = 2;
    obj->callback = 0x02008b99;
    obj->flag = 0;
    Func_02005a96(obj, arg2);
    return obj;
}
