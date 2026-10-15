#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_02001330[];
extern const u8 Data_02001390[];
extern const u8 Data_02001480[];
extern const u8 Data_02001504[];
extern const u8 Data_0200169c[];

const u8 *Func_02000350(void)
{
    s32 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov6afRoom2)
        return Data_02001390;
    if (scene == (s32)&SceneId_Ov6afRoom3)
        return Data_02001480;
    if (scene == (s32)&SceneId_Ov6afRoom4)
        return Data_02001504;
    if (scene == (s32)&SceneId_Ov6afRoom5)
        return Data_0200169c;
    return Data_02001330;
}
