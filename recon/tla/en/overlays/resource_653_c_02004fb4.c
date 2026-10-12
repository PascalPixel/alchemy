/* Near miss, differ 28 of 51: the native loads the address of Data_020062ac
 * into r5 before the creation call and keeps a zero in r7 for the three
 * byte clears; the draft loads the address after the call and builds the
 * zero in r2. A slot pointer local leaves differ 22. */
#include "../../../../games/tla/src/field/overlay_653/local.h"

void Func_02004fb4(void)
{
    struct ObjectRuntime *object = Func_02005140(0x1e9, 0x1980000, 0x280000, 0x1580000);

    Data_020062ac = object;
    if (object) {
        struct FieldSprite *sprite = object->animation;
        u8 *buffer;

        sprite->full_color = 0;
        sprite->palette = 0;
        object->motion_flags = 0;
        object->unknown_5c[0] = 1;
        sprite->next_frame = 0;
        sprite->unknown_1b[0] = 0;
        buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_020051b0(198);
        VramBlock_LoadCached(sprite->vram_block, 128, buffer + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
}

