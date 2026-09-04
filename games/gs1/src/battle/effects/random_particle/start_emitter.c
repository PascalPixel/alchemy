#include "types.h"

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

extern u8 *Data_03001ebc;
extern const u8 Data_0809e6c0[];
extern const u8 Data_0809e87c[];
#define Object_CreateFar Func_080090c8
#define ItemIcon_LoadTilesFar Func_08015250
#define VramBlock_LoadCached Func_08003fa4
#define Runtime_ReleaseHeapBlock Func_08002dd8

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
void BattleEffect_SpawnRandomParticleAtPosition(const void *src);

#define EfxWork Data_03001ebc
#define EmitterData Data_0809e6c0
#define ParticleData Data_0809e87c
#define EfxPool (*(struct EfxObj **)((u8 *)&Data_03001ebc - 88))
#define BATTLE_ACTIVE_OFS 0xcb8

#define BattleEffect_StartRandomParticleEmitter Func_0808ef70

struct EfxObj *BattleEffect_StartRandomParticleEmitter(s32 obj_id, s32 item)
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
                if (ent->proc == (void (*)(void))BattleEffect_SpawnRandomParticleAtPosition) {
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
