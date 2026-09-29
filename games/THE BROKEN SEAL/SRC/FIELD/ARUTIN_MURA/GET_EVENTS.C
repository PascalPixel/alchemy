#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gArutinMuraEvents1[];
extern const struct SceneEvent gArutinMuraEvents2[];
extern const struct SceneEvent gArutinMuraEventsOther[];

/* What each of Altin's two areas answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ArutinMura1) {
        return gArutinMuraEvents1;
    }
    if (v == (s32)&SceneId_ArutinMura2) {
        return gArutinMuraEvents2;
    }
    return gArutinMuraEventsOther;
}
