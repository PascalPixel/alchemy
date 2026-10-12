#include "types.h"
#include "party_state.h"

extern char SceneId_Ov676Room1;
extern char SceneId_Ov676Room2;
extern const u8 Data_02002ac4[];
extern const u8 Data_02002be4[];
extern const u8 Data_02002bfc[];

const u8 *Func_02000218(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov676Room1 && gPartyState.entrance == 15)
        return Data_02002bfc;
    if (gPartyState.scene == (s32)&SceneId_Ov676Room2)
        return Data_02002be4;
    return Data_02002ac4;
}
