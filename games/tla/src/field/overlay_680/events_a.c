#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

struct SceneEvent;

void Func_02000bc0(s32 id, s32 flag);

extern const u8 Data_0200102c[];
extern const u8 Data_02001044[];
extern const u8 Data_02001194[];
extern const u8 Data_020012cc[];

const struct SceneEvent *Func_02000080(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov680Room1)
        return (const struct SceneEvent *)Data_02001044;
    if (gPartyState.scene == (s32)&SceneId_Ov680Room2)
        return (const struct SceneEvent *)Data_02001194;
    if (gPartyState.scene == (s32)&SceneId_Ov680Room3)
        return (const struct SceneEvent *)Data_020012cc;
    return (const struct SceneEvent *)Data_0200102c;
}

void Func_020000d0(void)
{
    Func_02000bc0(17, 0x300);
}

void Func_020000e0(void)
{
    Func_02000bc0(18, 0x301);
}

void Func_020000f0(void)
{
    Func_02000bc0(19, 0x302);
}
