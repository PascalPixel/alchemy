/* Draft of resource_3c7 0x02008084..0x020080c8 (68 bytes with pool),
 * the placements selector; the listing keeps the rows. Remaining difference:
 * the reference compares the scene with the house's id 0xb4 loaded from its
 * literal pool, a link-time value; the integer scene is an immediate (64
 * bytes, 33 differ from +0x5). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/HEYA.H"

u8 *SceneData_SelectTableBySceneIdAndFlag9a7(void)
{
    if (gGameState.scene == 0xb4) {
        if (GameFlag_IsSet(0x9A7) != 0) {
            return gRariberoHeyaPlacements9a7;
        }
        return gRariberoHeyaPlacements;
    }
    return gRariberoPlacements;
}
