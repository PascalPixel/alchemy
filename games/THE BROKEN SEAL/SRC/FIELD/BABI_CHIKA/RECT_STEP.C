/* Copy the cell attributes at (73, 38) to (9, 38), advance the staged pair
   and place and pin slots 8 and 9. */
#include "BABI.H"

void FieldScene_PlaceAndPinSlots8And9(void);

void SceneState_RunRect73x38Step(void)
{
    Event_Begin();
    Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots8And9();
    Event_End();
}
