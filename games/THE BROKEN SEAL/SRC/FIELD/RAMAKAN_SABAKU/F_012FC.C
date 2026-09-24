#include "TYPES.H"
#include "FIELD_EVENT.H"

void Effect_Spawn();
void RamakanSabaku_Func02001054(void);
s32 Main_080091d8(struct FieldActor *object, s32 *pos);

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

void RamakanSabaku_Func020012fc(void)
{
    struct FieldActor *center;
    s32 id;
    struct FieldActor *actor;
    s32 pos[3];
    struct DustParams params;
    s32 hit;
    s32 phase;
    s32 x;

    RamakanSabaku_Func02001054();
    {
        struct EventWork *event = gEventWork;

        id = Data_02000240_t.words[125];
        center = event->view_center;
    }
    actor = Engine_ActorGet(id);
    pos[0] = actor->x.fixed;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x18000;
    hit = Main_080091d8(actor, pos);
    phase = gFrameCount & 4;
    if (phase == 0) {
        params.angle = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
        x = actor->x.fixed + (((u32)(Engine_RandomNext() * 12) >> 16) << 16) - 0x60000;
        Effect_Spawn(x, actor->y.fixed, actor->z.fixed, 0, phase,
                     ((u32)(Engine_RandomNext() * 5) >> 16) * 0x1999 + 0x7ffd, 0x800000, &params);
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
    pos[0] = actor->x.fixed;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x80000;
    hit = Main_080091d8(actor, pos);
    if (hit > 0) {
        return;
    }
    pos[0] = actor->x.fixed + 0x5b333;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x5b333;
    hit = Main_080091d8(actor, pos);
    if (hit > 0) {
        return;
    }
    pos[0] = actor->x.fixed - 0x5b333;
    pos[1] = actor->y.fixed;
    pos[2] = actor->z.fixed + 0x5b333;
    hit = Main_080091d8(actor, pos);
    if (hit > 0) {
        return;
    }
    center->z.fixed += 0x18000;
    actor->z.fixed += 0x18000;
}
