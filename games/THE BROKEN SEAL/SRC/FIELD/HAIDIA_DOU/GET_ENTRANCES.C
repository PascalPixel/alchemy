#include "HAIDIA.H"

extern const struct SceneEntrance gHaidiaDouEntrances1[];
extern const struct SceneEntrance gHaidiaDouEntrances2[];
extern const struct SceneEntrance gHaidiaDouEntrances3[];
extern const struct SceneEntrance gHaidiaDouEntrancesOther[];

/* Where the party appears in each of the sanctum's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouEntrances1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouEntrances2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouEntrances3;
    }
    return gHaidiaDouEntrancesOther;
}
