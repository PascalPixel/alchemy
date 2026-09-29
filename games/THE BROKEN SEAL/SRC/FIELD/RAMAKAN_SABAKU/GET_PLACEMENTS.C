#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern const struct ScenePlacement gRamakanSabakuPlacements1[];
extern const struct ScenePlacement gRamakanSabakuPlacements2[];
extern const struct ScenePlacement gRamakanSabakuPlacements3[];
extern const struct ScenePlacement gRamakanSabakuPlacementsOther[];

/* The actors placed in each of the desert's areas. Entering the third area
   by its fifth entrance sets flag 0x90a first. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
        if (gGameState.entrance == 5) {
            GameFlag_Set(0x90a);
        }
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku1) {
        return gRamakanSabakuPlacements1;
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku2) {
        return gRamakanSabakuPlacements2;
    }
    if (gGameState.scene == (s32)&SceneId_RamakanSabaku3) {
        return gRamakanSabakuPlacements3;
    }
    return gRamakanSabakuPlacementsOther;
}
