#include "types.h"
#include "party_state.h"

extern char SceneId_Ov663Room1;
extern char SceneId_Ov663Room2;
extern s16 Data_02003e84[];
extern s16 Data_02003ec4[];
extern s16 Data_02003f44[];

s16 *Func_020004c0(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov663Room1)
        return Data_02003e84;
    if (scene == (s32)&SceneId_Ov663Room2)
        return Data_02003ec4;
    return Data_02003f44;
}
