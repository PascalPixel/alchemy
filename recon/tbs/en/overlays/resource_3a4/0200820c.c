/* Draft of resource_3a4 0x0200820c (SceneData_SelectDataByRuntimeSelector), built with
 * games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_YAMA/YAMA.H.
 * Remaining difference: none in its bytes, but the ROM loads scene numbers 0x4d-0x57 from the literal pool as link-time values, and no source defines those values.
 * The listing keeps these rows. */
#include "YAMA.H"

extern u8 Value_0000004d;
extern u8 Data_0200c940[];
extern u8 Value_0000004f;
extern u8 Data_0200c9a0[];
extern u8 Value_00000051;
extern u8 Data_0200ca00[];
extern u8 Value_00000052;
extern u8 Data_0200ca60[];
extern u8 Value_00000053;
extern u8 Data_0200caa8[];
extern u8 Value_00000054;
extern u8 Data_0200cb68[];
extern u8 Value_00000055;
extern u8 Data_0200cb98[];
extern u8 Value_00000056;
extern u8 Data_0200cc40[];
extern u8 Value_00000057;
extern u8 Data_0200ccd0[];
extern u8 Data_0200c928[];

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_0000004d) {
        return (s32)Data_0200c940;
    }
    if (v == (s32)&Value_0000004f) {
        return (s32)Data_0200c9a0;
    }
    if (v == (s32)&Value_00000051) {
        return (s32)Data_0200ca00;
    }
    if (v == (s32)&Value_00000052) {
        return (s32)Data_0200ca60;
    }
    if (v == (s32)&Value_00000053) {
        return (s32)Data_0200caa8;
    }
    if (v == (s32)&Value_00000054) {
        return (s32)Data_0200cb68;
    }
    if (v == (s32)&Value_00000055) {
        return (s32)Data_0200cb98;
    }
    if (v == (s32)&Value_00000056) {
        return (s32)Data_0200cc40;
    }
    if (v == (s32)&Value_00000057) {
        return (s32)Data_0200ccd0;
    }
    return (s32)Data_0200c928;
}
