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

extern u8 gData_020024ac[];
extern u8 gData_020024dc[];
extern u8 gData_02002524[];
extern u8 gData_02002554[];
extern u8 gData_020025cc[];
extern u8 gData_02002614[];
extern u8 gData_020027ac[];
extern u8 gData_020027dc[];
extern u8 gData_02002854[];

u8 *Func_02000358(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov690Room1)
        return gData_020024dc;
    if (scene == (s32)&SceneId_Ov690Room2)
        return gData_02002524;
    if (scene == (s32)&SceneId_Ov690Room4)
        return gData_02002554;
    if (scene == (s32)&SceneId_Ov690Room5)
        return gData_020025cc;
    if (scene == (s32)&SceneId_Ov690Room7)
        return gData_020027dc;
    if (scene == (s32)&SceneId_Ov690Room8)
        return gData_02002854;
    if (scene == (s32)&SceneId_Ov690Room9)
        return gData_02002614;
    if (scene == (s32)&SceneId_Ov690Room10)
        return gData_020027ac;
    return gData_020024ac;
}
