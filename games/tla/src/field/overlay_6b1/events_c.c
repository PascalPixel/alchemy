#include "ov6b1.h"

extern const u8 Data_02001c1c[];
extern const u8 Data_02001c28[];

const u8 *Func_02000714(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov6b1Room1)
        return Data_02001c28;
    return Data_02001c1c;
}
