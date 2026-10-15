#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

s32 GameFlag_Test(s32 flag);

extern u8 Data_02001194[];
extern u8 Data_020011ac[];
extern u8 Data_020012cc[];
extern u8 Data_0200147c[];
extern u8 Data_0200159c[];
extern u8 Data_0200165c[];

void *Func_02000064(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov693Room1) {
        if (GameFlag_Test(0x9b2))
            return Data_0200147c;
        if (GameFlag_Test(0x9d0))
            return Data_020012cc;
        return Data_020011ac;
    }
    if (scene == (s32)&SceneId_Ov693Room2) {
        if (GameFlag_Test(0x9d0))
            return Data_0200159c;
        return Data_0200165c;
    }
    return Data_02001194;
}
