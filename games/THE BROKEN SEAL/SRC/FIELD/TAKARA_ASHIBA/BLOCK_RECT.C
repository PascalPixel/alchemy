#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"

void SceneState_ApplyRectAndClearSlotTenByte85(void);
void SceneState_ApplyRectAndClearActor10Byte85(void);
void SceneState_ApplyTwoRectsAtRow56(void);

/* Tracks block 10: once it has sunk to cell height 2 or below, applies its
 * rectangle and sets flag 0x300; on row 55 applies the rectangle for columns
 * 42 and 38, and anywhere else the two rectangles at row 56. */
void TakaraAshiba_UpdateBlockRects(void)
{
    struct FieldActor *block = Engine_ActorGet(10);
    s32 y = block->y.fixed / 0x100000;
    s32 x = block->x.fixed / 0x100000;
    s32 z = block->z.fixed / 0x100000;

    if (!Value1(Engine_GameFlagIsSet, 0x300) && y <= 2) {
        SceneState_ApplyRectAndClearSlotTenByte85();
        Engine_GameFlagSet(0x300);
    }
    if (z == 55) {
        if (x == 42) {
            SceneState_ApplyRectAndClearActor10Byte85();
        }
        if (x == 38) {
            SceneState_ApplyRectAndClearSlotTenByte85();
        }
    } else {
        SceneState_ApplyTwoRectsAtRow56();
    }
}
