/* resource_39e:0200834c..0200837c (48 bytes with pool), still linked from
 * the listing. Remaining difference: the scene test loads 0x3c from the
 * literal pool, a link-time value; an integer scene compares with an
 * immediate (40 bytes, 20 differ). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
/* Declarations and helpers: games/THE BROKEN SEAL/SRC/FIELD/COMMON/SHIAN_JIIN/TEMPLE.H. */

s32 GetXianScriptData(void)
{
    if (gGameState.scene == 0x3c) {
        return (s32)Data_0200c7a8;
    }
    return (s32)Data_0200c838;
}
