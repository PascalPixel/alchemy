/* Draft of resource_3c4 0x020092b0 (SceneData_SelectTableB81cByWord224), built with
 * games/THE BROKEN SEAL/SRC/FIELD/BABI_CHIKA/BABI.H.
 * Remaining difference: the ROM loads scene number 0xad from the literal
 * pool, as a link-time value would; the C constant is built with movs.
 * The listing keeps these rows. */
#include "BABI.H"

s32 SceneData_SelectTableB81cByWord224(void)
{
    if (gGameState.scene == (s32)&Value_000000ad) {
        return (s32)Data_0200b81c;
    }
    return 0;
}
