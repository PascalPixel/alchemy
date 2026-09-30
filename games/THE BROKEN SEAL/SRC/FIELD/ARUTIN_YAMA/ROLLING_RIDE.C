/* Arutin mountain: the leader is carried with actor 8 until it has come
   level with it, then the rolling object starts. */
#include "TYPES.H"
#include "FIELD_EVENT.H"


void ArutinYama_RunRollingObject(s32 id, s32 heading);
void Battle_ResetEffectCounter(void);
void BattleFx_PlayQueuedSound(void);

void ArutinYama_BeginRollingRide(s32 a0)
{
    u32 i;
    struct FieldActor *rec7;
    struct FieldActor *rec8;
    s32 record;
    s32 v3;

    rec8 = Engine_ActorGet(0);
    rec7 = Engine_ActorGet(8);
    Battle_ResetEffectCounter();
    Engine_EventBegin();
    Engine_ActorSetAnimation(0, 22);
    Engine_EventWait(10);
    Engine_AudioPlayCue(152);
    Engine_ActorSetSpeed(0, 0x33333, 0x19999);
    v3 = rec7->y.fixed - rec8->y.fixed;
    if ((rec7->y.fixed - rec8->y.fixed) < 0) {
        v3 = rec8->y.fixed - rec7->y.fixed;
    }
    {
        s32 speed = 0x80;

        asm("lsl %0, %0, #11" : "+l"(speed)); /* FAKEMATCH: builds the base speed after the height step */
        rec8->velocity_y = ((v3 >> 14) << 14) + speed;
    }
    Engine_ActorSetAnimation(0, 7);
    Engine_ObjectSetPosition(rec8, rec7->x.fixed, rec7->y.fixed, rec7->z.fixed);
    Engine_TaskWait(10);
    rec8->sprite->priority = 3;
    Engine_ActorWaitForMove(0);
    for (;;) {
        if (!((rec7->y.fixed >> 14) < (rec8->y.fixed >> 14))) break;
        Engine_TaskWait(1);
    }
    Engine_EventEnd();
    Engine_AudioPlayCue(159);
    ArutinYama_RunRollingObject(a0, 0);
    Engine_TaskWait(20);
    BattleFx_PlayQueuedSound();
}
