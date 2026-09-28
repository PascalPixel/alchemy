/* Draft of resource_3ab 0x020086e4..0x02008738 (84 bytes with pool),
 * Scene_GetEvents; the listing keeps the rows. Remaining difference: the
 * reference loads the scene numbers 0x9f and 0x68 from its literal pool, as a
 * link-time scene symbol does; the constants compile to cmp with an
 * immediate, 12 bytes shorter. */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/RUNPA_MURA/VILLAGE.H"

const struct SceneEvent *Scene_GetEvents(void)
{
    s32 scene = gGameState.scene;

    if (scene == SCENE_RUNPA_JO_GATE) {
        /* The gate asks whether Lunpa trades again but answers alike either way. */
        GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED);
        return gGateEvents;
    }
    if (scene == SCENE_RUNPA_MURA && GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        return gLunpaReopenedEvents;
    }
    return gLunpaSealedEvents;
}
