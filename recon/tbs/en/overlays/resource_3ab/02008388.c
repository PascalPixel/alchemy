/* Draft of resource_3ab 0x02008388..0x020083c0 (56 bytes with pool),
 * Scene_GetPlacements; the listing keeps the rows. Remaining difference: the
 * reference loads the scene numbers 0x68 and 0x9f from its literal pool and
 * compares registers, as a link-time scene symbol does; the constants compile
 * to cmp with an immediate, 12 bytes shorter. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"

const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == SCENE_RUNPA_MURA) {
        return gLunpaPlacements;
    }
    if (scene == SCENE_RUNPA_JO_GATE) {
        return gGatePlacements;
    }
    return gLunpaPlacements;
}
