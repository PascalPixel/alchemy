/* Draft of resource_3ae 0x0200819c (SceneData_SelectSecondaryDataBySelectorAndFlags), built with
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/KAREI.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x6b and 0x70 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "KAREI.H"

extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;
extern u8 Data_02009e74[];
extern u8 Data_0200a018[];
extern u8 Data_0200a120[];
extern u8 Data_02009e80[];
extern u8 Data_02009fa0[];
extern u8 Data_0200a24c[];
extern u8 Data_0200a30c[];
extern u8 Data_0200a390[];

s32 SceneData_SelectSecondaryDataBySelectorAndFlags(void)
{
    s16 scene = gGameState.scene;

    if (scene == (s32)&Value_0000006b) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_02009fa0;
        }
        return (s32)Data_02009e80;
    }

    if (scene == (s32)&Value_00000070) {
        /* 0x950 is built by shifting a small constant, not loaded. */
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_0200a120;
        }
        return (s32)Data_0200a018;
    }

    if (scene == (s32)&Value_0000006c) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_0200a390;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_0200a30c;
        }
        return (s32)Data_0200a24c;
    }

    return (s32)Data_02009e74;
}
