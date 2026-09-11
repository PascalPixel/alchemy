#include "types.h"
#include "scene.h"
#include "abi/object/motion/act/set_variant_callback_and_refresh.h"

s32 Motion_SetVarCb();

void Motion_SetVarCbAndRefresh(s32 arg0)
{
    Motion_SetVarCb();
    Obj_Check(arg0);
}
