#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gBiribinoMuraEvents1[];
extern const struct SceneEvent gBiribinoMuraEvents2[];
extern const struct SceneEvent gBiribinoMuraEvents3[];
extern const struct SceneEvent gBiribinoMuraEventsOther[];

/* What each of Bilibin's three scenes answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_BiribinoMura1) {
        return gBiribinoMuraEvents1;
    }
    if (scene == (s32)&SceneId_BiribinoMura3) {
        return gBiribinoMuraEvents3;
    }
    if (scene == (s32)&SceneId_BiribinoMura2) {
        return gBiribinoMuraEvents2;
    }
    return gBiribinoMuraEventsOther;
}
