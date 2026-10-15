#include "types.h"
#include "party_state.h"

extern char SceneId_Ov676Room1;
extern char SceneId_Ov676Room2;
extern const u8 Data_02002d24[];
extern const u8 Data_02002eec[];
extern const u8 Data_02002f10[];

const u8 *Func_020004ec(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov676Room1 && gPartyState.entrance == 15)
        return Data_02002f10;
    if (gPartyState.scene == (s32)&SceneId_Ov676Room2)
        return Data_02002eec;
    return Data_02002d24;
}
