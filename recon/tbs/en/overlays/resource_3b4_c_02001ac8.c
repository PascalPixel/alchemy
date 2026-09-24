/* NONMATCHING: 152 of 152 bytes, 16 halfword edits (2026-09-24). Hand-written from the
 * resolved jump-table disassembly as a single-overlay unit binding Engine_* at
 * their import veneers. Remaining: 37 halfwords: reference keeps the motion-flag zero in r6 from before the column tests, and loads height before width for Engine_MapCopyCellAttributes */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneState_ApplyFourRectsAndSetActor8Byte85(void);
void SceneState_ApplyRectAndSetSlotEightByte35(void);
void ActorPresentation_SetSceneCell58AndMarkActorEight(void);
void SceneState_ApplyRectAndSetActor8Byte35(void);

void Local_02001ac8(void)
{
    struct FieldActor *block = Engine_ActorGet(8);
    s32 x = block->x.fixed;
    s32 y = block->y.fixed;
    s32 column = x / 0x100000;
    u8 still;

    if (y == 0)
        Engine_ActorGet(8)->priority_flags = 2;
    SceneState_ApplyFourRectsAndSetActor8Byte85();
    Engine_ActorGet(8)->motion_flags = 3;
    still = 0;
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
        Engine_ActorGet(8)->motion_flags = still;
        Engine_ActorGet(8)->y.fixed = 0x200000;
    }
}
