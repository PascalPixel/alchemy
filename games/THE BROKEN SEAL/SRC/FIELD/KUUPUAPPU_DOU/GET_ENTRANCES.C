#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gKuupuappuDouEntrances1[];
extern const struct SceneEntrance gKuupuappuDouEntrances2[];
extern const struct SceneEntrance gKuupuappuDouEntrances3[];
extern const struct SceneEntrance gKuupuappuDouEntrancesOther[];

/* Where the party appears in each of the cave's three areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouEntrances1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouEntrances2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouEntrances3;
    }
    return gKuupuappuDouEntrancesOther;
}
