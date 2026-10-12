#include "ov674.h"

s32 Func_02002b64(void)
{
    struct Ov674EventWork *work = Ram_HeapSlots->event_work;
    s16 scene;

    work->field_1ac = 0x100;
    scene = gPartyState.scene;
    if (scene == (s32)&SceneId_Ov674Room1)
        Func_02002c38();
    else if (scene == (s32)&SceneId_Ov674Room2)
        Func_02002f1c();
    else if (scene == (s32)&SceneId_Ov674Room3)
        Func_020030b4();
    else if (scene == (s32)&SceneId_Ov674Room4)
        Func_02003164();
    else if (scene == (s32)&SceneId_Ov674Room5)
        Func_0200329c();
    return 0;
}

void Func_02002bd8(void)
{
    s32 y = Object_GetById(gPartyState.current_owner)->y;

    asm volatile("" : "+l"(y)); /* FAKEMATCH: the listing loads y before it builds 0x80000 */
    if (y > 0x80000) {
        Object_GetById(9)->collision_flags &= ~8;
        Object_GetById(10)->collision_flags &= ~8;
    } else {
        Object_GetById(9)->collision_flags |= 8;
        Object_GetById(10)->collision_flags |= 8;
    }
}
