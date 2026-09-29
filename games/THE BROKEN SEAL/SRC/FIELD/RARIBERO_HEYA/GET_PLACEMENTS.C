#include "HEYA.H"

/* The house places its actors anew once the aerie's events are done. */
u8 *RariberoHeya_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_RariberoHeya2) {
        if (GameFlag_IsSet(0x9A7) != 0) {
            return gRariberoHeyaPlacements9a7;
        }
        return gRariberoHeyaPlacements;
    }
    return gRariberoPlacements;
}
