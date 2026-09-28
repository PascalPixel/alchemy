/* Draft of resource_3c7 0x020084b0..0x02008508 (88 bytes with pool),
 * the events selector; the listing keeps the rows. Remaining difference: the
 * reference compares the scene with the house's id 0xb4 loaded from its
 * literal pool, a link-time value; the integer scene is an immediate (80
 * bytes, 43 differ from +0x2). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/HEYA.H"

u8 *SceneData_SelectScriptBySceneIdAndFlag9a7(void)
{
    s32 GameFlag_IsSet();

    if (gGameState.scene == 0xb4) {
        if (GameFlag_IsSet(0x9a7) != 0) {
            return gRariberoHeyaEvents9a7;
        }
        return gRariberoHeyaEvents;
    }
    if (GameFlag_IsSet(0x9a7) != 0) {
        return gRariberoEvents9a7;
    }
    return gRariberoEvents;
}
