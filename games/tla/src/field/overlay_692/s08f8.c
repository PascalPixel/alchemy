#include "types.h"
#include "party_state.h"

extern char SceneId_Ov692Room1;
extern char SceneId_Ov692Room2;
extern char SceneId_Ov692Room3;
extern char SceneId_Ov692Room4;
extern char SceneId_Ov692Room5;
extern char SceneId_Ov692Room6;
extern char SceneId_Ov692Room7;
extern char SceneId_Ov692Room8;

extern u8 Data_02005840[];
extern u8 Data_02005870[];
extern u8 Data_02005978[];
extern u8 Data_02005a50[];
extern u8 Data_02005c30[];
extern u8 Data_02005ca8[];
extern u8 Data_02005d38[];
extern u8 Data_02005ed0[];

u8 *Func_020008f8(void)
{
    s16 scene = gPartyState.scene;

    if (scene != (s32)&SceneId_Ov692Room1) {
        if (scene == (s32)&SceneId_Ov692Room2)
            return Data_02005870;
        if (scene == (s32)&SceneId_Ov692Room3)
            return Data_02005978;
        if (scene == (s32)&SceneId_Ov692Room4)
            return Data_02005a50;
        if (scene == (s32)&SceneId_Ov692Room5)
            return Data_02005c30;
        if (scene == (s32)&SceneId_Ov692Room6)
            return Data_02005ca8;
        if (scene == (s32)&SceneId_Ov692Room7)
            return Data_02005d38;
        if (scene == (s32)&SceneId_Ov692Room8)
            return Data_02005ed0;
    }
    return Data_02005840;
}
