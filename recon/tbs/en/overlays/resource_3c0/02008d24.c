/* NONMATCHING: resource_3c0 0x02008d24 (164 bytes with its pool),
 * FieldScene_RunOpeningAuxiliarySequence, before
 * FIELD/SUHARA_SABAKU/ENCOUNTER.C, stays listing.
 *
 * Its scene is SceneId_SuharaSabaku2 and the level it restores is the
 * linked gSuharaSabakuShownLevel. Remaining difference: 2 halfwords in the
 * third cell copy. The game sets r1 first and r0 last (r1, r2, r3, r0); the
 * value-call wrapper below gives r3, r2, r1, r0, and a plain call r3, r2,
 * r0, r1. The forced temporary before the palette write is needed for the
 * game's load order.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SABAKU.H"

s32 FieldScene_RunOpeningAuxiliarySequence(void)
{
    s32 record;

    if (gGameState.scene == (s32)&SceneId_SuharaSabaku2) {
        Engine_ActorGet(14)->priority_flags = 2;
        Engine_ActorGet(14)->motion_flags = 3;
        Actor_SetPosition(14, 0, 0);
        Map_CopyCellAttributes(16, 44, 1, 1, 15, 44);
        MapObject_SetPosition(100, 0, 0);
        Map_CopyCellAttributes(12, 71, 1, 1, 127, 127);
        Value6(Engine_MapCopyCellAttributes, 11, 71, 1, 1, 12, 71);
        record = Task_RemoveCallback(EncounterPalette_Pulse);
        {
            s32 shown = gSuharaSabakuShownLevel;

            EncounterPalette = shown;
        }
        return record;
    }
}
