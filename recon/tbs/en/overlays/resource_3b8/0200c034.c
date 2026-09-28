/* resource_3b8:0200c034..0200c0b4 (128 bytes), still linked from the
 * listing. Remaining difference: the scene is compared with 0x8b loaded from
 * the literal pool, a link-time value; an integer compares with an
 * immediate (120 bytes, 65 differ). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/TOREBI_KYUDEN/KYUDEN.H. */

s32 SceneData_SelectTableD004ByStateAndFlags(void)
{
    if (gGameState.scene == 0x8b) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_0200dad8;
        }
        if (GameFlag_IsSet(0x962) != 0) {
            return (s32)Data_0200da48;
        }
        return (s32)Data_0200d9e8;
    }
    if (GameFlag_IsSet(0x950) != 0) {
        return (s32)Data_0200d688;
    }
    if (GameFlag_IsSet(0x962) != 0) {
        return (s32)Data_0200d394;
    }
    return (s32)Data_0200d004;
}
