#include "TYPES.H"
#define FIELD_STAGED_ACTOR_IMPORTS
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gBabiChikaEntrances1[];
extern const struct SceneEntrance gBabiChikaEntrances2[];
extern const struct SceneEntrance gBabiChikaEntrancesOther[];
extern const struct SceneRegion gBabiChikaRegions2[];

/* Where the party appears in each of the tunnel's two areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_BabiChika1) {
        return gBabiChikaEntrances1;
    }
    if (v == (s32)&SceneId_BabiChika2) {
        return gBabiChikaEntrances2;
    }
    return gBabiChikaEntrancesOther;
}

/* Only the second area has map regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BabiChika2) {
        return gBabiChikaRegions2;
    }
    return 0;
}
