#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

void Func_02001f2c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static __inline__ void Func_02001f2c_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: as in ☀️'s scene layer, forwarding through the inline sets the arguments up in their own order and keeps each call's constants; a direct call builds the costly constant first or shares it (differ 2 or more). */
    Func_02001f2c(a, b, c, d, e, f);
}

s32 Func_02000c80(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov657Room1) {
        Func_02001f2c_(2, 2, 10, 24, 18, 10);
        Func_02001f2c_(0, 62, 26, 62, 3, 8);
        Func_02001f2c_(50, 65, 25, 61, 6, 1);
    }
    return 0;
}
