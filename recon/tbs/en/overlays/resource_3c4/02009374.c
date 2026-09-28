/* Draft of resource_3c4 0x02009374 (FieldScene_RunLayoutAt93By30), built
 * with games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/BABI.H. Remaining
 * difference: the ROM sets the first argument (r0) of the six-argument
 * cell-attribute copy before r1-r3; this C sets it last. The listing keeps
 * these rows. */
#include "BABI.H"

/*
 * Layout step in resource_3c4 for slots 10 and 11. One six-argument
 * placement, then two grid-cell pins built from each slot's +8 and +16 words
 * shifted right by 20.
 *
 * Several call sites reach the same routine, but each keeps its own call
 * word; the sites must not be collapsed onto one alias.
 */
void FieldScene_RunLayoutAt93By30(void)
{
    Event_Begin();
    {
        s32 width = 29;
        s32 height = 30;

        Map_CopyCellAttributes(93, 30, 6, 5, width, height);
    }
    StagedActor_AdvancePair();
    FieldScene_PlaceAndPinSlots10And11();
    Event_End();
}
