#include "IRIGUCHI.H"

extern const struct SceneEvent gBabiIriguchiEvents3[];
extern const struct SceneEvent gBabiIriguchiEvents2[];
extern const struct SceneEvent gBabiIriguchiEvents1[];
extern const struct SceneEvent gBabiIriguchiEventsOther[];

/* What each of the entrance's scenes answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BabiIriguchi3) {
        return gBabiIriguchiEvents3;
    }
    if (scene == (s32)&SceneId_BabiIriguchi2) {
        return gBabiIriguchiEvents2;
    }
    if (scene == (s32)&SceneId_BabiIriguchi1) {
        return gBabiIriguchiEvents1;
    }
    return gBabiIriguchiEventsOther;
}
