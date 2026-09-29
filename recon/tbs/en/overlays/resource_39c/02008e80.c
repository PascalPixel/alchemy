/* Draft of SceneData_SelectTableByWord224, resource_39c at 0x02008e80 (split from FIELD/MAKYURI_HEYA/FIELD_PROBE_SCENE.C).
 * Remaining difference: it compares the scene with 0x36..0x39, which the game loads from its literal pool as link-time values; an integer compares with an immediate. Its tables are not yet labelled. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

extern s16 Data_02000240[];
extern s16 Data_02000240_t[][1];
extern u8 *Data_03001ebc;
extern u8 Value_00000036;
extern u8 Value_00000037;
extern u8 Value_00000038;
extern u8 Value_00000039;
extern u8 Data_00000036[];

s32 SceneData_SelectTableByWord224(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000036) {
        return (s32)Data_0200e700;
    }
    if (v == (s32)&Value_00000037) {
        return (s32)Data_0200e7a8;
    }
    if (v == (s32)&Value_00000038) {
        return (s32)Data_0200e838;
    }
    if (v == (s32)&Value_00000039) {
        return (s32)Data_0200e988;
    }
    return (s32)Data_0200e6e8;
}
