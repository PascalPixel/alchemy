#include "TYPES.H"
#include "SCENE.H"

s32 GameFlag_TestFar();

s32 GameFlag_IsConditionActive(s32 condition)
{
    s32 flag_state;

    if (condition == -1) {
        return 1;
    }
    if (0x1000 & condition) {
        return GameFlag_TestFar();
    }
    flag_state = GameFlag_TestFar();
    return 1 - ((u32)((0 - flag_state) | flag_state) >> 0x1F);
}
