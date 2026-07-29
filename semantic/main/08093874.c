#include "types.h"

struct EffectVisual_08093874 {
    u8 unknown_00[9];
    u8 orientation;
    u8 unknown_0a[0x1c];
    u8 state_26;
};

struct EffectObject_08093874 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[0x3c];
    struct EffectVisual_08093874 *visual;
    u8 unknown_54;
    u8 state_55;
    u8 unknown_56[0x0e];
    u16 phase;
    u16 source_id;
    struct EffectObject_08093874 *linked_object;
    s32 (*callback)(struct EffectObject_08093874 *);
};

struct EffectObject_08093874 *Func_0808ba1c(u32 id);
struct EffectObject_08093874 *Func_080090c8(s32, s32, s32, s32);
void Func_080090d0(struct EffectObject_08093874 *object);
void Func_08009080(struct EffectObject_08093874 *object, s32 mode);
void Func_08009098(
    struct EffectObject_08093874 *object,
    const void *animation);
s32 Func_0809376c(struct EffectObject_08093874 *object);

/*
 * Create, replace, or remove the effect linked to a world object.  Mode one
 * owns the effect from the source; mode two creates an independent variant.
 */
void Func_08093874(u32 source_id, s32 flags)
{
    struct EffectObject_08093874 *source = Func_0808ba1c(source_id);
    struct EffectObject_08093874 *effect = 0;
    s32 mode = flags & 3;

    if (source == 0) {
        return;
    }

    if (mode == 0) {
        if (source->linked_object != 0) {
            Func_080090d0(source->linked_object);
            source->linked_object = 0;
        }
        return;
    }

    if (mode == 2 || source->linked_object == 0) {
        effect = Func_080090c8(
            0xd1, source->x, source->y, source->z);
    }
    if (effect == 0) {
        return;
    }

    switch (mode) {
    case 1:
        Func_08009080(effect, 1);
        source->linked_object = effect;
        effect->phase = 1;
        break;
    case 2:
        Func_08009080(effect, 2);
        Func_08009098(effect, (const void *)0x0809fd38);
        effect->phase = 1;
        break;
    }

    effect->source_id = (u16)source_id;
    effect->state_55 = 0;
    effect->callback = Func_0809376c;
    effect->visual->state_26 = 0;
    effect->linked_object = source;

    if ((flags & 0x100) != 0) {
        effect->visual->orientation =
            (effect->visual->orientation & (u8)~0x0c) | 4;
    } else {
        effect->visual->orientation =
            (effect->visual->orientation & (u8)~0x0c) |
            (source->visual->orientation & 0x0c);
    }
}
