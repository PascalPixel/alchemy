/* Near miss of Func_02000c40 of overlay 682 (32 instructions, 2 differ): the native reads the
 * frames from the stack before it loads the pool zero that masks the halfword field; the draft
 * loads the zero first (one pair of rows). Both loads have the same priority and the zero's
 * insn comes first in RTL order. Measured: a local copy of the frames (same), a u16 parameter
 * (extended at entry), barriers that name the frames (the parameter is then loaded at entry). */
#include "types.h"
#include "field_event.h"

extern const u8 Data_02003620[];

struct FieldActor *Func_020029ec(s32 type, s32 x, s32 y, s32 z);
void Func_020029dc(struct FieldActor *object, s32 mode);
void Func_020029e4(struct FieldActor *object, const void *script);
struct FieldActor *Object_GetById(s32 id);
void Func_02002bc4(s32 cue);
void Battle_WaitMode0(s32 frames);

void Func_02000c40(s32 x, s32 y, s32 z, s32 angle, s32 frames)
{
    s16 value = angle;
    struct FieldActor *object;
    struct FieldSprite *sprite;

    object = Func_020029ec(0x1e8, x, y, z);
    if (object != 0) {
        sprite = object->sprite;
        Func_020029dc(object, 1);
        Func_020029e4(object, Data_02003620);
        object->motion_flags = 0;
        { s32 count = frames; ((struct { u8 unknown_00[94]; u16 frames; } *)object)->frames = count; }

        sprite->next_frame = 0;
        sprite->rotation = value;
    }
}
