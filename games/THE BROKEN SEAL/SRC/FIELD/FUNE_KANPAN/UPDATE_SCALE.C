#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 Math_RemainderUnsigned(s32, s16);

/* Briefly stretches the sprite, then waits before the next pulse. */
s32 SceneActor_UpdateScalePulse(struct FieldActor *actor)
{
    /* FAKEMATCH: In-place signed accesses preserve the countdown's load
     * and address scheduling. */
    switch (*(s16 *)&actor->unknown_64) {
    case 6:
        actor->scale_x += -0x4000;
        actor->scale_y += 0x2000;
        break;
    case 4:
        actor->scale_x += 0x2000;
        /* The loader relocates the stored pool word to -0x1000. */
        actor->scale_y -= 0x1000;
        break;
    case 2:
        actor->scale_x += 0x1000;
        actor->scale_y += -0x800;
        break;
    case 0:
        actor->scale_x = 0x10000;
        actor->scale_y = 0x10000;
        (*(s16 *)&actor->unknown_64) =
            (s16)(Math_RemainderUnsigned(Random_Next(), 90) + 60);
        break;
    }
    (*(s16 *)&actor->unknown_64)--;
    return 1;
}

s32 SceneState_ApplyArgMode1AndReturnZero(s32 a)
{
    Actor_SetSpriteFlags(a, 1);
    return 0;
}
