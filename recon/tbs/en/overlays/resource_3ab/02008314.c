/* Draft of resource_3ab 0x02008314..0x0200834c (56 bytes with pool),
 * Scene_GetEntrances; the listing keeps the rows. Remaining difference: the
 * reference loads the scene numbers 0x68 and 0x9f from its literal pool and
 * compares registers, as a link-time scene symbol does; the constants
 * SCENE_RUNPA_MURA and SCENE_RUNPA_JO_GATE compile to cmp with an immediate,
 * 12 bytes shorter. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"

const struct SceneEntrance *Scene_GetEntrances(void)
{
    s32 scene = gGameState.scene;

    if (scene == SCENE_RUNPA_MURA) {
        return gLunpaEntrances;
    }
    if (scene == SCENE_RUNPA_JO_GATE) {
        return gGateEntrances;
    }
    return gLunpaEntrances;
}
