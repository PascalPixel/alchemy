#include "HEYA.H"

/* The sanctum has regions of its own; the house takes the town's. */
u8 *RariberoHeya_GetRegions(void)
{
    if (gGameState.scene == (s32)&SceneId_RariberoHeya1) {
        return gRariberoSanctumRegions;
    }
    return gRariberoRegions;
}
