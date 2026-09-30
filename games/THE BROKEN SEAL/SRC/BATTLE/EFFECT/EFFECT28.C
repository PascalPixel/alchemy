#include "TYPES.H"
#include "FIXED_MATH.H"
#include "OBJECT_EFFECT.H"
#include "SYSTEM.H"
#include "OBJECT_LOOKUP.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

struct ParticlePosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ParticleEmitter {
    u8 padding[8];
    struct ParticlePosition position;
    u8 padding2[20];
    s32 travel_offset;
    u8 padding3[41];
    u8 active;
};

struct ParticleChild {
    u8 padding[9];
    u8 flags;
};

struct ParticleEffectObject {
    u8 padding[80];
    struct ParticleChild *child;
};

#define OBJECT_0808EEE4_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char ParticlePosition_size[
    sizeof(struct ParticlePosition) == 0x0c ? 1 : -1
];
typedef char ParticleEmitter_travel_offset_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEmitter, travel_offset) == 0x28 ? 1 : -1
];
typedef char ParticleEmitter_active_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEmitter, active) == 0x55 ? 1 : -1
];
typedef char ParticleEffectObject_child_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEffectObject, child) == 0x50 ? 1 : -1
];
extern void Vector_AddPolarOffset(s32, s32, struct ParticlePosition *);
extern void ObjectDispatch_InitializeFar(struct ParticleEffectObject *, void *);
extern void Object_SetMode(struct ParticleEffectObject *, s32);
extern const u8 BattleFx_ParticleScript[];

struct EfxSrc {
    u8 pad0[6];
    u16 ang;
    s32 x;
    s32 y;
    s32 z;
};

struct EfxPos {
    s32 x;
    s32 y;
    s32 z;
};

struct EfxVisual {
    u8 pad0[5];
    u8 flatbs;
    u8 pad1[3];
    u8 flatla;
    u8 pad2[18];
    u8 slot;
    u8 pad3[9];
    u8 unk1;
    u8 unk2;
};

struct TileBits {
    u16 tile : 10;
};

struct EfxObj {
    s32 data;
    u8 pad0[0x24];
    u32 travel;
    u8 pad1[0x1c];
    u32 dy;
    u8 pad2[4];
    struct EfxVisual *vis;
    u8 pad3[24];
    void (*proc)(void);
};

extern u8 *gEventWork;
extern const u8 BattleFx_ParticleEmitterScript[];
struct EfxObj *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void Object_Destroy(struct EfxObj *obj);
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void ItemIcon_LoadTilesFar(s32 item);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);
void BattleFx_SpawnRandomParticleAtPosition(const void *src);
#define EfxPool (*(struct EfxObj **)((u8 *)&gEventWork - 88))
#define BATTLE_ACTIVE_OFS 0xcb8

void WaitFrames(s32);
struct EffectObject_0808f1c0;

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

extern struct State_0808f0d8 gGameState;
void Object_SetPosition(struct Object_0808f0d8 *, s32, s32, s32);
extern const u8 RomBytes_0809e75c[];

/* effect_runtime/Effect_RunRisingObjectSequence.c */
void ObjectDispatch_SetSingleChildField26Far(void *, s32);

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

void *Runtime_AllocateHeapBlock(s32 asset_id, s32 size);
void ItemIcon_LoadTilesFar(s32);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);
void Runtime_ReleaseHeapBlock(s32);

void BattleFx_EmitRandomParticleFromEmitter(struct ParticleEmitter *emitter);

#undef OBJECT_0808EEE4_OFFSET

void BattleFx_EmitRandomParticleFromEmitter(struct ParticleEmitter *emitter)
{
    struct ParticlePosition position;
    struct ParticleEffectObject *object;
    u32 random_angle;

    if (emitter->travel_offset >= -255 && emitter->travel_offset <= 255)
        emitter->active = 0;

    if ((100 * Random16() >> 16) > 9)
        return;

    position.x = emitter->position.x;
    position.y = emitter->position.y;
    position.z = emitter->position.z;
    random_angle = Random16();
    Vector_AddPolarOffset(random_angle << 4, Random16(), &position);
    object = (struct ParticleEffectObject *)Object_Spawn(
        0x11D, position.x, position.y, position.z);
    if (object != 0) {
        s32 mask;
        u8 flags;

        ObjectDispatch_InitializeFar(object, (void *)BattleFx_ParticleScript);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}

struct EfxObj *BattleFx_StartRandomParticleEmitter(s32 obj_id, s32 item)
{
    struct EfxObj *obj;
    /* GCC shape: work holds the state base, then the pool count. */
    s32 work = (s32)gEventWork;
    u32 off = (obj_id * 4) + 0x14;
    /* GCC shape: src_z holds the source pointer, then the spawn Z. */
    s32 src_z = (s32)*(struct EfxSrc **)(work + off);
    struct EfxPos pos;
    s32 x;

    if (src_z == 0)
        return 0;

    pos.x = ((struct EfxSrc *)src_z)->x;
    pos.y = ((struct EfxSrc *)src_z)->y;
    pos.z = ((struct EfxSrc *)src_z)->z;
    Vector_AddPolarOffset(
        0x100000,
        ((struct EfxSrc *)src_z)->ang,
        &pos);
    x = (pos.x & 0xfff00000) + 0x80000;
    src_z = (pos.z & 0xfff00000) + 0x80000;

    if (*(s16 *)(work + BATTLE_ACTIVE_OFS) != 0) {
        struct EfxObj *ent = EfxPool;

        work = 63;

        do {
            if (ent->data != 0) {
                if (ent->proc == (void (*)(void))BattleFx_SpawnRandomParticleAtPosition) {
                    Object_Destroy(ent);
                }
                if (ent->data == (s32)BattleFx_ParticleScript) {
                    Object_Destroy(ent);
                }
            }
            work--;
            ent++;
        } while (work >= 0);
    }

    WaitFrames(3);
    obj = Object_CreateFar(22, x, 0x100000, src_z);
    if (obj == 0)
        return 0;

    ObjectDispatch_InitializeFar(obj, (s32)BattleFx_ParticleEmitterScript);
    {
        struct EfxVisual *vis = obj->vis;
        void *buf;
        s32 mask;
        s32 zero = 0;

        vis->unk1 = zero;
        vis->unk2 = zero;

        vis->flatbs &= zero - 33;

        mask = vis->flatla & 0x0f;
        {
            s32 clr = 13;
            clr = -clr;
            mask &= clr;
        }
        mask |= 4;
        vis->flatla = mask;

        obj->travel = 0x20000;
        obj->dy = 0x4000;

        buf = Runtime_AllocateHeapBlock(17, 0x608);
        ItemIcon_LoadTilesFar(item);

        ((struct TileBits *)((u8 *)vis + 8))->tile =
            VramBlock_LoadCached(vis->slot, 128, (u8 *)buf + 0x400);

        Runtime_ReleaseHeapBlock(17);
        obj->proc = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
    }

    return obj;
}

void Object_DestroyIfPresent(void *object)
{
    if (object != 0)
        Object_Destroy(object);
}

void EffectRuntime_PrepareRisingObject(struct Object_0808f0d8 *object)
{
    struct Entity_0808f0d8 *entity;

    if (object == 0)
        return;

    entity = ObjectTable_Get(gGameState.object_index);
    object->field34 = 0x10000;
    object->field30 = 0x20000;
    object->field55 = 0;
    Object_SetPosition(object, entity->x, entity->y + 0x240000, entity->z);
    WaitFrames(3);
    Object_SetMode(entity, 28);
    ObjectDispatch_InitializeFar(object, RomBytes_0809e75c);
    entity->angle = 0x4000;
}

void EffectRuntime_RunRisingObjectSequence(void *object, s32 flags)
{
    void *other;

    if (object != NULL) {
        other = ObjectTable_Get(gGameState.object_index);
        if (flags & 1) {
            ObjectDispatch_SetSingleChildField26Far(object, 0);
            ObjectDispatch_InitializeFar(object, BattleFx_ParticleEmitterScript);
            FIELD_AT_OFFSET(object, u32 *, 0x28) = 0x20000;
            FIELD_AT_OFFSET(object, u32 *, 0x48) = 0x4000;
            FIELD_AT_OFFSET(object, s32 *, 0x6C) = (s32)&BattleFx_EmitRandomParticleFromEmitter;
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

void BattleFx_StartEffectObject22(s32 value, s32 flags)
{
    struct EffectResource_0808f1c0 *resource =
        ObjectTable_Get(gGameState.object_index);
    void *handle = Runtime_AllocateHeapBlock(17, 0x608);
    struct EffectObject_0808f1c0 *object = Object_CreateFar(
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

        ItemIcon_LoadTilesFar(value);
        VramBlock_LoadCached(visual->value_1c, 128, (u8 *)handle + 0x400);
        Runtime_ReleaseHeapBlock(17);

        if (flags & 1)
            object->callback = (void (*)(void))BattleFx_EmitRandomParticleFromEmitter;
        if (flags & 2)
            EffectRuntime_PrepareRisingObject((struct Object_0808f0d8 *)object);

        WaitFrames(80);
        Object_SetMode(resource, 1);
        Object_Destroy(object);
    }
}
