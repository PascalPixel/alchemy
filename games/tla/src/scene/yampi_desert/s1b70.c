#include "types.h"
#include "object_runtime.h"

struct YampiDesertVisual {
    u8 unknown_00[5];
    u8 flags_05;
    u8 unknown_06[3];
    u8 flags_09;
    u8 unknown_0a[6];
    u8 tile_slot;
    u8 unknown_11[9];
    u8 value_1a;
    u8 value_1b;
};

struct ObjectRuntime *Func_02003fc0(s32 kind, s32 x, s32 y, s32 z);
void Func_02004030(s32 item);
u8 *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *source);

struct ObjectRuntime *Func_02001b70(void)
{
    struct ObjectRuntime *object = Func_02003fc0(489, 232 << 16, 32 << 16, 200 << 16);
    u8 *buffer = 0;

    struct YampiDesertVisual *visual;
    s32 mask;

    if (object == 0)
        return 0;
    {
        visual = object->animation;

        {
            s32 keep = 33;

            keep = -keep;
            visual->flags_05 &= keep;
        }
        mask = visual->flags_09 & 0x0f;
        {
            s32 clear = 13;

            clear = -clear;
            mask &= clear;
        }
        mask |= 4;
        visual->flags_09 = mask;
        object->motion_flags = 0;
        object->unknown_5c[0] = 1;
        visual->value_1a = 0;
        visual->value_1b = 0;
        buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_02004030(209);
        VramBlock_LoadCached(visual->tile_slot, 128, buffer + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
    return object;
}
