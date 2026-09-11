#include "types.h"
#include "scene.h"

void ObjectDispatch_StopCallbacksAndHideLayers(void)
{
    Obj_Check((u32)Obj_Run);
    Obj_Check((u32)Obj_unk2_2);
    *(u16 *)0x04000000 &= 0xE1FF;
}
