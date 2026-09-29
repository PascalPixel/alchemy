#include "IMIRU_FUCHIN.H"

void DialogueLayout_ConfigureGroupOne(void);
void DialogueLayout_ConfigureGroupTwo(void);
void DialogueLayout_ConfigureGroupThree(void);
void FieldScene_RunFlagBranchedLayoutSteps(void);

/* Copy the room's cell attributes for the way it is entered, then run the
 * room's layout step. */
void ImiruFuchin_ApplyRoomLayout(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruFuchin2) {
        Map_CopyCellAttributes(8, 29, 15, 5, 8, 42);
        DialogueLayout_ConfigureGroupOne();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
        Map_CopyCellAttributes(12, 8, 10, 18, 0, 28);
        DialogueLayout_ConfigureGroupTwo();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin4 && gGameState.entrance != 1) {
        Map_CopyCellAttributes(12, 21, 9, 16, 12, 3);
        DialogueLayout_ConfigureGroupThree();
    } else if (gGameState.scene == (s32)&SceneId_ImiruFuchin5) {
        if (gGameState.entrance == 1 || gGameState.entrance == 2) {
            Map_CopyCellAttributes(14, 10, 9, 8, 22, 20);
        } else {
            Map_CopyCellAttributes(7, 45, 11, 4, 20, 45);
        }
        FieldScene_RunFlagBranchedLayoutSteps();
    }
}
