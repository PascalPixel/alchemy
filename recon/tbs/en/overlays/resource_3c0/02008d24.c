/* Draft of resource_3c0 0x02008d24, built with
 * games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SABAKU.H.
 * Remaining difference: the ROM loads scene 0xa5 from its literal pool as a
 * link-time value, and it reads a halfword just past the loaded image
 * (0x02009a00), which the listing link does not place.
 * The listing keeps these rows. */
#include "SABAKU.H"

s32 FieldScene_RunOpeningAuxiliarySequence(s32 a0)
{
    u32 i;
    s32 record;

    if (gGameState.scene == 0xa5) {
        *(u8 *)(Engine_ActorGet(14) + 35) = 2;
        *(u8 *)(Engine_ActorGet(14) + 85) = 3;
        Actor_SetPosition(14, 0, 0);
        Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
        MapObject_SetPosition(100, 0, 0);
        Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
        Value6(Engine_MapCopyCellAttributes, 11, 71, 1, 1, 12, 71);
        record = Value1(Engine_TaskRemoveCallback, (s32)EncounterPalette_Pulse);
        do {
            s32 shown = gSuharaSabakuShownLevel;

            *(volatile u16 *)0x0500019e = shown;
        } while (0);
        return record;
    }
    return a0;
}
