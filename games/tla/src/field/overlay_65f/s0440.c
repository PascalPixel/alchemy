#include "ov65f.h"
#include "party_state.h"

extern char SceneId_Ov65FRoom1;
extern char SceneId_Ov65FRoom2;
extern char SceneId_Ov65FRoom3;
extern char SceneId_Ov65FRoom4;
extern char SceneId_Ov65FRoom5;

extern u32 Data_020056dc[];
extern u32 Data_020056f4[];
extern s16 Data_02005874[];
extern u32 Data_020059dc[];
extern s32 Data_02005a54[];
extern u32 Data_02005b5c[];
extern s32 Data_02005c1c[];

// 部屋ごとの表を返す
const void *Func_02000440(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov65FRoom1) {
        if (Flag_IsSet(0x91b))
            return Data_02005874;
        return Data_020056f4;
    }
    if (scene == (s32)&SceneId_Ov65FRoom2)
        return Data_02005b5c;
    if (scene == (s32)&SceneId_Ov65FRoom3)
        return Data_020059dc;
    if (scene == (s32)&SceneId_Ov65FRoom4)
        return Data_02005a54;
    if (scene == (s32)&SceneId_Ov65FRoom5)
        return Data_02005c1c;
    return Data_020056dc;
}
