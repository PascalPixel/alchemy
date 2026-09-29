#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gSuharaSabakuEvents3[];
extern const struct SceneEvent gSuharaSabakuEventsOther[];

/* The third area answers its own events; the others share one table. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_SuharaSabaku3) {
        return gSuharaSabakuEvents3;
    }
    return gSuharaSabakuEventsOther;
}
