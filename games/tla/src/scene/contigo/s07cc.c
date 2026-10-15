#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern u8 Data_02004c70[];
extern u8 Data_02004c88[];
extern u8 Data_02004d90[];
extern u8 Data_02005018[];
extern u8 Data_02005228[];
extern u8 Data_02005318[];
extern u8 Data_02005390[];

s32 GameFlag_Test(s32 flag);

u8 *Func_020007cc(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov694Room1)
        return Data_02004c88;
    if (scene == (s32)&SceneId_Ov694Room4)
        return Data_02004d90;
    if (scene == (s32)&SceneId_Ov694Room2) {
        if (gPartyState.entrance == 4) {
            if (GameFlag_Test(0x9b2) != 0)
                return Data_02005318;
            if (GameFlag_Test(0xa21) != 0 && GameFlag_Test(0x9d0) == 0)
                return Data_02005228;
        }
        return Data_02005018;
    }
    if (scene == (s32)&SceneId_Ov694Room3)
        return Data_02005390;
    return Data_02004c70;
}
