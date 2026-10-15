#include "local.h"

extern const u8 Data_02001310[];

const struct SceneRegion *Func_02000db4(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov673Room1)
        return (const struct SceneRegion *)Data_02001310;
    return 0;
}
