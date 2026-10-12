#include "scene.h"

void Func_020037d4(void)
{
    ((struct SceneEventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x204;
}

void Func_020037e8(void)
{
    ((struct SceneEventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x204;
    Func_020041c8(137);
    Func_02003820(Data_02004800);
    Func_02002d54();
    Scene_TaskAdd(Func_02001e58, 0x480);
}
