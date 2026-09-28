/* Draft of resource_3ad 0x02008044..0x02008074 (48 bytes with pool),
 * Scene_GetPlacements; the listing keeps the rows. Remaining difference: the
 * reference compares the scene with the cave's own id 0x6a loaded from its
 * literal pool, a link-time value; the integer scene is an immediate (40
 * bytes, 20 differ from +0x2). */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/CAVE.H"

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == 0x6a) {
        return gCavePlacements;
    }
    return gCaveNoPlacements;
}
