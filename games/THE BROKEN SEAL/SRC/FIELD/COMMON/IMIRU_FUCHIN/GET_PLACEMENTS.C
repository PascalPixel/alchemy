#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gImiruFuchinPlacements1[];
extern const struct ScenePlacement gImiruFuchinPlacements2[];
extern const struct ScenePlacement gImiruFuchinPlacements3[];
extern const struct ScenePlacement gImiruFuchinPlacements4[];
extern const struct ScenePlacement gImiruFuchinPlacements5[];
extern const struct ScenePlacement gImiruFuchinPlacements7[];
extern const struct ScenePlacement gImiruFuchinPlacementsOther[];

/* The actors placed in each area; the sixth area places the table the other scenes take. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ImiruFuchin1) {
        return gImiruFuchinPlacements1;
    }
    if (v == (s32)&SceneId_ImiruFuchin2) {
        return gImiruFuchinPlacements2;
    }
    if (v == (s32)&SceneId_ImiruFuchin3) {
        return gImiruFuchinPlacements3;
    }
    if (v == (s32)&SceneId_ImiruFuchin4) {
        return gImiruFuchinPlacements4;
    }
    if (v == (s32)&SceneId_ImiruFuchin5) {
        return gImiruFuchinPlacements5;
    }
    if (v == (s32)&SceneId_ImiruFuchin7) {
        return gImiruFuchinPlacements7;
    }
    return gImiruFuchinPlacementsOther;
}
