// near miss: differ 14 (listing 48 instructions, draft 48); registers: the native keeps the object in r5 and the velocity in r6; the draft swaps them
#include "../../../../games/tla/src/field/overlay_696/ov696.h"

void Func_02001cc8(void)
{
    s32 moving;
    u8 *flags;
    struct Ov696Object *object = Object_GetById(14);

    if (object->x >> 20 == 32 && object->z >> 20 == 11) {
        Func_020048c4();
        Func_02004a24(0);
        flags = &object->motion_flags;
        *flags = 3;
        do {
            WaitFrames(1);
            moving = object->velocity_y;
        } while (moving != 0);
        Engine_AudioPlayCue(188);
        WaitFrames(10);
        *flags = moving;
        Ov696_BlendStart(30, 11, 1, 1, 32, 11);
        WaitFrames(1);
        GameFlag_SetBit(0x94e);
        Func_020048cc();
    }
}
