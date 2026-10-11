#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_020022cc[];
extern const u8 Data_0200235c[];

const u8 *Func_0200004c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov657Room1)
        return Data_0200235c;
    return Data_020022cc;
}
