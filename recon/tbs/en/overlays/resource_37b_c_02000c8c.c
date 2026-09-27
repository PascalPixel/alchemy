/* NONMATCHING: 548 bytes, candidate 544, 94 differing halfwords, 62 aligned
 * edits (2026-09-27). Whole extent 02000c8c..02000eb0 includes four pool words.
 * Sol burst audit (2026-09-27): fresh full normalized comparison reproduces
 * 544/548, 94 halfwords / 62 edits; no exact or equivalent sibling exists.
 * The new DRIFT witness isolates an address producer to change sched2 order.
 * This owner's missing final loop copies originate earlier, when global PRE
 * replaces the last two frame-address producers with shared pseudo 110.
 * The restart producer-scope transfer also failed address rematerialization.
 * No evidence predicts that another scoped copy here will preserve all four
 * loop-entry copies. Do not replay the closed type/inline/phase-pointer axes;
 * retain this canonical body until a distinct pointer producer is evidenced.
 * No function or alignment credit; both separate two-byte gaps are unchanged.
 * H1 transfers MAP_SCROLL.H's MapScrollWork.origin and signed coordinate
 * array from exact Map_UpdateLayerScroll; OpenSeal proves the caller and
 * GuardedStep supplies the actor import bindings. Fresh original: 544/548,
 * 96 halfwords / 63 aligned edits. Typed ownership fixes the second branch's
 * saved-origin load before the final coordinate store, but does not retain
 * the two final loop-entry pointer copies. Allocator pseudo 114 still has
 * 20 uses across 148 instructions / 21 calls and lands in r6, not r7.
 * Admission invariant: distinct pos and loop pointers at all four loops.
 * H1 fails it; stop this type/address axis, do not tune around the missing
 * copies. Exact sibling/header sources remain untouched. Unit registration
 * exposes this previously unindexed not-yet-C owner; it grants no DONE.
 *
 * Phase-reuse trial H1 (2026-09-27): one void *p owns the actor, then the
 * local camera position, transferring Vault's proven pointer-phase model.
 * Complete normalized diff and binary comparison are unchanged: 544/548,
 * 94 halfwords / 62 aligned edits. Still only two loop-entry copies, so
 * admission fails; no DONE. Normal and diagnostic compiler text agree.
 * Local CSE retains user pseudo 34 for all four position loops. GCSE PRE
 * then recognizes all four sfp-12 addresses: expression 4 creates shared
 * pseudo 110, replaces the final two address producers (insns 437/478),
 * and propagates 110 directly into their loads/stores (450/452, 491/493).
 * The shared address has 20 uses / 148 instructions / 21 calls and gets r6;
 * user 34 is left with only the actor lifetime. This is global expression
 * sharing before loop optimization, not a late allocator copy decision.
 * Flattening ShiftFocus cannot undo it: local CSE already uses parent p
 * directly. The saved camera pointer already begins after walk/face/wait,
 * as in the ROM; delaying that lifetime supplies no new boundary. Stop
 * this phase-pointer axis; do not repeat inline/declaration/type variants.
 * Rejected H1 is preserved at 92f409fb1; the canonical typed body below is
 * restored unchanged. Continuation needs evidence for a genuinely distinct
 * pointer producer, not another spelling of the same local array address.
 *
 * Earlier baseline evidence (2026-09-24):
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * the reference keeps
 * pos in r7 and copies it into a separate pointer (r5) at every ShiftFocus
 * loop, including the two after the door flashes, with the counter in r6;
 * here the inline parameter is copied only in the first two loops (pos is
 * dead after the last ones, so the copy coalesces), pos lands in r6 and the
 * counter in r5, and the flash loops keep 2 in r7. Loops count up with != so
 * they are not reversed; the pointer is passed as the array so the inline
 * copies it. A shared p pointer variable (p = pos before each loop), a shared
 * counter, and an explicit copy inside the inline all still give 96: cse
 * folds p back into the frame address, so the copy never survives
 * (2026-09-24, ovl8a). */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "MAP_SCROLL.H"

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the camera focus up or down one pixel a frame for thirty frames. */
static __inline__ void ShiftFocus(s32 *p, s32 step)
{
    s32 i;

    for (i = 0; i != 30; i++) {
        p[2] += step;
        Engine_EventWait(1);
    }
}

/* Swaps the two door frames, holding each for the given frames. */
static __inline__ void FlashDoor(s32 count, s32 frames)
{
    s32 i;

    for (i = 0; i != count; i++) {
        Engine_MapCopyCellsTo(2, 28, 34, 10, 4, 2);
        Engine_EventWait(frames);
        Engine_MapCopyCellsTo(2, 30, 34, 10, 4, 2);
        Engine_EventWait(frames);
    }
}

void Local_02000c8c(void)
{
    struct MapScrollWork *camera;
    s32 *saved;
    s32 pos[3];
    struct FieldActor *leader;
    s32 side;

    camera = Data_03001e70;
    leader = Engine_ActorGet(0);
    if (leader->z.fixed < 0xb30000) {
        Call3((void (*)())Engine_ActorWalkToAndWait, 0, 0x23f, 132);
        Call3((void (*)())Engine_ActorFaceDirection, 0, 0x4000, 0);
        Engine_EventWait(30);
        saved = camera->origin;
        pos[0] = leader->x.fixed;
        pos[1] = leader->y.fixed;
        pos[2] = leader->z.fixed;
        camera->origin = pos;
        ShiftFocus(pos, 0x10000);
        Engine_EventWait(40);
        side = 1;
    } else {
        Call3((void (*)())Engine_ActorWalkToAndWait, 0, 0x241, 222);
        Call3((void (*)())Engine_ActorFaceDirection, 0, 0xc000, 0);
        Engine_EventWait(30);
        pos[0] = leader->x.fixed;
        pos[1] = leader->y.fixed;
        pos[2] = leader->z.fixed;
        saved = camera->origin;
        camera->origin = pos;
        ShiftFocus(pos, -0x10000);
        Engine_EventWait(40);
        side = 2;
    }
    FlashDoor(6, 8);
    FlashDoor(10, 4);
    FlashDoor(12, 2);
    Engine_MapCopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_MapCopyCellsTo(8, 55, 32, 40, 8, 4);
    Engine_EventWait(60);
    if (side == 1)
        ShiftFocus(pos, -0x10000);
    else if (side == 2)
        ShiftFocus(pos, 0x10000);
    camera->origin = saved;
}
