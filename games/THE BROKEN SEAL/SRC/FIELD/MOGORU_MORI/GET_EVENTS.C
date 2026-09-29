#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gMogoruMoriEvents1[];
extern const struct SceneEvent gMogoruMoriEvents2[];
extern const struct SceneEvent gMogoruMoriEvents3[];
extern const struct SceneEvent gMogoruMoriEventsOther[];

/* What each area answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 selector = gGameState.scene;

    if (selector == (s32)&SceneId_MogoruMori1) {
        return gMogoruMoriEvents1;
    }
    if (selector == (s32)&SceneId_MogoruMori2) {
        return gMogoruMoriEvents2;
    }
    if (selector == (s32)&SceneId_MogoruMori3) {
        return gMogoruMoriEvents3;
    }
    return gMogoruMoriEventsOther;
}
