#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "KANPAN.H"

union Slot {
    s32 w;
    s16 h[2];
};

extern u8 LinkedMessage_TheresNothingWeCanDo[];

s32 BuildMotionCountdown(s32, s16);

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

void SceneState_ApplyFiveRectsAtColumn78(void)
{
    Map_CopyCellsTo(78, 39, 78, 40, 5, 1);
    Map_CopyCellsTo(78, 39, 78, 41, 5, 1);
    Map_CopyCellsTo(78, 39, 79, 42, 4, 1);
    Map_CopyCellsTo(78, 39, 82, 43, 1, 1);
    {
        s32 x = 17;
        s32 y = 40;

        Map_CopyCellAttributes(17, 38, 5, 2, x, y);
    }
}

void DialogueLayout_ConfigureTwoRegions(void)
{
    Map_CopyCellsTo(66, 61, 64, 40, 5, 4);
    Map_CopyCellAttributes(0, 0, 5, 4, 5, 39);
}

void FieldScene_RunStepThen10(s32 a)
{
    Event_ShowMessage(a, 0);
    Event_Wait(10);
}

void FieldScene_CallPairWith10(s32 a, s32 b)
{
    Actor_FaceDirection(a, b, 10);
}
