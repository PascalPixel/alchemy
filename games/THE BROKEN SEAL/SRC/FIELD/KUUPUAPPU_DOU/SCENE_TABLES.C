#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const u32 gKuupuappuDouExits1[];
extern const u32 gKuupuappuDouExits2[];
extern const u32 gKuupuappuDouExits3[];
extern const u32 gKuupuappuDouExitsOther[];
extern const struct ScenePlacement gKuupuappuDouPlacements1[];
extern const struct ScenePlacement gKuupuappuDouPlacements2[];
extern const struct ScenePlacement gKuupuappuDouPlacements3[];
extern const struct ScenePlacement gKuupuappuDouPlacementsOther[];

/* Where each area's exits lead. */
const u32 *Scene_GetExits(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouExits1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouExits2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouExits3;
    }
    return gKuupuappuDouExitsOther;
}

/* The actors placed in each area. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KuupuappuDou1) {
        return gKuupuappuDouPlacements1;
    }
    if (scene == (s32)&SceneId_KuupuappuDou2) {
        return gKuupuappuDouPlacements2;
    }
    if (scene == (s32)&SceneId_KuupuappuDou3) {
        return gKuupuappuDouPlacements3;
    }
    return gKuupuappuDouPlacementsOther;
}
