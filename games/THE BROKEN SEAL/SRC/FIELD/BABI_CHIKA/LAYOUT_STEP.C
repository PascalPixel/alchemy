/* Copy the cell attributes at (93, 30) to (29, 30), advance the staged pair
   and place and pin slots 10 and 11. */
#include "BABI.H"

void FieldScene_PlaceAndPinSlots10And11(void);

void FieldScene_RunLayoutAt93By30(void)
{
    Event_Begin();
    Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots10And11();
    Event_End();
}
