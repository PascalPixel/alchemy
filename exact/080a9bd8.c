#include "a9_motion.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

s32 Func_080022ec(s32, s32);
s32 Func_080022fc(s32, s32);
void Func_080a17c4(void *arg0);

void Func_080a9bd8(struct Object080a9bd8 *object, s32 index,
    s32 origin_x, s32 origin_y, s32 phase) {
    s32 wrapped_index;

    wrapped_index = index;
    if (wrapped_index > 0x1F) {
        wrapped_index = 0;
    }
    object->y =
        (s16)((Func_080022ec(wrapped_index, phase) * 0x10) + origin_y);
    object->x =
        (s16)((Func_080022fc(wrapped_index, phase) * 0x10) + origin_x);
    Func_080a17c4(object);
}
