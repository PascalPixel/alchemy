/* Draft of SceneData_SelectDataByRuntimeSelector, resource_3c0 at 0x0200834c,
 * built with games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SABAKU.H.
 * Remaining difference: the ROM loads the scene numbers 0xa4-0xa6 from its
 * literal pool, as link-time values; GCC compares immediates.
 * The listing keeps these rows. */
#include "SABAKU.H"

extern u8 Data_02009488[];
extern u8 Data_020094d0[];
extern u8 Data_02009548[];
extern u8 Data_02009458[];

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == 0xa4) {
        return (s32)Data_02009488;
    }
    if (selector == 0xa5) {
        return (s32)Data_020094d0;
    }
    if (selector == 0xa6) {
        return (s32)Data_02009548;
    }
    return (s32)Data_02009458;
}
