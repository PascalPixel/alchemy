#include "types.h"
#include "party_state.h"

extern char SceneId_SuharaGate1;
extern char SceneId_SuharaGate2;
extern char SceneId_SuharaGate3;

extern u8 Data_02000f10[];
extern u8 Data_02000f58[];
extern u8 Data_02001048[];
extern u8 Data_02001078[];
extern u8 Data_020010e4[];
extern u8 Data_02001180[];

u8 *Func_02000090(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_SuharaGate2)
        return Data_02000f58;
    if (scene == (s32)&SceneId_SuharaGate1)
        return Data_02001048;
    return Data_02000f10;
}

u8 *Func_020000d0(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_SuharaGate2)
        return Data_020010e4;
    if (scene == (s32)&SceneId_SuharaGate3)
        return Data_02001180;
    return Data_02001078;
}
