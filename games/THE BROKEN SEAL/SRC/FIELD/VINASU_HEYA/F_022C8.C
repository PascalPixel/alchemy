#include "TYPES.H"
#include "FIELD_EVENT.H"

extern s32 Data_0200d148[];

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void Call4(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3)
{
    f(a0, a1, a2, a3);
}

/* Moves actors 8 and 9 to the heights their table rows name, then marks the
 * cell under each of actors 8 to 12 that has sunk below the floor. */
void VinasuHeya_Func020022c8(s32 wait)
{
    struct FieldActor *a = Engine_ActorGet(8);
    struct FieldActor *b = Engine_ActorGet(9);
    u32 i;

    Call3((void (*)())Engine_ActorSetSpeed, 8, 0x8000, 0x4000);
    Call3((void (*)())Engine_ActorSetSpeed, 9, 0x8000, 0x4000);
    if (wait != 0) {
        Engine_AudioPlayCue(180);
    }
    Call4((void (*)())Engine_ObjectSetPosition, (s32)a, a->x.fixed, Data_0200d148[(s16)a->unknown_64], a->z.fixed);
    Call4((void (*)())Engine_ObjectSetPosition, (s32)b, b->x.fixed, Data_0200d148[(s16)b->unknown_64], b->z.fixed);
    Engine_ActorWaitForMove(8);
    Engine_ActorWaitForMove(9);
    a->y.fixed = Data_0200d148[(s16)a->unknown_64];
    b->y.fixed = Data_0200d148[(s16)b->unknown_64];
    if (wait != 0) {
        Engine_AudioPlayCue(0x121);
    }
    for (i = 0; i < 5; i++) {
        struct FieldActor *actor = Engine_ActorGet(i + 8);

        if (actor->y.fixed / 0x10000 < 0 && actor->y.fixed / 0x10000 > -30) {
            Call6((void (*)())Engine_MapCopyCellAttributes, 4, 19, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
        }
    }
    Engine_EventWait(wait);
}
