#include "VILLAGE.H"

/* Where the party appears in the village and at the fortress gate. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura1) {
        return gLunpaEntrances;
    }
    if (scene == (s32)&SceneId_RunpaMura2) {
        return gGateEntrances;
    }
    return gLunpaEntrances;
}
