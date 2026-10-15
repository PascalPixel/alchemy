#include "types.h"

struct DescriptorEntry {
    s16 id;
    u8 unknown_02[2];
    s16 range;
    u8 unknown_06[2];
};

struct DescriptorEntry *Func_080cda58(s32 id);
s32 Func_08020310(s32 x, s32 z, s32 layer, s32 height);

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 Func_080cda84(s32 id, u8 *object, s32 *position)
{
    struct DescriptorEntry *entry = Func_080cda58(id);
    s32 height = Func_08020310(position[0], position[2], object[34], *(s32 *)(object + 20));

    if (ABS(height) <= entry->range << 16)
        return ABS(height);
    return -1;
}
