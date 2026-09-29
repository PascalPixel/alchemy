#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEvent gKareiKyudenEvents[];
extern const struct SceneEvent gKareiKyudenEventsOther[];

/* What the scene answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_KareiKyuden) {
        return gKareiKyudenEvents;
    }
    return gKareiKyudenEventsOther;
}
