#include "types.h"
#include "party_state.h"
#include "scene_ids.h"

extern const u8 Data_02004688[];
extern const u8 Data_020046a0[];
extern const u8 Data_02004748[];
extern const u8 Data_020047a8[];
extern const u8 Data_02004808[];
extern const u8 Data_020048c8[];
extern const u8 Data_02004910[];
extern const u8 Data_020049e8[];
extern const u8 Data_02004a00[];
extern const u8 Data_02004a48[];
extern const u8 Data_02004b98[];

const u8 *Func_0200035c(void)
{
    s16 scene = gPartyState.scene;

    if (scene == (s32)&SceneId_Ov67cRoom1)
        return Data_020046a0;
    if (scene == (s32)&SceneId_Ov67cRoom2)
        return Data_02004748;
    if (scene == (s32)&SceneId_Ov67cRoom3)
        return Data_020047a8;
    if (scene == (s32)&SceneId_Ov67cRoom4)
        return Data_02004808;
    if (scene == (s32)&SceneId_Ov67cRoom5)
        return Data_020048c8;
    if (scene == (s32)&SceneId_Ov67cRoom6)
        return Data_02004910;
    if (scene == (s32)&SceneId_Ov67cRoom7)
        return Data_020049e8;
    if (scene == (s32)&SceneId_Ov67cRoom8)
        return Data_02004a00;
    if (scene == (s32)&SceneId_Ov67cRoom9)
        return Data_02004a48;
    if (scene == (s32)&SceneId_Ov67cRoom10)
        return Data_02004b98;
    return Data_02004688;
}
