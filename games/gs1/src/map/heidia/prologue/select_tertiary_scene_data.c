#include "types.h"

extern s16 Data_02000240[];
extern u8 Value_00000071;
extern u8 Value_00000072;
extern u8 Value_0000007b;
extern u8 Value_0000007c;
extern u8 Value_0000007d;
extern u8 Data_0200b904[];
extern u8 Data_0200b8e0[];
extern u8 Data_0200b9f4[];
extern u8 Data_0200bd48[];
extern u8 Data_0200bd6c[];
extern u8 Data_0200b880[];

#define SelectTertiarySceneData Func_02000ec4

s32 SelectTertiarySceneData(void) {
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
