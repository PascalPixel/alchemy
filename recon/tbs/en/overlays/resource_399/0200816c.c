/* Draft of resource_399 0x0200816c (SceneData_SelectScriptByScene33AndFlag881), built with
 * games/THE BROKEN SEAL/SRC/FIELD/IMIRU_MURA/IMIRU.H.
 * Remaining difference: none in its bytes, but the ROM loads scene number 0x33 from the literal pool as a link-time value, and no source defines it.
 * The listing keeps these rows. */
#include "IMIRU.H"

extern u8 Value_00000033;

/*
 * The overlay image is writable, so the chosen block is patched in place. The
 * coordinates are written as shifts, which is how a 16.16 whole number is
 * built here. The word at +0x4c is set only on this path and never read back,
 * so its meaning is unverified.
 */
u8 *SceneData_SelectScriptByScene33AndFlag881(void)
{
    u8 *script;

    if (gGameState.scene == ((s32)&Value_00000033)) {
        script = Data_0200aad0;
        Func_020023c6(script);
        if (GameFlag_IsSet(0x881) != 0) {
            script[262] = 0;
            *(s32 *)(script + 0x50) = 182 << 16;
            *(s32 *)(script + 0x58) = 564 << 16;
            *(s32 *)(script + 0x4c) = 2;
        }
        return script;
    }

    if (GameFlag_IsSet(0x881) != 0) {
        return Data_0200aa58;
    }
    return Data_0200a9e0;
}
