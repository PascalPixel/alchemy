#include "FORTRESS.H"

extern const struct SceneEvent gRunpaJoEvents1[];
extern const struct SceneEvent gRunpaJoEvents2[];
extern const struct SceneEvent gRunpaJoEvents3[];
extern const struct SceneEvent gRunpaJoEventsOther[];

/* What each floor of the fortress answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaJo1) {
        return gRunpaJoEvents1;
    }
    if (scene == (s32)&SceneId_RunpaJo2) {
        return gRunpaJoEvents2;
    }
    if (scene == (s32)&SceneId_RunpaJo3) {
        return gRunpaJoEvents3;
    }
    return gRunpaJoEventsOther;
}
