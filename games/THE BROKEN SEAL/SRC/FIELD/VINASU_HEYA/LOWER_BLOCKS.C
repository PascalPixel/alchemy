#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

extern s32 gVinasuBlockHeights[];

/* Moves actors 8 and 9 to the heights their table rows name, then marks the
 * cell under each of actors 8 to 12 that has sunk below the floor. */
void VinasuHeya_LowerFloatingBlocks(s32 wait)
{
    struct FieldActor *a = Engine_ActorGet(8);
    struct FieldActor *b = Engine_ActorGet(9);
    u32 i;

    Call3((void (*)())Engine_ActorSetSpeed, 8, 0x8000, 0x4000);
    ((void (*)())Engine_ActorSetSpeed)(9, 0x8000, 0x4000);
    if (wait != 0) {
        Engine_AudioPlayCue(180);
    }
    Call4((void (*)())Engine_ObjectSetPosition, (s32)a, a->x.fixed, gVinasuBlockHeights[(s16)a->unknown_64], a->z.fixed);
    Call4((void (*)())Engine_ObjectSetPosition, (s32)b, b->x.fixed, gVinasuBlockHeights[(s16)b->unknown_64], b->z.fixed);
    Engine_ActorWaitForMove(8);
    Engine_ActorWaitForMove(9);
    a->y.fixed = gVinasuBlockHeights[(s16)a->unknown_64];
    b->y.fixed = gVinasuBlockHeights[(s16)b->unknown_64];
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
