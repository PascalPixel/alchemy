/* Draft of SceneData_SelectOverlayDataByRuntimeSelector, resource_3c0 at
 * 0x020083ac, built with games/THE BROKEN SEAL/SRC/FIELD/SUHARA_SABAKU/SABAKU.H.
 * Remaining difference: the ROM loads the scene numbers 0xa4-0xa6 from its
 * literal pool, as link-time values; GCC compares immediates.
 * The listing keeps these rows. */
#include "SABAKU.H"

extern u8 Data_02009610[];
extern u8 Data_020096b8[];
extern u8 Data_02009790[];
extern u8 Data_020095f8[];

s32 SceneData_SelectOverlayDataByRuntimeSelector(void)
{
    s16 selector = gGameState.scene;

    if (selector == 0xa4) {
        return (s32)Data_02009610;
    }
    if (selector == 0xa5) {
        return (s32)Data_020096b8;
    }
    if (selector == 0xa6) {
        return (s32)Data_02009790;
    }
    return (s32)Data_020095f8;
}
