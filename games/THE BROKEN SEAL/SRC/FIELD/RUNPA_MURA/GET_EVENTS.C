#include "VILLAGE.H"

/* What the village answers before and after Lunpa trades again, and what
 * the gate answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura2) {
        /* The gate asks whether Lunpa trades again but answers alike either way. */
        GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED);
        return gGateEvents;
    }
    if (scene == (s32)&SceneId_RunpaMura1 && GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        return gLunpaReopenedEvents;
    }
    return gLunpaSealedEvents;
}
