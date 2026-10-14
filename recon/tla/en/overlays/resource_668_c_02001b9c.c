/* Draft, differ 14 of 35: the native stores the animation's byte 26 from a
 * pooled 0 (ldr r1, =0x0) apart from the motion flags' movs 0 and rebuilds the
 * value field's address from object + 85; this draft shares one zero. Tried:
 * s8, u16 and NULL forms of the store. */
#include "../../../../games/tla/include/types.h"
#include "../../../../games/tla/include/object_runtime.h"

extern const u8 Data_020056d4[];
struct ObjectRuntime *Func_02004a1c(s32 type, s32 x, s32 y, s32 z);
void Func_02004a0c(struct ObjectRuntime *object, s32 mode);
void Func_02004a14(struct ObjectRuntime *object, const void *script);
void Func_02004a74(struct ObjectRuntime *object, s32 priority);

void Func_02001b9c(s32 x, s32 y, s32 z, s16 angle, s32 value)
{
    struct ObjectRuntime *object = Func_02004a1c(488, x, y, z);

    if (object != NULL) {
        u8 *animation = object->animation;

        Func_02004a0c(object, 1);
        Func_02004a14(object, Data_020056d4);
        Func_02004a74(object, 1);
        object->motion_flags = 0;
        *(u16 *)&object->unknown_5c[2] = value;
        animation[26] = 0;
        *(s16 *)(animation + 18) = angle;
    }
}
