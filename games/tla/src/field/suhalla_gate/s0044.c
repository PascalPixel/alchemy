#include "types.h"
#include "party_state.h"

extern char SceneId_SuharaGate2;
extern char SceneId_SuharaGate3;

extern u8 Data_02000cd8[];
extern u8 Data_02000d98[];
extern u8 Data_02000e40[];

u8 *Func_02000044(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_SuharaGate2)
        return Data_02000d98;
    if (scene == (s32)&SceneId_SuharaGate3)
        return Data_02000e40;
    return Data_02000cd8;
}
