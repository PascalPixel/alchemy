#include "TYPES.H"
#include "SCENE_IDS.H"
#include "FIELD_EVENT.H"

void Engine_MapCopyCellAttributes();
void DialogueLayout_ConfigureGroupOne();
void DialogueLayout_ConfigureGroupTwo();
void DialogueLayout_ConfigureGroupThree();
void FieldScene_RunFlagBranchedLayoutSteps();

extern u8 Data_02000240[];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void ImiruFuchin_ApplyRoomLayout(void)
{
    u32 i;
    s32 record;

    if (gGameState.scene == (s32)&SceneId_ImiruFuchin2) {
        Call6(Engine_MapCopyCellAttributes, 8, 29, 15, 5, 8, 42);
        DialogueLayout_ConfigureGroupOne();
    } else {
        if (gGameState.scene == (s32)&SceneId_ImiruFuchin3) {
            Call6(Engine_MapCopyCellAttributes, 12, 8, 10, 18, 0, 28);
            DialogueLayout_ConfigureGroupTwo();
        } else {
            if (gGameState.scene == (s32)&SceneId_ImiruFuchin4) {
                if (gGameState.entrance == 1) {
                    goto L_020019c6;
                }
                Call6(Engine_MapCopyCellAttributes, 12, 21, 9, 16, 12, 3);
                DialogueLayout_ConfigureGroupThree();
            } else {
                L_020019c6:;
                if (gGameState.scene == (s32)&SceneId_ImiruFuchin5) {
                    if (gGameState.entrance == 1 || gGameState.entrance == 2) {
                        Call6(Engine_MapCopyCellAttributes, 14, 10, 9, 8, 22, 20);
                    } else {
                        Call6(Engine_MapCopyCellAttributes, 7, 45, 11, 4, 20, 45);
                    }
                    FieldScene_RunFlagBranchedLayoutSteps();
                }
            }
        }
    }
}
