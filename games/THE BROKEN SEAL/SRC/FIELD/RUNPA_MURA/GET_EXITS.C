#include "VILLAGE.H"

/* Where the village's and the gate's exits lead. */
const u32 *Scene_GetExits(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura1) {
        return gLunpaExits;
    }
    if (scene == (s32)&SceneId_RunpaMura2) {
        return gGateExits;
    }
    return gLunpaExits;
}
