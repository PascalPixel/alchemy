#include "ov697.h"

extern const u8 Data_0200646c[];
extern const u8 Data_02006ea4[];
extern const u8 Data_02006ed4[];
extern const u8 Data_02006f04[];
extern const u8 *const Data_02007024[];

s32 Func_02000058(struct ObjectRuntime *object)
{
    s32 height = Map_GetTerrainHeight(0, object->x, object->z) + 0x20000;

    object->terrain_height = height;
    object->y = height;
    object->motion_flags = 0;
    object->collision_flags = 0;
    object->scale_y = 0x18000;
    return 0;
}

const u8 *Func_02000088(void)
{
    s32 pattern = gOv697State->pattern;

    if (gPartyState.scene == (s32)&SceneId_Ov697Room1)
        return Data_02007024[pattern & 7];
    if (gPartyState.scene == (s32)&SceneId_Ov697Room3)
        return Data_02006ea4;
    if (gPartyState.scene == (s32)&SceneId_Ov697Room4)
        return Data_02006ed4;
    if (gPartyState.scene == (s32)&SceneId_Ov697Room2)
        return Data_02006f04;
    return Data_0200646c;
}

s32 Func_020000fc(s32 fallback)
{
    struct Ov697State *state = gOv697State;

    if (state->field_1a < state->field_1c)
        return 1;
    if (state->field_1a > state->field_1c)
        return 0;
    if (GameFlag_Test(0x284))
        return 0;
    if (state->field_14 != 0)
        return 1;
    return fallback;
}
