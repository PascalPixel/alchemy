/* Draft of resource_37f 0x, from
 * games/THE BROKEN SEAL/SRC/FIELD/SORU_IRIGUCHI/SCENARIO_DISPATCH.C.
 * Remaining difference: the ROM loads scene or message numbers from the
 * literal pool, as link-time symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "SORU.H"

s32 FieldScene_DispatchByScenarioId(void)
{
    s32 scenario = gGameState.scene;

    if (scenario == (s32)&Value_00000013) {
        FieldScene_RunScene37f_0200092c();
    } else if (scenario == (s32)&Value_00000010) {
        FieldScene_RunSceneEntryHook();
    }
    return 0;
}

