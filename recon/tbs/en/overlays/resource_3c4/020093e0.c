/* Draft of resource_3c4 0x020093e0 (SceneState_ApplyTwoRectsAndRunThree), built
 * with games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/BABI.H. Remaining
 * difference: the ROM sets the first argument (r0) of the six-argument
 * cell-attribute copy before r1-r3; this C sets it last. The listing keeps
 * these rows. */
#include "BABI.H"

void SceneState_ApplyTwoRectsAndRunThree(void)
{
    s32 lead = 25;

    Event_Begin();
    Map_CopyCellAttributes(89, 49, 3, 2, lead, 49);
    Map_CopyCellAttributes(89, 51, 8, 5, lead, 51);
    StagedActor_AdvancePair();
    FieldScene_RunScene3c4_02002480();
    Event_End();
}
