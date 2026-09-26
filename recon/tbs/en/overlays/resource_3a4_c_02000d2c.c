/* NONMATCHING: H2 408/404 bytes, 152 differing halfwords / 69 aligned edits.
 * Phase-local loops remove the increment spill: frame 4 now matches, as do
 * all four pool words. The compiler rotates each loop, keeps actor in r8
 * instead of sl, and rematerialises the sine half-turn instead of using fp.
 * Retained for the corrected frame; H1 in 0f0536a63 has 63 aligned edits.
 * STOP: complete model plus two structural follow-ups exhausted; no adoption.
 * H1 404/404 bytes, 125 differing halfwords / 63 aligned edits.
 * The unsigned whole-angle halfword view restores both shifts inside the
 * settle loop and x in r9; increment sharing still adds one stack word.
 * H1 does not match: the fourth pool word and several register lifetimes differ.
 * H0 408/404 bytes, 141 differing halfwords / 89 aligned edits (2026-09-26).
 * Complete own-ROM extent 02000d2c..02000ec0, including four pool words.
 * FIELD_EVENT.H and exact FieldScene_RunSharedSetPiece establish actor y at
 * +0x0c and sprite rotation at +0x1e. The old draft called y "z" and declared
 * ActorGet as an integer-returning unprototyped service.
 * Baseline 408/404 bytes, 141 halfword edits (2026-09-24).
 * H0 keeps an 8-byte frame instead of 4: CSE carries the fall increment
 * into settle, and GCSE carries its whole-angle value around the back edge.
 * Calls all resolve correctly; typed views alone do not change this shape.
 * Budget: complete typed model and at most two structural follow-ups. */
#include "FIELD_EVENT.H"

void FieldScene_RunSharedSetPiece(s32 delay);

union SwingAngle {
    u32 fixed;
    struct {
        u16 fraction;
        u16 whole;
    } part;
};

void ArutinYama_SwingActorIntoSetPiece(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 x;
    s32 y;
    u32 angle;
    u32 limit;
    u32 half;
    s32 c;
    s32 s;

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
    {
        union SwingAngle acc;

        acc.fixed = 0;
        limit = 0x8fff;
        for (;;) {
            acc.fixed += 0x80000;
            sprite->rotation += acc.part.whole;
            c = Math_Cos(sprite->rotation + 0x4000);
            actor->x.fixed = (c << 4) + x;
            angle = sprite->rotation;
            if (angle > limit)
                break;
            Task_Wait(1);
        }
    }
    {
        union SwingAngle acc;

        acc.fixed = 0;
        limit = 0x7000;
        for (;;) {
            acc.fixed += 0x80000;
            sprite->rotation = angle - acc.part.whole;
            c = Math_Cos(sprite->rotation + 0x4000);
            actor->x.fixed = (c << 4) + x;
            angle = sprite->rotation;
            if (angle <= limit)
                break;
            Task_Wait(1);
            angle = sprite->rotation;
        }
    }
    {
        union SwingAngle acc;

        half = 0x8000;
        acc.fixed = 0x80000;
        for (;;) {
            acc.fixed = (acc.part.whole + (acc.fixed >> 19)) << 16;
            limit = acc.part.whole;
            sprite->rotation = limit + angle;
            c = Math_Cos(sprite->rotation + 0x4000);
            s = Math_Sin(sprite->rotation + half);
            actor->x.fixed = (c << 4) + x;
            if (sprite->rotation > half)
                actor->y.fixed = y - (s << 3);
            if ((s32)(sprite->rotation + limit) > 0xbfff)
                break;
            Task_Wait(1);
            angle = sprite->rotation;
        }
    }
    Task_Wait(1);
    {
        s32 shown = 0xc000;

        sprite->rotation = shown;
    }
    Audio_PlayCue(183);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    Event_Wait(20);
    Audio_PlayCue(0x121);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    FieldScene_RunSharedSetPiece(5);
    Event_End();
}
