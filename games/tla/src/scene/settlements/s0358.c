#include "types.h"
#include "party_state.h"

extern char SceneId_Ov690Room1;
extern char SceneId_Ov690Room2;
extern char SceneId_Ov690Room4;
extern char SceneId_Ov690Room5;
extern char SceneId_Ov690Room7;
extern char SceneId_Ov690Room8;
extern char SceneId_Ov690Room9;
extern char SceneId_Ov690Room10;

extern u8 Data_020024ac[];
extern u8 Data_020024dc[];
extern u8 Data_02002524[];
extern u8 Data_02002554[];
extern u8 Data_020025cc[];
extern u8 Data_02002614[];
extern u8 Data_020027ac[];
extern u8 Data_020027dc[];
extern u8 Data_02002854[];

u8 *Func_02000358(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov690Room1)
        return Data_020024dc;
    if (scene == (s32)&SceneId_Ov690Room2)
        return Data_02002524;
    if (scene == (s32)&SceneId_Ov690Room4)
        return Data_02002554;
    if (scene == (s32)&SceneId_Ov690Room5)
        return Data_020025cc;
    if (scene == (s32)&SceneId_Ov690Room7)
        return Data_020027dc;
    if (scene == (s32)&SceneId_Ov690Room8)
        return Data_02002854;
    if (scene == (s32)&SceneId_Ov690Room9)
        return Data_02002614;
    if (scene == (s32)&SceneId_Ov690Room10)
        return Data_020027ac;
    return Data_020024ac;
}
