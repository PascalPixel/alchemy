#include "object_lookup.h"
#include "types.h"
#include "scene.h"
#include "fixed_math.h"
#include "global_cells.h"

/* effect_runtime/prepare_rising_object.c */
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
void Object_SetMode(struct Entity_0808f0d8 *, s32);
void Object_SetCallback(struct Object_0808f0d8 *, const void *);
extern const u8 gRom[];

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
    Object_SetCallback(object, gRom);
    entity->angle = 0x4000;
}

/* effect_runtime/run_rising_object_sequence.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void Object_SetCallback(void *, s32);
void WaitFrames(u32);
void EffectRuntime_PrepareRisingObject(struct Object_0808f0d8 *object);
void Object_SetMode(void *, s32);

struct GlobalState {
    u8 padding_000[500];
    u32 selected_object;
};

extern struct GlobalState gCell;
extern const u8 gRom[];
extern u8 gRom2;

void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
    void *other;

    if (object != NULL) {
        other = ObjectTable_Get(gCell.selected_object);
        if (flags & 1) {
            Sys_Apply(object, 0);
            Object_SetCallback(object, (s32)gRom);
            FIELD(object, u32, 0x28) = 0x20000;
            FIELD(object, u32, 0x48) = 0x4000;
            FIELD(object, s32, 0x6C) = (s32)&gRom2;
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

/* battle/effects/fx_start_effect_object22.c */
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
void *allocate_heap_block(s32 asset_id, s32 size);

void EmitRandomParticleEffect(void);

void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
    struct EffectResource_0808f1c0 *resource =
        ObjectTable_Get(gCell[125]);
    void *handle = allocate_heap_block(17, 0x608);
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
        Resource_CopyData(visual->value_1c, 128, (u8 *)handle + 0x400);
        Runtime_ReleaseHeapBlock(17);

        if (flags & 1)
            object->callback = (void (*)(void))EmitRandomParticleEffect;
        if (flags & 2)
            prepare_rising_object(object);

        WaitFrames(80);
        Object_SetMode((struct EffectObject_0808f1c0 *)resource, 1);
        Object_Destroy(object);
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

/* effect_runtime/is_active.c */
u32 EffectRuntime_IsActive(void)
{
  s16 active_count;
  u32 is_active;
  void *runtime;
  runtime = *((void **)ADDR_03001EBC);
  is_active = 0;
  if (runtime != ((void *) 0))
  {
    active_count = *((s16 *)(((u8 *)runtime) + 0xCB8));
    is_active = (u32)((0 - active_count) | active_count);
    is_active = is_active >> 0x1F;
  }
  return is_active;
}
