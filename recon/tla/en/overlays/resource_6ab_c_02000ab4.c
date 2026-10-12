/* Near miss of Func_02000ab4 (20 instructions, 2 differ): the native loads the destination byte
 * (ldrb r1, [r4, #9]) before it builds the mask 12 (movs r2, #12); the draft builds the mask
 * first. In sched2 the two are ready together and the mask has priority 38 against the load's
 * 37: its chain runs through the anti-dependence on r3 (ands r2, r3 before movs r3, #13).
 * Measured with the one-byte packed copy overlay 657's Func_02000be0 matches through:
 *  - plain bit-field assignment (the earlier draft): the same differ 2;
 *  - from, to, bits (this order of locals): the destination takes r0, not r4 (differ 6);
 *  - to, bits, from: the pointer and the byte trade r1 and r4 (differ 8);
 *  - a loop note after the two byte reads, or after the priority is read into a local:
 *    both pointers and bytes load destination first (differ 4);
 *  - an asm barrier on the owner in its place: the same differ 4.
 * The permuter (6 minutes, 3 jobs, 2,700 candidates) found nothing nearer than differ 2.
 * Next: a form in which the destination load is ready one step before the mask, or the
 * note falls between the source byte's mask and the destination's. */
#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct SpriteBits {
    u8 unknown_0 : 2;
    u8 priority : 2;
} __attribute__((packed));

struct SpriteAttributes {
    u8 unknown_00[9];
    struct SpriteBits bits;
};

struct ObjectRuntime *Object_GetById(s32 id);

void Func_02000ab4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *owner = Object_GetById(gPartyState.current_owner);
    struct SpriteAttributes *to = (struct SpriteAttributes *)object->animation;
    struct SpriteAttributes *from = (struct SpriteAttributes *)owner->animation;
    struct SpriteBits source = from->bits;
    struct SpriteBits bits = to->bits;

    bits.priority = source.priority;
    to->bits = bits;
}
