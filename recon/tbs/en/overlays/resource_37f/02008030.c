/* Draft of resource_37f 0x, from
 * games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SCENARIO_DISPATCH.C.
 * Remaining difference: the ROM loads scene or message numbers from the
 * literal pool, as link-time symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "SORU.H"

s32 SceneData_SelectOverlayDataBySelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000013) {
        return (s32)Data_02009d04;
    }
    if (v == (s32)&Value_00000010) {
        return (s32)Data_02009d64;
    }
    return (s32)Data_02009cd4;
}

