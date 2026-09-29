#include "HASHIRA.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gTakaraHashiraPlacements1[];
extern const struct ScenePlacement gTakaraHashiraPlacements2[];
extern const struct ScenePlacement gTakaraHashiraPlacements3[];
extern const struct ScenePlacement gTakaraHashiraPlacements5[];
extern const struct ScenePlacement gTakaraHashiraPlacementsOther[];

/* The actors placed in the pillar rooms; the fourth room takes the others'. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraHashira1) {
        return gTakaraHashiraPlacements1;
    }
    if (scene == (s32)&SceneId_TakaraHashira2) {
        return gTakaraHashiraPlacements2;
    }
    if (scene == (s32)&SceneId_TakaraHashira3) {
        return gTakaraHashiraPlacements3;
    }
    if (scene == (s32)&SceneId_TakaraHashira5) {
        return gTakaraHashiraPlacements5;
    }
    return gTakaraHashiraPlacementsOther;
}
