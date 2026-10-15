#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

struct SceneEvent;

extern const u8 gData_0200460c[];
extern const u8 gData_02004720[];
extern const u8 gData_020047e0[];
extern const u8 gData_0200487c[];
extern const u8 gData_02004930[];
extern const u8 gData_02004a5c[];

const struct SceneEvent *Func_020021e8(void)
{
    if (gPartyState.scene == (s32)&SceneId_Ov691Room2)
    {
        if (gPartyState.entrance <= 3)
            return (const struct SceneEvent *)gData_02004720;
        if (gPartyState.entrance <= 8)
            return (const struct SceneEvent *)gData_020047e0;
        if (gPartyState.entrance <= 12)
            return (const struct SceneEvent *)gData_0200487c;
        return (const struct SceneEvent *)gData_02004930;
    }
    if (gPartyState.scene == (s32)&SceneId_Ov691Room3)
        return (const struct SceneEvent *)gData_02004a5c;
    return (const struct SceneEvent *)gData_0200460c;
}
