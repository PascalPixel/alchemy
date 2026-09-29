#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gBiribinoMuraEntrances1[];
extern const struct SceneEntrance gBiribinoMuraEntrances2[];
extern const struct SceneEntrance gBiribinoMuraEntrances3[];
extern const struct SceneEntrance gBiribinoMuraEntrancesOther[];

/* Where the party appears in each of Bilibin's three scenes. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BiribinoMura1) {
        return gBiribinoMuraEntrances1;
    }
    if (scene == (s32)&SceneId_BiribinoMura3) {
        return gBiribinoMuraEntrances3;
    }
    if (scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraEntrances2;
    }
    return gBiribinoMuraEntrancesOther;
}

extern const struct SceneRegion gBiribinoMuraRegions2[];

/* Only the second scene has map regions. */
const struct SceneRegion *Scene_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraRegions2;
    }
    return 0;
}
