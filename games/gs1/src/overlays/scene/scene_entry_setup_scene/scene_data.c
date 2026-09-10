#include "types.h"

#define SceneData_SelectTableBySceneB5ToBa Func_02000e04
#define SceneData_ReturnZero Func_02000e7c
#define SceneData_GetTablee740 Func_02000e80
#define SceneData_SelectTableBySceneAndApply Func_02000e88
#define SceneData_SelectTableBySceneId Func_02002f8c
extern u8 Value_000000b5;
extern u8 Value_000000b7;
extern u8 Value_000000b8;
extern u8 Value_000000b9;
extern u8 Value_000000ba;
extern u8 Data_0200dd68[];
extern u8 Data_0200e020[];
extern u8 Data_0200e230[];
extern u8 Data_0200e350[];
extern u8 Data_0200e548[];
extern u8 Data_0200ddc8[];
extern u8 Data_0200e740[];
extern u8 Value_000000b6;
extern u8 Data_0200e8ec[];
extern u8 Data_0200e904[];
extern u8 Data_0200e9c4[];
extern u8 Data_0200eb74[];
extern u8 Data_0200ec04[];
extern u8 Data_0200ec64[];
extern u8 Data_0200ecf4[];
extern u8 Data_0200ee44[];
extern u8 Data_0200f120[];
extern u8 Data_0200f300[];
extern u8 Data_0200f3b4[];
extern u8 Data_0200f4f8[];
extern u8 Data_0200ef1c[];

void Func_02005d78(u8 *);

/* Contiguous unnamed leaf-owner run for resource_3c8. */

s32 SceneData_SelectTableBySceneB5ToBa(void)
{
    extern s16 Data_02000240[];

    s16 v = Data_02000240[224];

    if (v == (s32)&Value_000000b5) {
        return (s32)Data_0200dd68;
    }
    if (v == (s32)&Value_000000b7) {
        return (s32)Data_0200e020;
    }
    if (v == (s32)&Value_000000b8) {
        return (s32)Data_0200e230;
    }
    if (v == (s32)&Value_000000b9) {
        return (s32)Data_0200e350;
    }
    if (v == (s32)&Value_000000ba) {
        return (s32)Data_0200e548;
    }
    return (s32)Data_0200ddc8;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetTablee740(void)
{
    return Data_0200e740;
}

u8 *SceneData_SelectTableBySceneAndApply(void)
{
    extern s16 Data_02000240[];

    u8 *ret;
    s16 *tbl;
    s16 v;

    tbl = Data_02000240;
    v = tbl[224];
    if (v == (s32)(u32)&Value_000000b5) {
        return Data_0200e904;
    }
    if (v == (s32)(u32)&Value_000000b6) {
        ret = Data_0200e9c4;
    } else if (v == (s32)(u32)&Value_000000b7) {
        ret = Data_0200eb74;
    } else if (v == (s32)(u32)&Value_000000b8) {
        ret = Data_0200ec04;
    } else if (v == (s32)(u32)&Value_000000b9) {
        ret = Data_0200ec64;
    } else if (v == (s32)(u32)&Value_000000ba) {
        ret = Data_0200ecf4;
    } else {
        goto no_match;
    }
    Func_02005d78(ret);
    return ret;

no_match:
    return Data_0200e8ec;
}

u8 *SceneData_SelectTableBySceneId(void)
{
    extern u8 Data_02000240[];

    s32 off = 0x1c0;
    s32 v = *(s16 *)(Data_02000240 + off);

    if (v == (s32)&Value_000000b5) {
        return Data_0200ee44;
    }
    if (v == (s32)&Value_000000b6) {
        return Data_0200ef1c;
    }
    if (v == (s32)&Value_000000b7) {
        return Data_0200f120;
    }
    if (v == (s32)&Value_000000b8) {
        return Data_0200f300;
    }
    if (v == (s32)&Value_000000b9) {
        return Data_0200f3b4;
    }
    if (v == (s32)&Value_000000ba) {
        return Data_0200f4f8;
    }
    return Data_0200ef1c;
}
