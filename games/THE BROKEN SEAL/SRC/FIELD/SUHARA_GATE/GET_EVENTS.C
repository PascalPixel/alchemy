#include "GATE.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gSuharaGateEvents2[];
extern const struct SceneEvent gSuharaGateEvents3[];
extern const struct SceneEvent gSuharaGateEventsOther[];

/* What each side of the gate answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_SuharaGate2) {
        return gSuharaGateEvents2;
    }
    if (scene == (s32)&SceneId_SuharaGate3) {
        return gSuharaGateEvents3;
    }
    return gSuharaGateEventsOther;
}
