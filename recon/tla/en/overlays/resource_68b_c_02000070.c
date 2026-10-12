/* First draft, differ 48: the argument copies and the three masked angles take other registers, and the left and right tests come in the other order. */
#include "types.h"
#include "object_runtime.h"

s32 Func_02000038(s32 *a, s32 *b);
s32 ArcTan2(s32 y, s32 x);

s32 Func_02000070(struct ObjectRuntime *self, struct ObjectRuntime *other, s32 range)
{
    s32 *to = &other->x;
    s32 *from = &self->x;
    s32 result = 0;

    if (Func_02000038(to, from) < range) {
        u16 angle = ArcTan2(other->z - self->z, *to - *from);
        s32 right = (angle + 0x1000) & 0xf000;
        s32 left = (angle - 0x1000) & 0xf000;
        s32 ahead = angle & 0xf000;
        s32 facing = self->angle & 0xf000;

        if (ahead == facing || right == facing || left == facing)
            result = 1;
    }
    return result;
}
