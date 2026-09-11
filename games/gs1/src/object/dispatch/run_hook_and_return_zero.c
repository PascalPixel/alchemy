#include "types.h"
#include "scene.h"
#include "abi/object/dispatch/run_hook_and_return_zero.h"

s32 ObjectDispatch_RunHookAndReturnZero(void)
{
    Obj_Check();
    return 0;
}
