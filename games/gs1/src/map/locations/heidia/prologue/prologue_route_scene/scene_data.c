#include "types.h"

#define SelectPrimarySceneData Func_02000cc4
#define GetEmptySceneData Func_02000d3c
#define GetDefaultSceneData Func_02000d40
#define SelectSecondarySceneData Func_02000d48
#define SelectTertiarySceneData Func_02000ec4
extern s16 Data_02000240[];
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_0000007b;
extern u8 Value_0000007c;
extern u8 Value_0000007d;
extern u8 Data_0200b310[];
extern u8 Data_0200b358[];
extern u8 Data_0200b3a0[];
extern u8 Data_0200b400[];
extern u8 Data_0200b448[];
extern u8 Data_0200b478[];
extern u8 Value_00000086;
extern u8 Value_0000007e;
extern u8 Data_0200b610[];
extern u8 Data_0200b718[];
extern u8 Data_0200b850[];
extern u8 Data_0200b5f8[];
extern u8 Data_0200b904[];
extern u8 Data_0200b8e0[];
extern u8 Data_0200b9f4[];
extern u8 Data_0200bd48[];
extern u8 Data_0200bd6c[];
extern u8 Data_0200b880[];

s32 SelectPrimarySceneData(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b310;
    }
    if (v == (s32)&Value_00000072) {
        return (s32)Data_0200b358;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b3a0;
    }
    if (v == (s32)&Value_0000007c) {
        return (s32)Data_0200b400;
    }
    if (v == (s32)&Value_0000007d) {
        return (s32)Data_0200b448;
    }
    return (s32)Data_0200b478;
}

/* Complete four-byte leaf: movs r0,#0 followed by bx lr. */
s32 GetEmptySceneData(void)
{
    return 0;
}

/* Complete eight-byte literal-address getter, including its sole pool word. */
u8 *GetDefaultSceneData(void)
{
    return (u8 *)0x0200b508;
}

s32 SelectSecondarySceneData(void)
{
    s16 v = Data_02000240[224];

    if (v == (s32)&Value_00000071) {
        return (s32)Data_0200b610;
    }
    if (v == (s32)&Value_0000007b) {
        return (s32)Data_0200b718;
    }
    if (v <= (s32)&Value_00000086 && v >= (s32)&Value_0000007e) {
        return (s32)Data_0200b850;
    }
    return (s32)Data_0200b5f8;
}

s32 SelectTertiarySceneData(void)
{
    s16 scene_id = Data_02000240[224];

    if (scene_id == (s32)&Value_00000071) {
        return (s32)Data_0200b904;
    }
    if (scene_id == (s32)&Value_00000072) {
        return (s32)Data_0200b8e0;
    }
    if (scene_id == (s32)&Value_0000007b) {
        return (s32)Data_0200b9f4;
    }
    if (scene_id == (s32)&Value_0000007c) {
        return (s32)Data_0200bd48;
    }
    if (scene_id == (s32)&Value_0000007d) {
        return (s32)Data_0200bd6c;
    }
    return (s32)Data_0200b880;
}
