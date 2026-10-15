#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_0200171c[];
extern const u8 Data_02001728[];
extern const u8 Data_020018f0[];

const u8 *Func_020000e0(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov693Room1)
        return Data_02001728;
    if (scene == (s32)&SceneId_Ov693Room2)
        return Data_020018f0;
    return Data_0200171c;
}
