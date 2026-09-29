/* NONMATCHING: resource_39a at 0x02009a34 (124 bytes with its pool), the
 * trigger hop, before FIELD/COMMON/IMIRU_FUCHIN/DRAGON_EYE.C, stays listing.
 * It was FIELD/COMMON/IMIRU_FUCHIN/CAMERA_STEP.C.
 *
 * Remaining difference: the reference loads the scene numbers 0x3f and 0x40
 * from its literal pool and compares registers, as link-time scene numbers
 * do; plain constants compile to cmp with an immediate. With both numbers as
 * link-time values this body compiles to the reference exactly.
 */
#include "../../../../../games/THE BROKEN SEAL/SRC/FIELD/COMMON/IMIRU_FUCHIN/IMIRU_FUCHIN.H"

/* Hop the leader across the gap the touched trigger stands for. */
void ImiruFuchin_HopOnTrigger(void)
{
    s32 trigger = gEventWork->touched_trigger;

    if (gGameState.scene == 0x3f) {
        if (trigger == 17) {
            ImiruFuchin_HopBy(0, -32);
        } else {
            ImiruFuchin_HopBy(-32, 0);
        }
    }
    if (gGameState.scene == 0x40 && trigger == 25 && GameFlag_IsSet(0x309)) {
        ImiruFuchin_HopBy(0, 32);
    }
}
