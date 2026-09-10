#include "types.h"

#define SelectPrimarySceneData Func_020009dc
#define GetEmptySceneData Func_02000a30
#define SelectSecondarySceneData Func_02000a34
#define SelectTertiarySceneData Func_02000a80
#define SelectQuaternarySceneData Func_02000af8
#define SelectActor25SceneVariant Func_02004928
#define SelectActor24SceneVariant Func_02004964
extern s16 Data_02000240[];
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Data_0200e2a4[];
extern u8 Data_0200e4b4[];
extern u8 Data_0200e754[];
extern u8 Data_0200e814[];
extern u8 Value_000000a3;
extern u8 Data_0200e910[];
extern u8 Data_0200e97c[];
extern u8 Data_0200e8a4[];
extern u8 Value_0000006a;
extern u8 Data_0200e9d0[];
extern u8 Data_0200ee08[];
extern u8 Data_0200ec28[];
extern u8 Data_0200eac0[];
extern u8 Data_0200ee98[];
extern u8 Data_0200e9b8[];
extern u8 Data_0200eff4[];
extern u8 Data_0200f258[];
extern u8 Data_0200f528[];
extern u8 Data_0200f63c[];

s32 Func_02009f3e(s32);
void Func_0200a020(s32);
void Func_0200a038(s32, s32);
void Func_0200a030(s32);
void Func_0200a048(s32, s32);
s32 Func_02009f7a(s32);
void Func_0200a05c(s32);
void Func_0200a074(s32, s32);
void Func_0200a06c(s32);
void Func_0200a084(s32, s32);

/* No scene data is available for this slot. */

s32 SelectPrimarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200e2a4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e4b4;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200e754;
    }
    return (s32)Data_0200e814;
}

s32 GetEmptySceneData(void) { return 0; }

s32 SelectSecondarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e910;
    }
    if (scene_variant == (s32)&Value_000000a2 || scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200e97c;
    }
    return (s32)Data_0200e8a4;
}

s32 SelectTertiarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_0000006a) {
        return (s32)Data_0200e9d0;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200ee08;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200ec28;
    }
    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eac0;
    }
    if (scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200ee98;
    }
    return (s32)Data_0200e9b8;
}

s32 SelectQuaternarySceneData(void)
{
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a0) {
        return (s32)Data_0200eff4;
    }
    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200f258;
    }
    if (scene_variant == (s32)&Value_000000a2) {
        return (s32)Data_0200f528;
    }
    return (s32)Data_0200f63c;
}

void SelectActor25SceneVariant(void)
{
    if (Func_02009f3e(0x941)) {
        Func_0200a020(0x2568);
        Func_0200a038(25, 0);
    } else {
        Func_0200a030(0x2458);
        Func_0200a048(25, 0);
    }
}

void SelectActor24SceneVariant(void)
{
    if (Func_02009f7a(0x941)) {
        Func_0200a05c(0x2569);
        Func_0200a074(24, 0);
    } else {
        Func_0200a06c(0x244e);
        Func_0200a084(24, 0);
    }
}
