/* Draft of resource_3c5 0x02008fac..0x02008fd4 (40 bytes with pool), the
 * regions selector; the listing keeps the rows. Remaining difference: the
 * reference compares the scene with 0xb0 loaded from its literal pool, a
 * link-time value; the integer scene is an immediate. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/BABI_IRIGUCHI/IRIGUCHI.H"

s32 SceneData_SelectTableB5b8ByState(void)
{
    if (gGameState.scene == 0xb0) {
        return (s32)gBabiIriguchiRegions;
    }
    return 0;
}
