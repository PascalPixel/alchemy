#include "types.h"
#include "scene.h"
#include "abi/battle/effects/objects/start_effect_22.h"

/* Object table: 192 pointers at gWork + 0x14 (object/table/get.c). */
void *ObjectTable_Get(u32 object);

struct EffectVisual_0808f1c0 {
    u8 unknown_00[5];
    u8 flags_a;
    u8 unknown_06[3];
    u8 flags_b;
    u8 unknown_0a[18];
    u8 value_1c;
    u8 unknown_1d[9];
    u8 value_26;
    u8 value_27;
};

struct EffectResource_0808f1c0 {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
};

struct EffectObject_0808f1c0 {
    u8 unknown_00[80];
    struct EffectVisual_0808f1c0 *visual;
    u8 unknown_54[24];
    void (*callback)(void);
};

extern s32 gCell[];

struct EffectObject_0808f1c0 *Battle_Run(s32 kind, s32 x, s32 y, s32 z);
void *Battle_Run2(s32 asset_id, s32 size);

void EmitRandomParticleEffect(void);

void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
    struct EffectResource_0808f1c0 *resource =
        ObjectTable_Get(gCell[125]);
    void *handle = Battle_Run2(17, 0x608);
    struct EffectObject_0808f1c0 *object = Battle_Run(
        22, resource->x, resource->y + 0x240000, resource->z);

    if (object != 0) {
        struct EffectVisual_0808f1c0 *visual = object->visual;
        s32 mask;

        s32 zero = 0;
        visual->value_26 = zero;
        visual->value_27 = zero;

        visual->flags_a &= zero - 33;

        mask = visual->flags_b & 0x0f;
        {
            s32 clear = 13;
            clear = -clear;
            mask &= clear;
        }
        mask |= 4;
        visual->flags_b = mask;

        Battle_Do(value);
        Battle_Place(visual->value_1c, 128, (u8 *)handle + 0x400);
        Battle_Do2(17);

        if (flags & 1)
            object->callback = (void (*)(void))EmitRandomParticleEffect;
        if (flags & 2)
            Battle_Do3(object);

        Battle_Do4(80);
        Battle_Apply((struct EffectObject_0808f1c0 *)resource, 1);
        Battle_Do5(object);
    }
}
