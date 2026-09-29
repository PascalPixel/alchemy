#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gShianJiinEntrances1[];
extern const struct SceneEntrance gShianJiinEntrancesOther[];

/* Where the party appears in the scene it enters. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_ShianJiin1) {
        return gShianJiinEntrances1;
    }
    return gShianJiinEntrancesOther;
}
