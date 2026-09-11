#include "types.h"
#include "scene.h"
#include "abi/object/dispatch/stop_callbacks_and_hide_layers.h"

void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
    Obj_Check((u32)Obj_Run);
    Obj_Check((u32)Obj_Run2);
    *(u16 *)0x04000000 &= 0xE1FF;
}
