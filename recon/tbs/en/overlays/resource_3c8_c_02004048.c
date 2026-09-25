/* NONMATCHING: 628 bytes, candidate 628, 10 differing halfwords, 10 halfword
 * edits (2026-09-25). VinasuHeya_Func02004048, meant for
 * FIELD/VINASU_HEYA/F_04048.C as a single-overlay unit binding its names at
 * their runtime addresses (an import veneer's listing offset plus 0x8000).
 * Remaining: WALL: allocation-uncovered 10 HW — id↔center and
 * &params↔vx stack slots swapped, plus dir->x*scale mul operand order;
 * size-exact; RamakanSabaku_Func020012fc sibling shape */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void Effect_Spawn();
s32 Main_080091d8(struct FieldActor *object, s32 *pos);

struct DirXZ {
    s32 x;
    s32 unused;
    s32 z;
};

struct DustParams {
    u8 unknown_00[34];
    u16 angle;
    u8 unknown_24[4];
};

union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
};

extern union GameStateRows Data_02000240_t;

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

void VinasuHeya_Func02004048(struct DirXZ *dir)
{
    struct FieldActor *center;
    s32 id;
    struct FieldActor *actor;
    s32 pos[3];
    struct DustParams params;
    s32 hit;
    s32 phase;
    s32 x;
    s32 vx;
    s32 vz;

    {
        struct EventWork *event = gEventWork;

        id = Data_02000240_t.words[125];
        center = event->view_center;
    }
    actor = Engine_ActorGet(id);
    pos[0] = actor->x.fixed + ((dir->x * 3) << 15);
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + ((dir->z * 3) << 15);
    hit = Main_080091d8(actor, pos);
    phase = gFrameCount & 4;
    if (phase == 0) {
        params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        x = actor->x.fixed + (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
        vx = dir->x * (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd);
        vz = dir->z * (((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd);
        Effect_Spawn(x, actor->y.fixed, actor->z.fixed, vx, phase, vz, 0x800000, &params);
    }
    if (hit < 0) {
        Call2((void (*)())Engine_ActorSetAttachedEffect, id, 0x102);
        Engine_ObjectSetPosition(actor, actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000);
        Engine_ObjectSetAnimation(actor, 7);
        Engine_ObjectCommitPosition(actor);
        do {
            Engine_TaskWait(1);
        } while (actor->y.fixed != *(s32 *)((u8 *)actor + 20));
        Engine_ObjectSetAnimation(actor, 6);
        Engine_TaskWait(3);
        return;
    }
    pos[0] = actor->x.fixed + (dir->x << 19);
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + (dir->z << 19);
    hit = Main_080091d8(actor, pos);
    if (hit > 0) {
        return;
    }
    pos[0] = actor->x.fixed + dir->x * 0x5b333 - dir->z * 0x5b333;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + dir->z * 0x5b333 - dir->x * 0x5b333;
    hit = Main_080091d8(actor, pos);
    if (hit > 0) {
        actor->x.fixed += (dir->z * 3) << 15;
        actor->z.fixed += (dir->x * 3) << 15;
        return;
    }
    pos[0] = (dir->x + dir->z) * 0x5b333 + actor->x.fixed;
    pos[1] = actor->y.fixed;
    pos[2] = (dir->x + dir->z) * 0x5b333 + actor->z.fixed;
    hit = Main_080091d8(actor, pos);
    if (hit > 0) {
        actor->x.fixed -= (dir->z * 3) << 15;
        actor->z.fixed -= (dir->x * 3) << 15;
        return;
    }
    center->x.fixed += (dir->x * 3) << 15;
    actor->x.fixed += (dir->x * 3) << 15;
    center->z.fixed += (dir->z * 3) << 15;
    actor->z.fixed += (dir->z * 3) << 15;
}
