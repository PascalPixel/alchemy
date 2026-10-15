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

extern u8 Data_02005f60[];
extern u8 Data_02005fe4[];
extern u8 Data_020060bc[];
extern u8 Data_020061c4[];
extern u8 Data_020063a4[];
extern u8 Data_02006458[];
extern u8 Data_0200659c[];
extern u8 Data_02006714[];

u8 *Func_02003418(void)
{
    s16 scene = gPartyState.scene;

    if (scene != (s32)&SceneId_Ov692Room1) {
        if (scene == (s32)&SceneId_Ov692Room2)
            return Data_02005fe4;
        if (scene == (s32)&SceneId_Ov692Room3)
            return Data_020060bc;
        if (scene == (s32)&SceneId_Ov692Room4)
            return Data_020061c4;
        if (scene == (s32)&SceneId_Ov692Room5)
            return Data_020063a4;
        if (scene == (s32)&SceneId_Ov692Room6)
            return Data_02006458;
        if (scene == (s32)&SceneId_Ov692Room7)
            return Data_0200659c;
        if (scene == (s32)&SceneId_Ov692Room8)
            return Data_02006714;
    }
    return Data_02005f60;
}
