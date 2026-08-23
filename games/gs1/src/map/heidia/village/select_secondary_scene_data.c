#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_000000a1;
extern u8 Value_000000a2;
extern u8 Value_000000a3;
extern u8 Data_0200e910[];
extern u8 Data_0200e97c[];
extern u8 Data_0200e8a4[];

#define SelectSecondarySceneData Func_02000a34

s32 SelectSecondarySceneData(void) {
    s16 scene_variant = Data_02000240[224];

    if (scene_variant == (s32)&Value_000000a1) {
        return (s32)Data_0200e910;
    }
    if (scene_variant == (s32)&Value_000000a2 || scene_variant == (s32)&Value_000000a3) {
        return (s32)Data_0200e97c;
    }
    return (s32)Data_0200e8a4;
}
