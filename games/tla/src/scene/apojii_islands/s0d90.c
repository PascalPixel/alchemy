#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 gData_020024b8[];
extern const u8 gData_020024d0[];
extern const u8 gData_02002698[];
extern const u8 gData_0200280c[];

const u8 *Func_02000d90(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov679Room1)
        return gData_020024d0;
    if (scene == (s32)&SceneId_Ov679Room3)
        return gData_02002698;
    if (scene == (s32)&SceneId_Ov679Room2)
        return gData_0200280c;
    return gData_020024b8;
}
