#include "scene.h"
#include "effect_runtime.h"
#include "object_lookup.h"
#include "global_cells.h"
#include "fixed_math.h"
#include "types.h"
#include "object_effect.h"

/* battle/effects/particles/emit_random.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 ArcTan2(s32, s32);
void RotateVectorByMagnitude(s32, u16, void *);

struct GlobalData {
    u8 filler[0x1F4];
    u32 value_1F4;
};

extern struct GlobalData gCell;
extern char Value_fff80000;
extern char Value_001ffffe;

void BattleFx_EmitRandomParticle(void)
{
    register s32 rel_x;
    register s32 rel_y;
    register s32 x_offset;
    register s32 y_offset;
    register s32 x_delta;
    register s32 y_delta;
    register s32 object_x;
    register s32 object_y;
    register s32 negative_center;
    register u32 maximum;
    register s32 center;
    register s32 index;
    register u8 *object;
    register u8 *entry;

    object = ObjectTable_Get(gCell.value_1F4);
    entry = *(u8 **)ADDR_03001EBC + 0x11C;
    index = 0;
    if (entry[4] != 0) {
        object_x = FIELD(object, s32, 8);
        object_y = FIELD(object, s32, 0x10);
        negative_center = (s32)&Value_fff80000;
        maximum = (u32)&Value_001ffffe;
        center = 128;
        center <<= 12;
loop:
        x_offset = entry[6] << 20;
        x_delta = object_x - x_offset;
        rel_x = x_delta + negative_center;
        y_offset = entry[7] << 20;
        y_delta = object_y - y_offset;
        rel_y = y_delta + negative_center;
        if ((u32)(x_delta + 0x7FFFF) <= maximum &&
            (u32)(y_delta + 0x7FFFF) <= maximum) {
            FIELD(object, s32, 8) = x_offset + center;
            FIELD(object, s32, 0x10) = y_offset + center;
            RotateVectorByMagnitude(0x140000,
                          (u16)ArcTan2(rel_y, rel_x),
                          object + 8);
            FIELD(object, s32, 0x38) = 0x80000000;
            FIELD(object, s32, 0x3C) = 0x80000000;
            FIELD(object, s32, 0x40) = 0x80000000;
            return;
        }
        index++;
        entry += 8;
        if (index <= 9 && entry[4] != 0)
            goto loop;
    }
}

/* battle/effects/random_particle/emit.c */
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

extern u32 Random16(void);
extern void RotateVectorByMagnitude(s32, s32, struct ParticlePosition *);
extern void Object_SetCallback(struct ParticleEffectObject *, void *);
extern void Object_SetMode(struct ParticleEffectObject *, s32);
extern const u8 gRom[];

void EmitRandomParticleEffect(struct ParticleEmitter *emitter)
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
    RotateVectorByMagnitude(random_angle << 4, Random16(), &position);
    object = (struct ParticleEffectObject *)Object_Spawn(
        0x11D, position.x, position.y, position.z);
    if (object != 0) {
        s32 mask;
        u8 flags;

        Object_SetCallback(object, (void *)gRom);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}

/* battle/effects/random_particle/start_emitter.c */
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
    u8 flags1;
    u8 pad1[3];
    u8 flags2;
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

extern u8 *EfxWork;
extern const u8 EmitterData[];
extern const u8 ParticleData[];

void RotateVectorByMagnitude(s32 mag, s32 ang, struct EfxPos *pos);
struct EfxObj *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct EfxObj *obj, s32 data);
void Object_Destroy(struct EfxObj *obj);
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void WaitFrames(s32 frames);
void ItemIcon_LoadTilesFar(s32 item);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);
void Runtime_ReleaseHeapBlock(s32 kind);
void EmitRandomParticleEffect(void);
void BattleFx_SpawnRandomParticleAtPosition(const void *src);

#define BATTLE_ACTIVE_OFS 0xcb8

struct EfxObj *BattleFx_StartRandomParticleEmitter(s32 obj_id, s32 item)
{
    struct EfxObj *obj;
    /* GCC shape: work holds the state base, then the pool count. */
    s32 work = (s32)EfxWork;
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
    RotateVectorByMagnitude(
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
                if (ent->data == (s32)ParticleData) {
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

    ObjectDispatch_InitializeFar(obj, (s32)EmitterData);
    {
        struct EfxVisual *vis = obj->vis;
        void *buf;
        s32 mask;
        s32 zero = 0;

        vis->unk1 = zero;
        vis->unk2 = zero;

        vis->flags1 &= zero - 33;

        mask = vis->flags2 & 0x0f;
        {
            s32 clr = 13;
            clr = -clr;
            mask &= clr;
        }
        mask |= 4;
        vis->flags2 = mask;

        obj->travel = 0x20000;
        obj->dy = 0x4000;

        buf = Runtime_AllocateHeapBlock(17, 0x608);
        ItemIcon_LoadTilesFar(item);

        ((struct TileBits *)((u8 *)vis + 8))->tile =
            VramBlock_LoadCached(vis->slot, 128, (u8 *)buf + 0x400);

        Runtime_ReleaseHeapBlock(17);
        obj->proc = (void (*)(void))EmitRandomParticleEffect;
    }

    return obj;
}
