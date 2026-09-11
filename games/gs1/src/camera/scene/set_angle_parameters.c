#include "types.h"
#include "scene.h"
#include "abi/camera/scene/set_angle_parameters.h"
#include "global_cells.h"

typedef s32 (*WorkFunc_08005208)(s32, s32);

struct Work_08005208 {
    s32 result;
    s32 param1;
    s32 param2;
};

void Camera_SetAngleParameters(u32 value, s32 param1, s32 param2)
{
    s32 half;
    s32 first;
    s32 result;

    half = (s32)(value + (value >> 31)) >> 1;
    first = Sys_Check(half);
    result = ((WorkFunc_08005208)0x0300013C)(
        first,
        Sys_Check2(half)* 0x50
    );
    ((struct Work_08005208 *)ADDR_03001CE0)->param1 = param1;
    ((struct Work_08005208 *)ADDR_03001CE0)->result = result;
    ((struct Work_08005208 *)ADDR_03001CE0)->param2 = param2;
}
