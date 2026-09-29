#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gImiruMuraEvents2[];
extern const struct SceneEvent gImiruMuraEventsOther[];

/* What Imil answers; the second area has its own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_ImiruMura2) {
        return gImiruMuraEvents2;
    }
    return gImiruMuraEventsOther;
}
