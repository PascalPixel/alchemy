#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_020023ec[];
extern const u8 Data_020023f8[];

const u8 *Func_0200013c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov657Room2)
        return Data_020023f8;
    return Data_020023ec;
}
