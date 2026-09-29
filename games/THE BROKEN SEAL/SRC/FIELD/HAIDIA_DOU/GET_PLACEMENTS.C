#include "HAIDIA.H"

extern const struct ScenePlacement gHaidiaDouPlacements1[];
extern const struct ScenePlacement gHaidiaDouPlacements2[];
extern const struct ScenePlacement gHaidiaDouPlacements3[];

/* The actors placed in each of the sanctum's three areas; any other scene
   takes the first area's. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouPlacements1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouPlacements2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouPlacements3;
    }
    return gHaidiaDouPlacements1;
}
