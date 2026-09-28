/* Draft of resource_3c4 0x02009270 (SceneData_SelectDataByRuntimeSelector), built with
 * games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/BABI.H.
 * Remaining difference: the ROM loads scene number 0xad from the literal
 * pool, as a link-time value would; the C constant is built with movs.
 * The listing keeps these rows. */
#include "BABI.H"

/* Return this overlay's state block. */
s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_000000ac) {
        return (s32)Data_0200b474;
    }
    if (v == (s32)&Value_000000ad) {
        return (s32)Data_0200b654;
    }
    return (s32)Data_0200b42c;
}
