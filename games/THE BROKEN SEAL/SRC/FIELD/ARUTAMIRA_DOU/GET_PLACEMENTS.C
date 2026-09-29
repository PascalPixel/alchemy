#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gArutamiraDouPlacements2[];
extern const struct ScenePlacement gArutamiraDouPlacements4[];
extern const struct ScenePlacement gArutamiraDouPlacements6[];
extern const struct ScenePlacement gArutamiraDouPlacementsOther[];

/* The actors placed in the scene. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutamiraDou2) {
        return gArutamiraDouPlacements2;
    }
    if (scene == (s32)&SceneId_ArutamiraDou4) {
        return gArutamiraDouPlacements4;
    }
    if (scene == (s32)&SceneId_ArutamiraDou6) {
        return gArutamiraDouPlacements6;
    }
    return gArutamiraDouPlacementsOther;
}
