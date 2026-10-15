/* Draft, differ 2 of 39: the listing copies the object out of r0 after it stores params.kind (add r4; movs; str; adds r5, r0); this draft copies it first. Tried: a pointer local for the params (same); permute 3 minutes found nothing. */
#include "types.h"
#include "object_runtime.h"

struct SpawnParams {
    s32 kind;
    s32 part;
    u8 unknown_08[32];
};

void Func_02000480(s32 x, s32 y, s32 z, s32 target_x, s32 target_y, s32 target_z, s32 flags, struct SpawnParams *params);
void Func_02003328(s32 src_x, s32 src_y, s32 dst_x, s32 dst_y, s32 width, s32 height);

s32 Func_02000794(struct ObjectRuntime *object)
{
    struct SpawnParams params;
    s32 x;
    s32 y;

    params.kind = 2;
    params.part = *((u8 *)object + 98);
    Func_02000480(object->x, object->y, object->z, 0, 0x10000, 0, 0x30000, &params);
    x = (object->bounce >> 20) + 64;
    y = object->gravity >> 20;
    Func_02003328(x, y + 26, x, y, 1, 1);
    return 0;
}
