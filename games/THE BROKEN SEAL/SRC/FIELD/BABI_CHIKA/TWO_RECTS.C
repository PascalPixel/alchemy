/* Copy two cell-attribute rectangles to column 25, advance the staged pair
   and run the scene that follows. */
#include "BABI.H"

void FieldScene_RunScene3c4_02002480(void);

void SceneState_ApplyTwoRectsAndRunThree(void)
{
    Event_Begin();
    Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
    StagedActor_AdvancePair();
    FieldScene_RunScene3c4_02002480();
    Event_End();
}
