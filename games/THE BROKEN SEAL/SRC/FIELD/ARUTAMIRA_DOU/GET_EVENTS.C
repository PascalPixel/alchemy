#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gArutamiraDouEvents2[];
extern const struct SceneEvent gArutamiraDouEvents3[];
extern const struct SceneEvent gArutamiraDouEvents4[];
extern const struct SceneEvent gArutamiraDouEvents5[];
extern const struct SceneEvent gArutamiraDouEvents6[];
extern const struct SceneEvent gArutamiraDouEventsOther[];

/* What the scene answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ArutamiraDou2) {
        return gArutamiraDouEvents2;
    }
    if (scene == (s32)&SceneId_ArutamiraDou3) {
        return gArutamiraDouEvents3;
    }
    if (scene == (s32)&SceneId_ArutamiraDou4) {
        return gArutamiraDouEvents4;
    }
    if (scene == (s32)&SceneId_ArutamiraDou5) {
        return gArutamiraDouEvents5;
    }
    if (scene == (s32)&SceneId_ArutamiraDou6) {
        return gArutamiraDouEvents6;
    }
    return gArutamiraDouEventsOther;
}
