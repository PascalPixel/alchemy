#include "FORTRESS.H"

extern const struct ScenePlacement gRunpaJoPlacementsRunpaDou[];
extern const struct ScenePlacement gRunpaJoPlacements1[];
extern const struct ScenePlacement gRunpaJoPlacements2[];
extern const struct ScenePlacement gRunpaJoPlacements3[];
extern const struct ScenePlacement gRunpaJoPlacements4[];
extern const struct ScenePlacement gRunpaJoPlacementsOther[];

/* The actors placed on each floor of the fortress, and a table for the
 * Lunpa cave's row that this overlay never serves. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaDou) {
        return gRunpaJoPlacementsRunpaDou;
    }
    if (scene == (s32)&SceneId_RunpaJo3) {
        return gRunpaJoPlacements3;
    }
    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoPlacements2;
    }
    if (scene == (s32)&SceneId_RunpaJo1) {
        return gRunpaJoPlacements1;
    }
    if (scene == (s32)&SceneId_RunpaJo4) {
        return gRunpaJoPlacements4;
    }
    return gRunpaJoPlacementsOther;
}
