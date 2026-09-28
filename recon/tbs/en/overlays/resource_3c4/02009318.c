/* Draft of resource_3c4 0x02009318 (SceneState_RunRect73x38Step), built
 * with games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/BABI.H. Remaining
 * difference: the ROM sets the first argument (r0) of the six-argument
 * cell-attribute copy before r1-r3; this C sets it last. The listing keeps
 * these rows. */
#include "BABI.H"

void SceneState_RunRect73x38Step(void)
{
    Event_Begin();
    {
        s32 width = 9;
        s32 height = 38;

        Map_CopyCellAttributes(73, 38, 5, 5, width, height);
    }
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots8And9();
    Event_End();
}
