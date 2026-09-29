#include "HEYA.H"

/* Both interiors answer differently once the aerie's events are done. */
u8 *RariberoHeya_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_RariberoHeya2) {
        if (GameFlag_IsSet(0x9a7) != 0) {
            return gRariberoHeyaEvents9a7;
        }
        return gRariberoHeyaEvents;
    }
    if (GameFlag_IsSet(0x9a7) != 0) {
        return gRariberoEvents9a7;
    }
    return gRariberoEvents;
}
