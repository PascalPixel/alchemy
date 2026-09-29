#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 gKareiKyudenPlacements[];
extern const struct ScenePlacement gKareiKyudenPlacementsOther[];

void FieldScene_PrepareActors(u8 *placements);

/* The actors placed in the palace, prepared before they are returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_KareiKyuden) {
        FieldScene_PrepareActors(gKareiKyudenPlacements);
        return (const struct ScenePlacement *)gKareiKyudenPlacements;
    }
    return gKareiKyudenPlacementsOther;
}
