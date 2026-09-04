#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000a0;
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Data_0200e2a4[];
extern u8 Data_0200e4b4[];
extern u8 Data_0200e754[];
extern u8 Data_0200e814[];

#define SelectPrimarySceneData Func_020009dc
s32 SelectPrimarySceneData(void) {
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
