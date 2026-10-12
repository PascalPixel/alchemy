/* Near miss of Func_02001fec of overlay 696 (56 instructions, 2 differ): the native builds the
 * source address and the size before it loads the sprite's block number for
 * VramBlock_LoadCached; the draft loads the block number first. Measured: a "+r" barrier on
 * the source (4 differ) or on source and size (2 differ) fixes that call but moves the shift of
 * the earlier 0x608 one row; a one-pass block before the call takes other registers (29). Tried 2026-10-12: the source address in a
 * temporary behind an empty asm barrier leaves the block number above the size and moves the
 * 0x608 shift below two stores (4 differ). */
#include "../../../../games/tla/src/field/overlay_696/ov696.h"

u8 *Runtime_AllocateHeapBlockFar(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
void VramBlock_LoadCached(s32 block, s32 size, const u8 *source);
void Func_02004884(s32 resource);

struct Ov696Object *Func_02001fec(void)
{
    struct Ov696Object *object = Engine_ObjectCreate(489, 0x780000, 0x400000, 0x1080000);
    u8 *buffer = NULL;

    if (object == NULL)
        return NULL;
    {
        struct FieldSprite *sprite = object->sprite;

        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 1;
        object->motion_flags = 0;
        object->field_5c = 1;
        sprite->next_frame = 0;
        sprite->unknown_1b[0] = 0;
        buffer = Runtime_AllocateHeapBlockFar(68, 0x608);
        Func_02004884(65);
        VramBlock_LoadCached(sprite->vram_block, 128, buffer + 0x400);
        Runtime_ReleaseHeapBlock(68);
    }
    return object;
}
