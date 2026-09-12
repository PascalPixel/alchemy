#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"

struct EffectObject_0808f1c0;
void Object_Destroy(struct EffectObject_0808f1c0 *);

void Object_DestroyIfPresent(void *object)
{
    if (object != 0)
        Object_Destroy(object);
}

/* effect_runtime/prepare_rising_object.c */
struct State_0808f0d8 {
    u8 pad0[0x1f4];
    s32 object_index;
};

struct Entity_0808f0d8 {
    u8 pad0[6];
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
};

struct Object_0808f0d8 {
    u8 pad0[0x30];
    s32 field30;
    s32 field34;
    u8 pad38[0x1d];
    u8 field55;
};

extern struct State_0808f0d8 gCell;
void Object_SetPosition(struct Object_0808f0d8 *, s32, s32, s32);
void WaitFrames(s32);
void Object_SetMode(void *, s32);
void Object_SetCallback(void *, const void *);
extern const u8 RomBytes_0809e75c[];

void EffectRuntime_PrepareRisingObject(struct Object_0808f0d8 *object)
{
    struct Entity_0808f0d8 *entity;

    if (object == 0)
        return;

    entity = ObjectTable_Get(gCell.object_index);
    object->field34 = 0x10000;
    object->field30 = 0x20000;
    object->field55 = 0;
    Object_SetPosition(object, entity->x, entity->y + 0x240000, entity->z);
    WaitFrames(3);
    Object_SetMode(entity, 28);
    Object_SetCallback(object, RomBytes_0809e75c);
    entity->angle = 0x4000;
}

/* effect_runtime/run_rising_object_sequence.c */
void Sys_Apply(void *, s32);
extern const u8 gRom[];
extern u8 gRom2;

void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
    void *other;

    if (object != NULL) {
        other = ObjectTable_Get(gCell.object_index);
        if (flags & 1) {
            Sys_Apply(object, 0);
            Object_SetCallback(object, gRom);
            FIELD_AT_OFFSET(object, u32 *, 0x28) = 0x20000;
            FIELD_AT_OFFSET(object, u32 *, 0x48) = 0x4000;
            FIELD_AT_OFFSET(object, s32 *, 0x6C) = (s32)&gRom2;
        }
        if (flags == 3) {
            WaitFrames(60);
        }
        if (flags & 2) {
            EffectRuntime_PrepareRisingObject(object);
        }
        if (flags == 3) {
            WaitFrames(80);
        }
        Object_SetMode(other, 1);
    }
}

/* battle/effects/objects/start_effect_22.c */
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

struct EffectObject_0808f1c0 *Battle_Run(s32 kind, s32 x, s32 y, s32 z);
void *Battle_Run2(s32 asset_id, s32 size);
void Battle_Do(s32);
s32 Battle_Place(u32 slot, u32 size, const void *source);
void Battle_Do2(s32);
void EmitRandomParticleEffect(void);

void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
    struct EffectResource_0808f1c0 *resource =
        ObjectTable_Get(gCell.object_index);
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
            EffectRuntime_PrepareRisingObject((struct Object_0808f0d8 *)object);

        WaitFrames(80);
        Object_SetMode(resource, 1);
        Object_Destroy(object);
    }
}
