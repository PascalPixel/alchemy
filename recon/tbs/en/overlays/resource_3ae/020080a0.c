/* Draft of resource_3ae 0x020080a0 (SceneData_SelectDataBySelectorAndFlags), built with
 * games/THE BROKEN SEAL/SRC/FIELD/COMMON/KAREI_TOREBI/KAREI.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x6b and 0x70 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "KAREI.H"

extern u8 Value_0000006b;
extern u8 Value_00000070;
extern u8 Value_0000006c;
extern u8 Data_020098d4[];
extern u8 Data_020098ec[];
extern u8 Data_020099c4[];
extern u8 Data_02009acc[];
extern u8 Data_02009ba4[];
extern u8 Data_02009c7c[];
extern u8 Data_02009d24[];
extern u8 Data_02009dcc[];

s32 SceneData_SelectDataBySelectorAndFlags(void)
{
    s16 room = gGameState.scene;

    if (room == (s32)&Value_0000006b) {
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_02009ba4;
        }
        return (s32)Data_02009acc;
    }

    if (room == (s32)&Value_00000070) {
        /* 0x950 is built from an immediate and a shift. */
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_020099c4;
        }
        return (s32)Data_020098ec;
    }

    if (room == (s32)&Value_0000006c) {
        if (GameFlag_IsSet(0x950) != 0) {
            return (s32)Data_02009dcc;
        }
        if (GameFlag_IsSet(0x93e) != 0) {
            return (s32)Data_02009d24;
        }
        return (s32)Data_02009c7c;
    }

    return (s32)Data_020098d4;
}
