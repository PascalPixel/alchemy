#include "HAIDIA.H"

extern const struct SceneEvent gHaidiaDouEvents1[];
extern const struct SceneEvent gHaidiaDouEvents2[];
extern const struct SceneEvent gHaidiaDouEvents3[];
extern const struct SceneEvent gHaidiaDouEventsOther[];

/* What each of the sanctum's three areas answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_HaidiaDou1) {
        return gHaidiaDouEvents1;
    }
    if (scene == (s32)&SceneId_HaidiaDou2) {
        return gHaidiaDouEvents2;
    }
    if (scene == (s32)&SceneId_HaidiaDou3) {
        return gHaidiaDouEvents3;
    }
    return gHaidiaDouEventsOther;
}
