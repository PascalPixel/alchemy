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

struct EffectObject *Func_02000c5c(s32 x, s32 z, s32 arg2)
{
    struct EffectObject *object;
    s32 sx;
    s32 sz;

    sx = x << 16;
    sz = z << 16;
    object = Func_02005a56(284, sx, 0, sz);
    if (object == 0) {
        return 0;
    }
    object->scale_x = 0x10000;
    object->scale_y = 0x10000;
    Func_02005ace(object, 0);
    Func_02005a66(object, 7);
    object->state = 0;
    object->timer = 0;
    object->phase = 0;
    object->mode = 2;
    object->callback = 0x02008b99;
    object->flag = 0;
    Func_02005a96(object, arg2);
    return object;
}
