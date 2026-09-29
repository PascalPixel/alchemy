#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gTorebiKyudenPlacements2[];
extern const struct ScenePlacement gTorebiKyudenPlacementsAfterColosso[];
extern const struct ScenePlacement gTorebiKyudenPlacementsColosso[];
extern const struct ScenePlacement gTorebiKyudenPlacementsOther[];

/* The actors placed in the palace: the second scene has its own; the
   others change once Colosso is under way (flag 0x962) and once it is over
   (flag 0x950). */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_TorebiKyuden2) {
        return gTorebiKyudenPlacements2;
    }
    if (GameFlag_IsSet(0x950) != 0) {
        return gTorebiKyudenPlacementsAfterColosso;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return gTorebiKyudenPlacementsColosso;
    }
    return gTorebiKyudenPlacementsOther;
}
