/* Draft of resource_3a5 0x020083ac, from
 * games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SELECTED_ACTOR_SCENE_ID_SELECTED_ACTOR_SCENE.C.
 * Remaining difference: the ROM loads desert scenes 0x59-0x5c from the
 * literal pool, as link-time scene symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "RAMAKAN.H"

s32 SceneData_SelectTableByScene59To5c(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000059) {
        return (s32)Data_0200a174;
    }
    if (v == (s32)&Value_0000005a) {
        return (s32)Data_0200a1d4;
    }
    if (v == (s32)&Value_0000005b) {
        return (s32)Data_0200a234;
    }
    if (v == (s32)&Value_0000005c) {
        return (s32)Data_0200a2dc;
    }
    return (s32)Data_0200a12c;
}

