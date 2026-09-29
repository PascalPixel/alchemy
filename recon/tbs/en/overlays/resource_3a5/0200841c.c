/* Draft of resource_3a5 0x0200841c, from
 * games/THE BROKEN SEAL/SRC/FIELD/RAMAKAN_SABAKU/SELECTED_ACTOR_SCENE_ID_SELECTED_ACTOR_SCENE.C.
 * Remaining difference: the ROM loads desert scenes 0x59-0x5c from the
 * literal pool, as link-time scene symbols would; C builds those constants
 * with movs. The Value_/Data_ spellings below are the old address-named
 * forms. The listing keeps these rows. */
#include "RAMAKAN.H"

s32 SceneData_SelectTableBySceneId(void)
{
    if (gGameState.scene == (s32)&Value_0000005b) {
        if (gGameState.entrance == 5) {
            GameFlag_Set(0x90a);
        }
    }
    if (gGameState.scene == (s32)&Value_00000059) {
        return (s32)Data_0200a3c8;
    }
    if (gGameState.scene == (s32)&Value_0000005a) {
        return (s32)Data_0200a410;
    }
    if (gGameState.scene == (s32)&Value_0000005b) {
        return (s32)Data_0200a4b8;
    }
    return (s32)Data_0200a3b0;
}

