#include "types.h"
#include "party_state.h"

extern char SceneId_SuhallaGate2;
extern char SceneId_SuhallaGate3;

extern u8 gData_02000cd8[];
extern u8 gData_02000d98[];
extern u8 gData_02000e40[];

u8 *Func_02000044(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_SuhallaGate2)
        return gData_02000d98;
    if (scene == (s32)&SceneId_SuhallaGate3)
        return gData_02000e40;
    return gData_02000cd8;
}
