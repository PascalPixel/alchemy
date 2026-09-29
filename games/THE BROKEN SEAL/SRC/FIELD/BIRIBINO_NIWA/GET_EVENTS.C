#include "NIWA.H"

extern const struct SceneEvent gBiribinoNiwaEvents[];
extern const struct SceneEvent gBiribinoNiwaEventsOther[];

/* What the garden answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        return gBiribinoNiwaEvents;
    }
    return gBiribinoNiwaEventsOther;
}
