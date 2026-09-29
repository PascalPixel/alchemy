#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gGomaSuiroEvents2[];
extern const struct SceneEvent gGomaSuiroEventsOther[];

/* What the waterway answers; the second area has its own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_GomaSuiro2) {
        return gGomaSuiroEvents2;
    }
    return gGomaSuiroEventsOther;
}
