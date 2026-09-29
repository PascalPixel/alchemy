#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gKorimaMuraEvents[];
extern const struct SceneEvent gKorimaMuraEvents3[];

/* What Kori answers; the third scene has its own events. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_KorimaMura3) {
        return gKorimaMuraEvents3;
    }
    return gKorimaMuraEvents;
}
