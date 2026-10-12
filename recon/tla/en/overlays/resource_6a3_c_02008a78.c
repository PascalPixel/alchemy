/*
 * Draft of overlay 6a3 Func_02000a78. 2 instructions differ: the listing
 * begins the angle step (movs r3, #182) before it loads the frame count and
 * finishes it after (lsls r3), the draft loads the count first. Tried: both
 * operand orders, Value1 for the sine call (same 2), a barrier on the frame
 * count (10). The cue call goes through Call2 (direct: 4 differ).
 */
#include "types.h"
#include "field_event.h"
#include "call.h"

#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern u32 gFrameCount;

struct FieldActor *Object_GetById();
void Func_020048d4();
s32 Math_Sine();

s32 Func_02000a78(void)
{
    struct FieldActor *object = Object_GetById(1);

    if (object != NULL) {
        u8 *sprite = (u8 *)WORD_AT(object, 80);

        if ((gFrameCount & 127) == 0)
            Call2(Func_020048d4, 29, 258);
        if (sprite != NULL) {
            sprite[23] = 30;
            sprite[22] = 252;
            HALF_AT(sprite, 18) = Value1(Math_Sine, gFrameCount * 728) / 128 - 0x800;
        }
    }
    return 1;
}
