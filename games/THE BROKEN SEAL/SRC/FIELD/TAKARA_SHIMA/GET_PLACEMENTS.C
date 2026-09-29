#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gTakaraShimaPlacements1[];
extern const struct ScenePlacement gTakaraShimaPlacements3[];
extern const struct ScenePlacement gTakaraShimaPlacements6To14[];
extern const struct ScenePlacement gTakaraShimaPlacementsOther[];

/* The actors placed on the island: the first and third scenes have their
   own, the sixth to the fourteenth share one table. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&SceneId_TakaraShima1) {
        return gTakaraShimaPlacements1;
    }
    if (scene == (s32)&SceneId_TakaraShima3) {
        return gTakaraShimaPlacements3;
    }
    if (scene <= (s32)&SceneId_TakaraShima14 && scene >= (s32)&SceneId_TakaraShima6) {
        return gTakaraShimaPlacements6To14;
    }
    return gTakaraShimaPlacementsOther;
}
