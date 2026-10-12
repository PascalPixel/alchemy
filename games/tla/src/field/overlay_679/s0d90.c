#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_020024b8[];
extern const u8 Data_020024d0[];
extern const u8 Data_02002698[];
extern const u8 Data_0200280c[];

const u8 *Func_02000d90(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov679Room1)
        return Data_020024d0;
    if (scene == (s32)&SceneId_Ov679Room3)
        return Data_02002698;
    if (scene == (s32)&SceneId_Ov679Room2)
        return Data_0200280c;
    return Data_020024b8;
}
