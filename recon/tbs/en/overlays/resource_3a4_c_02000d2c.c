/* NONMATCHING: H1 404/404 bytes, 125 differing halfwords / 63 aligned edits.
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

void ArutinYama_SwingActorIntoSetPiece(void)
{
    struct FieldActor *actor;
    struct FieldSprite *sprite;
    s32 x;
    s32 y;
    union {
        u32 fixed;
        struct {
            u16 fraction;
            u16 whole;
        } part;
    } acc;
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
    acc.fixed = 0;
    limit = 0x8fff;
rise:
    acc.fixed += 0x80000;
    sprite->rotation += acc.part.whole;
    c = Math_Cos(sprite->rotation + 0x4000);
    actor->x.fixed = (c << 4) + x;
    angle = sprite->rotation;
    if (angle <= limit) {
        Task_Wait(1);
        goto rise;
    }
    acc.fixed = 0;
    limit = 0x7000;
fall:
    acc.fixed += 0x80000;
    sprite->rotation = angle - (acc.part.whole);
    c = Math_Cos(sprite->rotation + 0x4000);
    actor->x.fixed = (c << 4) + x;
    angle = sprite->rotation;
    if (angle > limit) {
        Task_Wait(1);
        angle = sprite->rotation;
        goto fall;
    }
    half = 0x8000;
    acc.fixed = 0x80000;
settle:
    acc.fixed = ((acc.part.whole) + (acc.fixed >> 19)) << 16;
    limit = acc.part.whole;
    sprite->rotation = limit + angle;
    c = Math_Cos(sprite->rotation + 0x4000);
    s = Math_Sin(sprite->rotation + half);
    actor->x.fixed = (c << 4) + x;
    if (sprite->rotation > half) {
        actor->y.fixed = y - (s << 3);
    }
    if ((s32)(sprite->rotation + limit) <= 0xbfff) {
        Task_Wait(1);
        angle = sprite->rotation;
        goto settle;
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
