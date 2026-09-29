#include "GATE.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gSuharaGateEntrances2[];
extern const struct SceneEntrance gSuharaGateEntrances3[];
extern const struct SceneEntrance gSuharaGateEntrancesOther[];

/* Where the party appears on each side of the gate. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SuharaGate2) {
        return gSuharaGateEntrances2;
    }
    if (scene == (s32)&SceneId_SuharaGate3) {
        return gSuharaGateEntrances3;
    }
    return gSuharaGateEntrancesOther;
}
