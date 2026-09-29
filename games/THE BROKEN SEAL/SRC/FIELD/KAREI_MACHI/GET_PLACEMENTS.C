#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gKareiMachiPlacements1[];
extern const struct ScenePlacement gKareiMachiPlacements2[];
extern const struct ScenePlacement gKareiMachiPlacements3[];
extern const struct ScenePlacement gKareiMachiPlacements6[];
extern const struct ScenePlacement gKareiMachiPlacementsOther[];

/* The actors placed in the scene. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiMachi1) {
        return gKareiMachiPlacements1;
    }
    if (scene == (s32)&SceneId_KareiMachi2) {
        return gKareiMachiPlacements2;
    }
    if (scene == (s32)&SceneId_KareiMachi3) {
        return gKareiMachiPlacements3;
    }
    if (scene == (s32)&SceneId_KareiMachi6) {
        return gKareiMachiPlacements6;
    }
    return gKareiMachiPlacementsOther;
}
