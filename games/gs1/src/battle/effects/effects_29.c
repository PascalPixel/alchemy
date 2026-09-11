#include "types.h"
#include "scene.h"
#include "fixed_math.h"

/* battle/effects/objects/start_effect_22.c */
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
void *Battle_unk2_4(s32 asset_id, s32 size);

void EmitRandomParticleEffect(void);

void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
    struct EffectResource_0808f1c0 *resource =
        ObjectTable_Get(gCell[125]);
    void *handle = Battle_unk2_4(17, 0x608);
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
        Battle_unk2_2(17);

        if (flags & 1)
            object->callback = (void (*)(void))EmitRandomParticleEffect;
        if (flags & 2)
            Battle_unk3_2(object);

        Battle_unk4_2(80);
        Battle_Apply((struct EffectObject_0808f1c0 *)resource, 1);
        Battle_unk5(object);
    }
}

/* battle/effects/random_particle/spawn_at_position.c */
struct Values_0808f28c {
    u32 first;
    u32 second;
    u32 third;
};

struct Source_0808f28c {
    u8 padding[8];
    struct Values_0808f28c values;
};

struct Child_0808f28c {
    u8 padding[9];
    u8 flags;
};

struct Object_0808f28c {
    u8 padding[80];
    struct Child_0808f28c *child;
};

extern u32 Random16(void);
extern void RotateVectorByMagnitude(s32, s32, struct Values_0808f28c *);
extern struct Object_0808f28c *Object_Spawn(s32, u32, u32, u32);
extern void Object_SetCallback(struct Object_0808f28c *, void *);
extern void Object_SetMode(struct Object_0808f28c *, s32);

void BattleFx_SpawnRandomParticleAtPosition(const struct Source_0808f28c *source)
{
    struct Values_0808f28c values;
    struct Object_0808f28c *object;
    u32 rnd;

    if ((100 * Random16() >> 16) > 9)
        return;

    values.first = source->values.first;
    values.second = source->values.second;
    values.third = source->values.third;
    rnd = Random16();
    RotateVectorByMagnitude(rnd << 4, Random16(), &values);
    object = Object_Spawn(
        0x11D, values.first, values.second, values.third);
    if (object != 0) {
        s32 mask;
        u8 flags;

        Object_SetCallback(object, (void *)0x0809E87C);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}
