#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gKuupuappuDouEvents1[];
extern const struct SceneEvent gKuupuappuDouEvents2[];
extern const struct SceneEvent gKuupuappuDouEvents3[];
extern const struct SceneEvent gKuupuappuDouEventsOther[];

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouEvents1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouEvents2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouEvents3;
    }
    return gKuupuappuDouEventsOther;
}
