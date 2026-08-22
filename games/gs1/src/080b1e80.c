#include "types.h"
#include "global_cells.h"
#include "gs1_edition.h"

#if defined(GS1_EDITION_DE) || defined(GS1_EDITION_FR)
#define EFFECT_X 0x78
#else
#define EFFECT_X 0x80
#endif

struct EffectFlags {
    u8 padding[3];
    u8 flags;
};

/*
 * This owner's view of the object behind 0x03001f2c.
 *
 * NOT `struct BattleRuntime`, which this file used to call it. That name
 * belongs to a different object in games/gs1/include/battle_effect_runtime.h, the one
 * behind Data_03001ebc, and the two disagree about every field: 0x1c8 is an
 * s32 mode word there and sits inside this view's padding. Two structures
 * under one name is how a reader concludes that an offset proved in one owner
 * holds in another.
 *
 * 62 owners reach 0x03001f2c and declare it eight different ways -- `u8 *`,
 * `void *`, PlacementState, FourObjectMotionState, and three `Work_`/`State_`
 * structs named after the owner that wrote them. This follows that last
 * convention, which is the honest one: a per-owner view, named after its
 * owner, claiming nothing about the fields it does not touch.
 *
 * Only the four fields below are evidence. The paddings are this owner's
 * arithmetic, not a layout claim.
 */
struct Work_080b1e80 {
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
extern char Value_00000cad;

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
    struct Work_080b1e80 *runtime;
    u8 *object_state;

    runtime = *(struct Work_080b1e80 **)ADDR_03001F2C;
    object_state = Func_08077008(arg0);
    entry_offset = (arg1 * 2) + 0xd8;
    flags = Func_08077018(*(u16 *)(object_state + entry_offset));
    result = 1;
    effect = Func_080b19cc(*(u16 *)(object_state + entry_offset));
    state = Func_08077020(arg0, arg1);
    selection = state;
    if ((flags->flags & 0x10) && state > 1) {
        Func_080b04dc((s32)&Value_00000cad);
        saved_x = runtime->saved_x;
        saved_y = runtime->saved_y;
        runtime->object[5] = 4;
        runtime->mode = 0xc;
        Func_080b0a6c(NULL, EFFECT_X, 0x30);
        result = Func_080b1614(0, selection, effect);
        Func_080030f8(1);
        Func_080a1038(runtime->object);
        Func_080b0a6c(NULL, saved_x, saved_y);
    }
    return result;
}
