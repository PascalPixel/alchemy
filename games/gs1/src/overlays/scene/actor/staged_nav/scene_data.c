#include "types.h"

#define SceneData_SelectTableBySceneId Func_02001440
#define SceneData_ReturnZero Func_020014b8
#define SceneData_GetTableb014 Func_020014bc
#define SceneData_SelectDataByRuntimeSelector Func_020014c4
#define SceneData_SelectSecondaryTableBySceneId Func_02002580
extern s16 Data_02000240[];
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
    s16 v = Data_02000240[224];

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

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *SceneData_GetTableb014(void)
{
    return (u8 *)0x0200b014;
}

s32 SceneData_SelectDataByRuntimeSelector(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b06c;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b0e4;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b174;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b2dc;
    }
    return (s32)Data_0200b264;
}

s32 SceneData_SelectSecondaryTableBySceneId(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000073) {
        return (s32)Data_0200b3a8;
    }
    if (v == (s32)&Value_00000074) {
        return (s32)Data_0200b438;
    }
    if (v == (s32)&Value_00000077) {
        return (s32)Data_0200b498;
    }
    if (v == (s32)&Value_00000079) {
        return (s32)Data_0200b51c;
    }
    if (v == (s32)&Value_0000007a) {
        return (s32)Data_0200b618;
    }
    return (s32)Data_0200b39c;
}
