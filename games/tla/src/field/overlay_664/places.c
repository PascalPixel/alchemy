#include "ov664.h"

extern const u8 Data_02006238[];
extern const u8 Data_02006268[];

const u8 *Func_020025cc(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov664Room1)
        return Data_02006268;
    return Data_02006238;
}
