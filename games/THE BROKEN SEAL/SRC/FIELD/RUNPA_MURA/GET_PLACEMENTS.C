#include "VILLAGE.H"

/* The actors standing in the village or at the gate. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura1) {
        return gLunpaPlacements;
    }
    if (scene == (s32)&SceneId_RunpaMura2) {
        return gGatePlacements;
    }
    return gLunpaPlacements;
}
