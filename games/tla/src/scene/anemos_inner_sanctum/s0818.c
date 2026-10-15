#include "types.h"
#include "party_state.h"

extern char SceneId_Ov6B0Room1;
extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;
extern char SceneId_Ov6B0Room5;
extern const u8 Data_02003b38[];
extern const u8 Data_02003b50[];
extern const u8 Data_02003b68[];
extern const u8 Data_02003bc8[];
extern const u8 Data_02003c88[];
extern const u8 Data_02003ce8[];

const u8 *Func_02000818(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room1)
        return Data_02003b50;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room2)
        return Data_02003b68;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room3)
        return Data_02003bc8;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room4)
        return Data_02003c88;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room5)
        return Data_02003ce8;
    return Data_02003b38;
}
