/* Draft of Func_02003a30 (overlay 655), differ 22 of 48: the native loads the
 * address of Data_02004420 into r5 before the spawn call and keeps the zero
 * for the three byte stores in r7 (set after the call); this loads the
 * address after the call into r3 and keeps the zero in r1. */
#include "types.h"
#include "mem.h"
#include "resource.h"
#include "object_runtime.h"
#include "field_sprite.h"

extern struct ObjectRuntime *Data_02004420;

struct ObjectRuntime *Func_02003bb0(s32 id, s32 x, s32 y, s32 z);
void Func_02003c18(s32 id);
void *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);

void Func_02003a30(void)
{
    struct ObjectRuntime *object;
    s32 zero;

    object = Data_02004420 = Func_02003bb0(489, 0, 0, 0);
    zero = 0;
    if (object != NULL) {
        struct FieldSprite *sprite = (struct FieldSprite *)object->animation;
        u8 *buffer;

        sprite->full_color = 0;
        sprite->palette = 0;
        *((u8 *)object + 85) = zero;
        *((u8 *)object + 92) = 1;
        ((u8 *)sprite)[26] = zero;
        ((u8 *)sprite)[27] = zero;
        buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_02003c18(199);
        VramBlock_LoadCached(sprite->vram_block, 128, buffer + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
}
