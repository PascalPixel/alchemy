/* Draft of resource_3ab 0x02008350..0x02008388 (56 bytes with pool),
 * Scene_GetExits; the listing keeps the rows. Remaining difference: the
 * reference loads the scene numbers 0x68 and 0x9f from its literal pool and
 * compares registers, as a link-time scene symbol does; the constants compile
 * to cmp with an immediate, 12 bytes shorter. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"

const u32 *Scene_GetExits(void)
{
    s32 scene = gGameState.scene;

    if (scene == SCENE_RUNPA_MURA) {
        return gLunpaExits;
    }
    if (scene == SCENE_RUNPA_JO_GATE) {
        return gGateExits;
    }
    return gLunpaExits;
}
