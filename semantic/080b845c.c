#include "types.h"

/*
 * Camera hook for one battle participant, the sibling of 0x080b7f20: project
 * the object's position triple into the caller's screen-space triple, scale
 * that projection result by the participant record's field at +0x18, scale it
 * again by the whole part of the object's palette word, and subtract the
 * product from the projected Y.  The hook always reports zero.
 *
 * Unlike 0x080b7f20 this owner hands the object's own position array to the
 * projector instead of copying it into a local first; the 12 bytes of stack
 * it reserves at 0x080b8464 are never touched.
 *
 * The two `mov ip, pc ; bx r5` sequences at 0x080b848c and 0x080b84a0 call
 * the pooled 0x03000118, the ARM fixed-point multiply relocated into IWRAM.
 * `mov ip, pc` sets the return address to the halfword after each `bx`
 * (0x080b8490 and 0x080b84a4), and both landing sites consume r0 as that
 * call's result in the same frame with the live registers intact - the
 * ordinary indirect call already established for 0x0800ebec, not retained
 * assembly.  They are written below as calls through a function pointer.
 * The `movs r0, r0` at 0x080b849e in front of the second is alignment filler.
 *
 * Excluded (4 bytes): the one-word pool at 0x080b84bc holding 0x03000118.
 * The single epilogue is at 0x080b84ae, reached only by fall-through.  Two
 * direct callers, at 0x080c1864 and 0x080c18f0, and published as a Thumb
 * pointer at 0x080b50b4.
 */

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080b845c)(s32 a, s32 b);

struct Object_080b845c {
    u8 unknown_00[8];
    s32 position_08[3];
};

struct Record_080b845c {
    u8 unknown_00[0x18];
    s32 scale_18;
};

struct Slot_080b845c {
    struct Object_080b845c *object;
};

struct Slot_080b845c *Func_080b7dd0(s32 id);
struct Record_080b845c *Func_080b7f70(struct Object_080b845c *object,
                                      s32 kind);
void Func_080b7ed8(void);
s32 Func_08005268(const s32 *source, s32 *projected);
u32 Func_080b8530(s32 id);

s32 Func_080b845c(s32 id, s32 *projected)
{
    Multiply_080b845c multiply = (Multiply_080b845c)0x03000118;
    struct Object_080b845c *object = Func_080b7dd0(id)->object;
    struct Record_080b845c *record = Func_080b7f70(object, 0);
    s32 scaled;

    Func_080b7ed8();

    scaled = multiply(Func_08005268(object->position_08, projected),
                      record->scale_18);
    scaled = multiply(scaled, (s32)Func_080b8530(id) >> 16);
    projected[1] -= scaled;
    return 0;
}
