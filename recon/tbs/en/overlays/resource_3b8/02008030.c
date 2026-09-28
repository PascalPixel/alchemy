/* resource_3b8:02008030..02008060 (48 bytes with pool), still linked from
 * the listing. Remaining difference: the game compares the scene with 0x8b
 * loaded from the literal pool (ldr r3, =0x8b; cmp r2, r3), as a link-time
 * value would be; an integer scene compares with an immediate. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H. */

s32 SceneData_SelectTablec614ByState(void)
{
    if (gGameState.scene == 0x8b) {
        return (s32)Data_0200ca1c;
    }
    return (s32)Data_0200c614;
}
