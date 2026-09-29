#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct SceneEntrance gTorebiKyudenEntrances2[];
extern const struct SceneEntrance gTorebiKyudenEntrancesOther[];

/* Where the party appears in the palace; the second scene has its own. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiKyuden2) {
        return gTorebiKyudenEntrances2;
    }
    return gTorebiKyudenEntrancesOther;
}
