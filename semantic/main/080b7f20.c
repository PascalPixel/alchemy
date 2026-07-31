#include "types.h"

/*
 * Camera hook for one battle participant: project the object's position into
 * the caller's screen-space triple, then scale the projection result by the
 * participant record's field at +0x18.  The scaled value is not stored - the
 * call is made for the side effect the ARM helper has on the projection state
 * - and the hook always reports zero.
 *
 * `ldr r3,[pc,#24]` loads 0x03000118, the ARM fixed-point multiply relocated
 * into IWRAM, and `mov ip, pc ; bx r3` calls it: mov ip,pc sets the return
 * address to the instruction after the bx, so control resumes in the same
 * frame with the live registers intact.  It is an ordinary indirect call, as
 * already established for 0x0800ebec, and is written as one below.  The
 * `movs r0, r0` in front of it is alignment filler.
 *
 * Reached only as a published Thumb pointer at 0x080b50ac.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080b7f20)(s32 a, s32 b);

struct Object_080b7f20 {
    u8 unknown_00[8];
    s32 position_08[3];
};

struct Record_080b7f20 {
    u8 unknown_00[0x18];
    s32 scale_18;
};

struct Slot_080b7f20 {
    struct Object_080b7f20 *object;
};

struct Slot_080b7f20 *Func_080b7dd0(s32 id);
struct Record_080b7f20 *Func_080b7f70(struct Object_080b7f20 *object, s32 kind);
void Func_080b7ed8(void);
s32 Func_08005268(const s32 *source, s32 *projected);

s32 Func_080b7f20(s32 id, s32 *projected)
{
    Multiply_080b7f20 multiply = (Multiply_080b7f20)0x03000118;
    struct Object_080b7f20 *object = Func_080b7dd0(id)->object;
    struct Record_080b7f20 *record = Func_080b7f70(object, 0);
    s32 position[3];

    Func_080b7ed8();

    position[0] = object->position_08[0];
    position[1] = object->position_08[1];
    position[2] = object->position_08[2];

    (void)multiply(Func_08005268(position, projected), record->scale_18);
    return 0;
}
