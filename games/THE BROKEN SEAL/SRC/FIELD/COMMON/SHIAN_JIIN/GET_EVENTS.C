#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gShianJiinEvents1[];
extern const struct SceneEvent gShianJiinEventsEntrance3[];
extern const struct SceneEvent gShianJiinEvents[];

/* What the temple answers: the first scene and the third entrance have
   their own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_ShianJiin1) {
        return gShianJiinEvents1;
    }
    if (gGameState.entrance == 3) {
        return gShianJiinEventsEntrance3;
    }
    return gShianJiinEvents;
}
