#include "types.h"

struct EffectFlags {
    u8 padding[3];
    u8 flags;
};

struct BattleRuntime {
    u8 padding_000[0x380];
    u8 *object;
    u8 padding_384[4];
    s16 saved_x;
    s16 saved_y;
    u8 padding_38c[0x3a8 - 0x38c];
    u8 mode;
};

u8 *Func_08077008(s32);
struct EffectFlags *Func_08077018(s32);
s32 Func_08077020(s32, s32);
void Func_080b04dc(s32);
s32 Func_080b1614(s32, s32, s32);
s32 Func_080b19cc(u16);
void Func_080b0a6c(void *, s32, s32);
void Func_080030f8(u32);
void Func_080a1038(void *);

s32 Func_080b1e80(s32 arg0, s32 arg1)
{
    s32 result;
    s16 saved_x;
    s16 saved_y;
    s32 effect;
    s32 state;
    s32 entry_offset;
    s32 selection;
    struct EffectFlags *flags;
    struct BattleRuntime *runtime;
    u8 *object_state;

    runtime = *(struct BattleRuntime **)0x03001f2c;
    object_state = Func_08077008(arg0);
    entry_offset = (arg1 * 2) + 0xd8;
    flags = Func_08077018(*(u16 *)(object_state + entry_offset));
    result = 1;
    effect = Func_080b19cc(*(u16 *)(object_state + entry_offset));
    state = Func_08077020(arg0, arg1);
    selection = state;
    if ((flags->flags & 0x10) && state > 1) {
        Func_080b04dc(0xcad);
        saved_x = runtime->saved_x;
        saved_y = runtime->saved_y;
        runtime->object[5] = 4;
        runtime->mode = 0xc;
        Func_080b0a6c(NULL, 0x80, 0x30);
        result = Func_080b1614(0, selection, effect);
        Func_080030f8(1);
        Func_080a1038(runtime->object);
        Func_080b0a6c(NULL, saved_x, saved_y);
    }
    return result;
}
