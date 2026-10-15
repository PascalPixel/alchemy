#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 gData_02006b20[];
extern const u8 gData_02005fd4[];
extern const u8 gData_02006004[];
extern const u8 gData_02006244[];
extern const u8 gData_020064cc[];
extern const u8 gData_020067e4[];
extern const u8 gData_02006898[];
extern const u8 gData_02006a90[];

const u8 *Func_020004b8(void)
{
    s16       scene = gPartyState.scene;
    const u8 *table = gData_02006b20;

    if (scene == (s32)&SceneId_Ov671Room1)
        table = gData_02005fd4;
    else if (scene == (s32)&SceneId_Ov671Room2)
        table = gData_02006004;
    else if (scene == (s32)&SceneId_Ov671Room3)
        table = gData_02006244;
    else if (scene == (s32)&SceneId_Ov671Room4)
        table = gData_020064cc;
    else if (scene == (s32)&SceneId_Ov671Room5)
        table = gData_020067e4;
    else if (scene == (s32)&SceneId_Ov671Room6)
        table = gData_02006898;
    else if (scene == (s32)&SceneId_Ov671Room7)
        table = gData_02006a90;
    return table;
}
