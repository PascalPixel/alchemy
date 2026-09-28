/* resource_39e:0200a574..0200a5b8 (68 bytes with pool), still linked from
 * the listing. Remaining difference: the scene test loads 0x3c from the
 * literal pool, a link-time value; an integer scene compares with an
 * immediate (64 bytes, 40 differ). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/TEMPLE.H. */

s32 FieldScene_SelectData(void)
{
    if (gGameState.scene == 0x3c) {
        return (s32)Data_0200cb90;
    }
    if (gGameState.entrance == 3) {
        return (s32)Data_0200d184;
    }
    return (s32)Data_0200cd40;
}
