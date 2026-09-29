#include "FORTRESS.H"

extern const struct SceneEntrance gRunpaJoEntrances1[];
extern const struct SceneEntrance gRunpaJoEntrances2[];
extern const struct SceneEntrance gRunpaJoEntrances3[];
extern const struct SceneEntrance gRunpaJoEntrancesOther[];

/* Where the party appears on each floor of the fortress. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaJo1) {
        return gRunpaJoEntrances1;
    }
    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoEntrances2;
    }
    if (scene == (s32)&SceneId_RunpaJo3) {
        return gRunpaJoEntrances3;
    }
    return gRunpaJoEntrancesOther;
}
