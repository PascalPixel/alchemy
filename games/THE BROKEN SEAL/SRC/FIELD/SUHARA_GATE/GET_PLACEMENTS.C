#include "GATE.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gSuharaGatePlacements1[];
extern const struct ScenePlacement gSuharaGatePlacements1Flagged[];
extern const struct ScenePlacement gSuharaGatePlacements2[];
extern const struct ScenePlacement gSuharaGatePlacementsOther[];

/* The actors placed at the gate; the first row places others once flag
 * 0x96f is set. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SuharaGate2) {
        return gSuharaGatePlacements2;
    }
    if (scene == (s32)&SceneId_SuharaGate1) {
        if (GameFlag_IsSet(0x96f) != 0) {
            return gSuharaGatePlacements1Flagged;
        }
        return gSuharaGatePlacements1;
    }
    return gSuharaGatePlacementsOther;
}
