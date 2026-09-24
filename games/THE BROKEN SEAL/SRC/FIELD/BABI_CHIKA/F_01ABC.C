#include "TYPES.H"
#include "FIELD_EVENT.H"

/* The floor height of each step, by the actor's step index. */
extern s32 Data_0200b350[];

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves steps 10 and 11 to the floor heights their step indices name, then
 * marks the cell under each of actors 10 to 14 that has sunk below the
 * floor. */
void BabiChika_Func02001abc(s32 wait)
{
    u32 i;

    Call3((void (*)())Engine_ActorSetSpeed, 10, 0x8000, 0x4000);
    Call3((void (*)())Engine_ActorSetSpeed, 11, 0x8000, 0x4000);
    if (wait != 0) {
        Engine_AudioPlayCue(180);
    }
    Engine_ObjectSetPosition(Engine_ActorGet(10), Engine_ActorGet(10)->x.fixed,
                             Data_0200b350[(s16)Engine_ActorGet(10)->unknown_64], Engine_ActorGet(10)->z.fixed);
    Engine_ObjectSetPosition(Engine_ActorGet(11), Engine_ActorGet(11)->x.fixed,
                             Data_0200b350[(s16)Engine_ActorGet(11)->unknown_64], Engine_ActorGet(11)->z.fixed);
    Engine_ActorWaitForMove(10);
    Engine_ActorWaitForMove(11);
    Engine_ActorGet(10)->y.fixed = Data_0200b350[(s16)Engine_ActorGet(10)->unknown_64];
    Engine_ActorGet(11)->y.fixed = Data_0200b350[(s16)Engine_ActorGet(11)->unknown_64];
    if (wait != 0) {
        Engine_AudioPlayCue(0x121);
    }
    for (i = 0; i < 5; i++) {
        if (Engine_ActorGet(i + 10)->y.fixed / 0x10000 < 0 && Engine_ActorGet(i + 10)->y.fixed / 0x10000 > -30) {
            Engine_MapCopyCellAttributes(4, 9, 1, 1, Engine_ActorGet(i + 10)->x.fixed >> 20,
                                         Engine_ActorGet(i + 10)->z.fixed >> 20);
        }
    }
    Engine_EventWait(wait);
}
