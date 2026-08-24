#include "types.h"

struct Root_080200cc {
    u8 pad_000[548];
    void *objects[4];
    s16 coords[8];
};

extern struct Root_080200cc *Data_03001f2c;

void Func_08009008(void *object, s32 *position, s32 *scale, s32 flags);

void Func_080200cc(void)
{
    struct Root_080200cc *root = Data_03001f2c;
    s32 scale[2];
    s32 position[4];
    s32 index;

    index = 0;
    do {
        void *object = root->objects[index];

        if (object != 0) {
            s32 unit = 0x10000;
            s32 bias = 0x1f40000;

            scale[0] = unit;
            scale[1] = unit;
            position[0] = root->coords[index] << 16;
            position[1] = bias;
            position[2] = (root->coords[index + 4] << 16) + bias;
            position[3] = 0;
            Func_08009008(object, position, scale, 0x4000);
        }
        index++;
    } while (index <= 3);
}
