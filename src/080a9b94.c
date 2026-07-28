#include "a9_motion.h"
#include "types.h"

#define M2C_FIELD(base, type, offset)     (*(type)((u8 *)(base) + (offset)))

void Func_080a9b94(s32 origin_x, s32 origin_y, s32 phase) {
    s32 index;
    struct Object080a9bd8 *object;
    struct Object080a9bd8 **objects;

    index = 0;
    objects =
        (struct Object080a9bd8 **)(*(s32 *)0x03001F2C + 0x48);
    do {
        object = *objects++;
        if (object != NULL) {
            Func_080a9bd8(object, index, origin_x, origin_y, phase);
        }
        index += 1;
    } while (index <= 0x1F);
}
