/* H1 rejected (2026-09-27): literal Half zero from MAKYURI_CHOJO/LAMP.C
 * emits 160/152 bytes, 55 differing halfwords / 27 aligned edits. CSE keeps
 * an HI zero pseudo, but it is loaded only at the final byte store; no r6
 * lifetime is admitted. An extra zero pool and moved division-mask pool
 * add two branch/pool islands. Unlike LAMP there is no earlier HI zero
 * producer to share. Map size-argument order is unchanged. Whole normalized
 * diff read; -da assembly equals normal output. No new DONE bytes.
 * NONMATCHING: 152 of 152 bytes, 16 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 37 halfwords: reference keeps the motion-flag zero in r6 from before the column tests, and loads height before width for Engine_MapCopyCellAttributes.
 * 2026-09-26: widening still from u8 to s32 did not change either residual;
 * retain the original width and the verified whole-owner bindings. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_ApplyFourRectsAndSetActor8Byte85(void);
void SceneState_ApplyRectAndSetSlotEightByte35(void);
void ActorPresentation_SetSceneCell58AndMarkActorEight(void);
void SceneState_ApplyRectAndSetActor8Byte35(void);

struct Half {
    u16 value;
};

void Local_02001ac8(void)
{
    struct FieldActor *block = Engine_ActorGet(8);
    s32 x = block->x.fixed;
    s32 y = block->y.fixed;
    s32 column = x / 0x100000;
    struct Half still;

    if (y == 0)
        Engine_ActorGet(8)->priority_flags = 2;
    SceneState_ApplyFourRectsAndSetActor8Byte85();
    Engine_ActorGet(8)->motion_flags = 3;
    /* FAKEMATCH: retain the literal zero's halfword producer across calls. */
    still.value = 0;
    if (column == 40) {
        SceneState_ApplyRectAndSetSlotEightByte35();
    } else if (column == 42) {
        ActorPresentation_SetSceneCell58AndMarkActorEight();
    } else if (column == 41) {
        SceneState_ApplyRectAndSetActor8Byte35();
    } else {
        if (column == 39)
            goto copy;
        if (column == 38)
            goto copy;
        if (column != 37)
            return;
    copy:
        Engine_MapCopyCellAttributes(61, 36, 1, 1, column, 42);
        Engine_ActorGet(8)->motion_flags = still.value;
        Engine_ActorGet(8)->y.fixed = 0x200000;
    }
}
