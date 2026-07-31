#include "types.h"

/*
 * Camera hook for one battle participant, the third of the family that also
 * holds 0x080b7f20 and 0x080b845c: project the object's position triple into
 * the caller's screen-space triple, scale that projection result by the
 * participant record's field at +0x18, scale it again by a fixed factor that
 * depends on the object's character id, and subtract the product from the
 * projected Y.  The hook always reports zero.
 *
 * The character id is the byte at +0x128 of the object block returned by
 * Func_08077008; Func_080c23c0 classifies it, and the factor is 24 for the
 * classified group and 48 otherwise.  The two arms share the scaling call and
 * everything after it, so only the factor is chosen per arm.
 *
 * The two `mov ip, pc ; bx r5` sequences at 0x080b84ec and 0x080b8514 call
 * the pooled 0x03000118, the ARM fixed-point multiply relocated into IWRAM.
 * `mov ip, pc` sets the return address to the halfword after each `bx`
 * (0x080b84f0 and 0x080b8518), and both landing sites consume r0 as that
 * call's result in the same frame with the live registers intact - the
 * ordinary indirect call already established for 0x0800ebec, not retained
 * assembly.  The `movs r0, r0` at 0x080b8512 in front of the second is
 * alignment filler.
 *
 * Excluded (6 bytes): the 2-byte `movs r0, r0` alignment filler at
 * 0x080b852a and the one-word pool at 0x080b852c holding 0x03000118.  The
 * single epilogue is at 0x080b8520, reached from both arms.  No direct
 * caller; published as a Thumb pointer at 0x080b50bc, next to the pointers
 * for its two siblings.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080b84c0)(s32 a, s32 b);

struct Object_080b84c0 {
    u8 unknown_00[8];
    s32 position_08[3];
};

struct Record_080b84c0 {
    u8 unknown_00[0x18];
    s32 scale_18;
};

struct Slot_080b84c0 {
    struct Object_080b84c0 *object;
};

struct Slot_080b84c0 *Func_080b7dd0(s32 id);
struct Record_080b84c0 *Func_080b7f70(struct Object_080b84c0 *object,
                                      s32 kind);
void Func_080b7ed8(void);
s32 Func_08005268(const s32 *source, s32 *projected);
u8 *Func_08077008(s32 id);
s32 Func_080c23c0(s32 character);

s32 Func_080b84c0(s32 id, s32 *projected)
{
    Multiply_080b84c0 multiply = (Multiply_080b84c0)0x03000118;
    struct Object_080b84c0 *object = Func_080b7dd0(id)->object;
    struct Record_080b84c0 *record = Func_080b7f70(object, 0);
    s32 scaled;
    s32 factor;

    Func_080b7ed8();

    scaled = multiply(Func_08005268(object->position_08, projected),
                      record->scale_18);

    if (Func_080c23c0(Func_08077008(id)[0x128]) != 0) {
        factor = 24;
    } else {
        factor = 48;
    }

    projected[1] -= multiply(scaled, factor);
    return 0;
}
