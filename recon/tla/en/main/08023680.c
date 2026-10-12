/* Near miss: differ 2 of 19 (one reordered pair). The native reads the flag byte before it
 * masks the value (ldrb r2, [r0, #5]; ands r1, r3); the draft masks first. sched2 has the two
 * at equal priority 5 and prefers the mask as the dependent of the last scheduled insn
 * (movs r3, #3). Tried without gain: a child or byte pointer local, an explicit
 * (byte & ~0xc) | ((value & 3) << 2) (differ 10 and more: r4 is taken), a one-pass block
 * before the store, an empty asm on the value or on the child (differ 12), u8 and s32 value.
 * Both have two dependents, so the tie falls to RTL order, and the bitfield store emits the
 * mask of the value before the read of the byte: the native's source read the byte first.
 * The permuter (3 minutes) found nothing nearer. Animation_SetDisplayFlag is the same case. */
#include "object_dispatch.h"

void Animation_SetStateFlags(struct DispatchObject *object, u32 value)
{
    if (object != NULL && object->kind == 1)
        object->target.child->state_flags = value;
}
