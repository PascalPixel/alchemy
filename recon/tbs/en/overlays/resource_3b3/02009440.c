/* resource_3b3 0x02009440 Scene_GetEntrances (as SceneData_SelectTableBySceneId),
 * written against FIELD/TAKARA_HASHIRA/HASHIRA.H; compiles exactly.
 * Remaining difference: it compares the scene number with 0x73..0x7a loaded
 * from its literal pool, which only link-time names explain; the main image
 * names none of them. */
#include "HASHIRA.H"
extern u8 Value_00000073;
extern u8 Value_00000074;
extern u8 Value_00000077;
extern u8 Value_00000079;
extern u8 Value_0000007a;
extern u8 Data_0200aeac[];
extern u8 Data_0200aef4[];
extern u8 Data_0200af3c[];
extern u8 Data_0200af84[];
extern u8 Data_0200afcc[];
extern u8 Data_0200ae7c[];
extern u8 Data_0200b06c[];
extern u8 Data_0200b0e4[];
extern u8 Data_0200b174[];
extern u8 Data_0200b2dc[];
extern u8 Data_0200b264[];
extern u8 Data_0200b3a8[];
extern u8 Data_0200b438[];
extern u8 Data_0200b498[];
extern u8 Data_0200b51c[];
extern u8 Data_0200b618[];
extern u8 Data_0200b39c[];


/* Contiguous unnamed leaf-owner run for resource_3b3. */
s32 SceneData_SelectTableBySceneId(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200aeac;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200aef4;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200af3c;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)Data_0200af84;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200afcc;
    }
    return (s32)Data_0200ae7c;
}
