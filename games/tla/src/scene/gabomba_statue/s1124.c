#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_02006e9c[];
extern const u8 Data_02006b54[];
extern const u8 Data_02006b6c[];
extern const u8 Data_02006bd8[];
extern const u8 Data_02006c80[];
extern const u8 Data_02006d1c[];
extern const u8 Data_02006db8[];
extern const u8 Data_02006e78[];

const u8 *Func_02001124(void)
{
    s16 scene = gPartyState.scene;
    const u8 *table = Data_02006e9c;

    if (scene == (s32)&SceneId_Ov671Room1)
        table = Data_02006b54;
    else if (scene == (s32)&SceneId_Ov671Room2)
        table = Data_02006b6c;
    else if (scene == (s32)&SceneId_Ov671Room3)
        table = Data_02006bd8;
    else if (scene == (s32)&SceneId_Ov671Room4)
        table = Data_02006c80;
    else if (scene == (s32)&SceneId_Ov671Room5)
        table = Data_02006d1c;
    else if (scene == (s32)&SceneId_Ov671Room6)
        table = Data_02006db8;
    else if (scene == (s32)&SceneId_Ov671Room7)
        table = Data_02006e78;
    return table;
}
