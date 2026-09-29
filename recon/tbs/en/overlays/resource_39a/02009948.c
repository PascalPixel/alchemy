/* NONMATCHING: resource_39a at 0x02009948 (236 bytes with its pool),
 * ImiruFuchin_ApplyRoomLayout, after the entry setup, stays listing. It was
 * FIELD/COMMON/IMIRU_FUCHIN/ROOM_LAYOUT.C.
 *
 * Remaining difference: the reference loads the scene numbers 0x3e, 0x3f,
 * 0x40 and 0x41 from its literal pool and compares registers, as link-time
 * scene numbers do; plain constants compile to cmp with an immediate. With
 * the four numbers as link-time values this body compiles to the reference
 * exactly.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

void DialogueLayout_ConfigureGroupOne(void);
void DialogueLayout_ConfigureGroupTwo(void);
void DialogueLayout_ConfigureGroupThree(void);
void FieldScene_RunFlagBranchedLayoutSteps(void);

/* Copy the room's cell attributes for the way it is entered, then run the
 * room's layout step. */
void ImiruFuchin_ApplyRoomLayout(void)
{
    if (gGameState.scene == 0x3e) {
        Map_CopyCellAttributes(8, 29, 15, 5, 8, 42);
        DialogueLayout_ConfigureGroupOne();
    } else if (gGameState.scene == 0x3f) {
        Map_CopyCellAttributes(12, 8, 10, 18, 0, 28);
        DialogueLayout_ConfigureGroupTwo();
    } else if (gGameState.scene == 0x40 && gGameState.entrance != 1) {
        Map_CopyCellAttributes(12, 21, 9, 16, 12, 3);
        DialogueLayout_ConfigureGroupThree();
    } else if (gGameState.scene == 0x41) {
        if (gGameState.entrance == 1 || gGameState.entrance == 2) {
            Map_CopyCellAttributes(14, 10, 9, 8, 22, 20);
        } else {
            Map_CopyCellAttributes(7, 45, 11, 4, 20, 45);
        }
        FieldScene_RunFlagBranchedLayoutSteps();
    }
}
