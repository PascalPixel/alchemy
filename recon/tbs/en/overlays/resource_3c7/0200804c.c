/* Draft of resource_3c7 0x0200804c..0x0200807c (48 bytes with pool),
 * the regions selector; the listing keeps the rows. Remaining difference:
 * the reference compares the scene with the sanctum's id 0xb3 loaded from
 * its literal pool, a link-time value; the integer scene is an immediate
 * (40 bytes, 20 differ from +0x2). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RARIBERO_HEYA/HEYA.H"

s32 SceneData_SelectTableByWord224(void)
{
    if (gGameState.scene == 0xb3) {
        return (s32)gRariberoSanctumRegions;
    }
    return (s32)gRariberoRegions;
}
