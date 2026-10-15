/* Draft, differ 12 of 76: in each column test the native loads X into r3 and shifts it in place; here the load goes to r2 (asrs r3, r2, #19), one more instruction on the fall-through path. Tried: an X local for the first test, braces, a one-pass block between the tests, reloading the object. permute (1 minute) reached differ 2 only by reading X through a pointer variable. */
#include "../../../../games/tla/src/field/overlay_68f/ov68f.h"

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_020031a8(s32 cue);
void Func_02003178(void);

void Func_02000d2c(void)
{
    struct ObjectRuntime *object = *(struct ObjectRuntime **)((u8 *)Ram_HeapSlots->event_work + 0xcbc);

    Func_02003048();
    Func_02003148(0);
    if (Map_GetTerrainHeight(2, object->x, object->z) != object->y) {
        object->terrain_id = 2;
        object->motion_flags = 3;
        OverlayObject_WaitUntilIdle(object);
        Func_020031a8(188);
        OverlayObject_WaitUntilIdle(object);
        object->motion_flags = 0;
        object->terrain_id = 0;
    }
    Func_02003178();
    if (gPartyState.scene == (s32)&SceneId_Ov68fRoom6) {
        if (object->x >> 19 == 27)
            GameFlag_SetBit(0x947);
        if (object->x >> 19 == 25 && GameFlag_Test(0x947))
            GameFlag_SetBit(0x946);
    }
    Func_02003050();
}
