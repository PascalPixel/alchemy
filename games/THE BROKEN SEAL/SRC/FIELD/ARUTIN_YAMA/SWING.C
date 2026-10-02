/* The tree that is pushed over to bridge the gap. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"

void FieldScene_RunSharedSetPiece(s32 delay);

/*
 * Actor 10 topples: it leans over faster and faster until it passes the
 * tipping point, rocks back, then falls for good, sinking as it goes down,
 * and comes to rest lying flat. Each phase turns the sprite a little more
 * every frame and swings the actor sideways with the turn. The shared set
 * piece follows.
 */
void ArutinYama_SwingActorIntoSetPiece(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 x;
    s32 y;
    s32 c;
    s32 s;
    u16 step;

    actor = Actor_Get(10);
    sprite = actor->sprite;
    x = actor->x.fixed;
    y = actor->y.fixed;
    Event_Begin();
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x20000, 0x10000, 0x10000);
    Event_Wait(10);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    Event_Wait(20);

    /* FAKEMATCH: each phase leaves by goto; a break lets GCC 2.96 roll the phase's update and test below its wait. */
    step = 0;
    for (;;) {
        step += 8;
        sprite->rotation += step;
        c = Math_Cos(sprite->rotation + 0x4000);
        actor->x.fixed = (c << 4) + x;
        if (sprite->rotation > 0x8fff) {
            goto risen;
        }
        Task_Wait(1);
    }
risen:
    step = 0;
    for (;;) {
        step += 8;
        sprite->rotation -= step;
        c = Math_Cos(sprite->rotation + 0x4000);
        actor->x.fixed = (c << 4) + x;
        if (sprite->rotation <= 0x7000) {
            goto fallen;
        }
        Task_Wait(1);
    }
fallen:
    step = 8;
    for (;;) {
        step += step >> 3;
        sprite->rotation += step;
        c = Math_Cos(sprite->rotation + 0x4000);
        s = Math_Sin(sprite->rotation + 0x8000);
        actor->x.fixed = (c << 4) + x;
        if (sprite->rotation > 0x8000) {
            actor->y.fixed = y - (s << 3);
        }
        if (sprite->rotation + step > 0xbfff) {
            goto settled;
        }
        Task_Wait(1);
    }
settled:
    Task_Wait(1);
    sprite->rotation = 0xc000;
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    FieldScene_RunSharedSetPiece(5);
    Event_End();
}
