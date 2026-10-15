#include "types.h"
#include "party_state.h"

extern char SceneId_Ov6B0Room1;
extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;
extern char SceneId_Ov6B0Room5;
extern const u8 Data_02003dc0[];
extern const u8 Data_02003dcc[];
extern const u8 Data_02003e38[];
extern const u8 Data_02003fac[];
extern const u8 Data_02004150[];
extern const u8 Data_020042c4[];

const u8 *Func_02001bec(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room1)
        return Data_02003dcc;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room2)
        return Data_02003e38;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room3)
        return Data_02003fac;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room4)
        return Data_02004150;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room5)
        return Data_020042c4;
    return Data_02003dc0;
}
