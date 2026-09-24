#include "TYPES.H"
#include "FIELD_EVENT.H"

void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, s32 params);

/* Spin actor 19 out along a quickening spiral, lift it and throw three puffs. */
void ShianJiin_SpinAway(void)
{
    struct FieldActor *actor;
    u32 i;
    s32 angle;

    actor = Engine_ActorGet(19);
    for (i = 8; i <= 12; i++) {
        angle = i << 12;
        actor->sprite->rotation = angle;
        Engine_TaskWait((12 - i) * 2);
        actor->x.fixed -= Engine_MathCos(angle) * 6;
        actor->z.fixed -= Engine_MathSin(angle) * 6;
    }
    actor->y.fixed = 0x120000;
    actor->target_y = 0x120000;
    actor->scale_x = 0xffff3334;
    Engine_AudioPlayCue(227);
    Effect_Spawn(actor->x.fixed - 0xc0000, actor->y.fixed, actor->z.fixed + 0x80000, 0x10000, 0x3333, 0, 0, 0);
    Effect_Spawn(actor->x.fixed, actor->y.fixed, actor->z.fixed + 0x80000, 0xcccc, 0x4ccc, 0, 0, 0);
    Effect_Spawn(actor->x.fixed + 0xa0000, actor->y.fixed, actor->z.fixed + 0x80000, 0x3333, 0x6666, 0, 0, 0);
}
