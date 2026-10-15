#include "types.h"
#include "party_state.h"

extern char SceneId_Ov663Room1;
extern char SceneId_Ov663Room2;
extern s32 Data_020040ec[];
extern u32 Data_02004194[];
extern u32 Data_02004314[];

void *Func_02000508(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov663Room1)
        return Data_020040ec;
    if (scene == (s32)&SceneId_Ov663Room2)
        return Data_02004194;
    return Data_02004314;
}
