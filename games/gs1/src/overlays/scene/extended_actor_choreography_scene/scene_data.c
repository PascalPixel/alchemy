#include "types.h"

#define SceneData_SelectTableByWord224 Func_02000340
#define SceneData_ReturnZero Func_02000370
#define SceneData_GetTableEB94 Func_02000374
#define SceneData_SelectTableBySceneIndexAndFlags Func_0200054c
extern s16 Data_02000240[];
extern u8 Value_0000006f;
extern u8 Data_0200e984[];
extern u8 Data_0200e96c[];
extern u8 Data_0200eb94;
extern u8 Data_0200f420[];
extern u8 Data_0200f444[];
extern u8 Data_0200f570[];
extern u8 Data_0200f6fc[];
extern u8 Data_0200f81c[];
extern u8 Data_0200f930[];
extern u8 Data_0200f984[];
extern u8 Data_0200f9c0[];
extern u8 Data_0200fb58[];
extern u8 Data_0200fd44[];
extern u8 Data_0200fedc[];

s32 Func_02006a6a();
s32 Func_02006a78();
s32 Func_02006a82();
s32 Func_02006a94();
s32 Func_02006aa8();
s32 Func_02006ab6();

/*
 * Overlay resource_3b1. Picks the scene data table for the current scene
 * index, with two arms further narrowed by story flags.
 */

s32 SceneData_SelectTableByWord224(void)
{
    if (Data_02000240[224] == (s32)&Value_0000006f) {
        return (s32)Data_0200e984;
    }
    return (s32)Data_0200e96c;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTableEB94(void)
{
    return (s32)&Data_0200eb94;
}

/*
 * The 292-byte owner at 0x0200054c covers the dispatcher, a 23-entry jump
 * table, the case bodies, an alignment halfword and the literal pool. Case
 * order and the shared arms reproduce the reference: 23 shares an arm with
 * 4 while 22 does not, and the 15/17/19 arm skips 16, 18 and 20. 2208 is
 * synthesised in the reference and stays decimal; 0x928 and 0x93e are pool
 * loads.
 */
u8 *SceneData_SelectTableBySceneIndexAndFlags(void)
{
    s16 *tbl = Data_02000240;
    s32 scene = tbl[225];

    switch (scene) {
    case 1:
    case 2:
        if (Func_02006a6a(2208) != 0) {
            return Data_0200f6fc;
        }
        if (Func_02006a78(0x928) != 0 && Func_02006a82(0x93e) == 0) {
            return Data_0200f570;
        }
        return Data_0200f444;
    case 4:
    case 23:
        if (Func_02006a94(0x93e) != 0) {
            return Data_0200fedc;
        }
        return Data_0200f9c0;
    case 5:
        if (Func_02006aa8(2208) != 0) {
            return Data_0200f930;
        }
        if (Func_02006ab6(0x93e) != 0) {
            return Data_0200f984;
        }
        return Data_0200f81c;
    case 15:
    case 17:
    case 19:
        return Data_0200fb58;
    case 21:
        return Data_0200fd44;
    default:
        break;
    }

    return Data_0200f420;
}
