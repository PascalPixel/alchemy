#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_020016e4[];
extern const u8 Data_02001780[];
extern const u8 Data_0200184c[];
extern const u8 Data_020018f4[];
extern const u8 Data_02001a5c[];

const u8 *Func_02000b3c(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6afRoom2)
        return Data_02001780;
    if (scene == (s32)&SceneId_Ov6afRoom3)
        return Data_0200184c;
    if (scene == (s32)&SceneId_Ov6afRoom4)
        return Data_020018f4;
    if (scene == (s32)&SceneId_Ov6afRoom5)
        return Data_02001a5c;
    return Data_020016e4;
}
