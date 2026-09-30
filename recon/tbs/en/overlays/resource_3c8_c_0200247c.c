/* NONMATCHING H6 (2026-09-27): exact DRIFT's explicit byte-store address
 * transferred only to the divergent camera motion_flags write. Prediction:
 * remove the member read/AND-zero ancestry, decouple this byte consumer from
 * the earlier reset HI zero, and remove the extra zero pool without losing
 * the admitted selected reset. Initial RTL now has a plain mem:QI write,
 * without that member read/AND sequence. But CSE still makes its source
 * subreg:QI(HI165), the reset's zero, live across the two calls. Full binary
 * is byte-identical to H4; full normalized diff and local-allocation dump
 * read: 624/616 bytes, 169 differing halfwords / 53 aligned edits. The extra
 * pool at +0x1e0, selected slot r6 instead of r5, and +8-byte extent survive.
 * Consumer-address ancestry is therefore insufficient here. Reject this
 * one-trial transfer; no wider byte-pointer conversion or producer sweep.
 * Trial retained at b4cb99e0d; the simpler member store is restored below.
 * Restoration compares byte-identically too. No credit.
 *
 * NONMATCHING H5 (2026-09-27): direct signed halfword reset access is
 * byte-identical to H4: 624/616 bytes, 169 halfwords / 53 aligned edits.
 * Exact linked-record consumers use direct halfword access at +100. That
 * transfer removes the initial RTL member read/AND-zero/write, but the
 * required SI-to-HI conversion (HI157) remains. CSE still turns it into
 * the camera byte's zero producer; its two-call lifetime and extra pool
 * survive. Full normalized diff and emitted assembly checked. Admission
 * fails: direct access is not enough to remove this pool. Trial is retained
 * at 6ddaa41d4; H4's simpler member view is restored below. This three-model
 * pass is stopped.
 * Further work needs a distinct producer/consumer lifetime fact, not more
 * signedness, loop, or cast spellings. No function or alignment credit.
 *
 * NONMATCHING H4 (2026-09-27): 624/616 bytes, 169 differing halfwords,
 * 53 aligned edits. Give the immediate height-reset flag result its own
 * block lifetime, separate from the earlier result surviving two calls.
 * First result SI43 drops from 14 refs/25 insns/two definitions to six
 * refs/15 insns/one definition; the new result is consumed directly in r0.
 * Actor r6, first result r7, selected slot r7, swap-coordinate block, and
 * final flag/loop update now match. H3's selected-reset admission remains.
 * Full normalized diff read. Extra HI-zero pool still makes the function
 * eight bytes too long and extends the height-test branch. No credit.
 *
 * NONMATCHING H3 (2026-09-27): local reset owner, 628/616 bytes,
 * 205 differing halfwords / 109 aligned edits. The two block reset sites
 * share a word-valued inline parameter for target/motion/height state.
 * Full normalized diff: the selected block's complete reset sequence now
 * matches, including pointer r3/zero r2. Frame and signed decrements stay
 * admitted. The primary pool prediction FAILED: inline constant expansion
 * still creates a dead HI zero which CSE uses for the later camera byte;
 * the extra zero pool and +12-byte extent remain. Retain the local reset
 * admission, not a claim of exactness. No function or alignment credit.
 *
 * Prior NONMATCHING: 628/616 bytes, 205 differing halfwords, 118 aligned edits.
 * 2026-09-26 own-ROM audit: 0200247c..020026e4 includes the sole pool word
 * 020026e0 = callback 0200a2a5. The 132-byte frame has a full 112-byte actor
 * scratch record at sp+20, not the old single s32 local with out-of-bounds
 * coordinate stores. Old header: 676 bytes / 254 differing HW / 147 edits.
 * H0 typed reconstruction: 624 bytes / 279 HW / 162 edits. Correct camera
 * getter (0808a228), void setters, signed search and duplicate actor lookups.
 * H1 explicit loop/swap blocks, post-EventBegin scratch lifetime:
 * 636 bytes / 203 HW / 121 edits; no extra flag induction variable.
 * H2 signed height-index view from exact LOWER_BLOCKS:
 * 628 bytes / 205 HW / 118 edits; both decrements now use subs, no 0xffff.
 * Astra 2026-09-27: model index as a signed 16-bit field in a word,
 * transferring the ship-row counter model. Result is binary-identical:
 * 624/616 bytes, 169 halfwords / 53 edits. The HI zero still supplies the
 * later camera byte and the extra pool survives. Retain the s16 view.
 * Both permitted variants completed. Remaining: actor/flag-result/selected
 * slot lifetimes, extra zero pool before height-index branch, and branch
 * reach. Exact SETTLE_BLOCKS rechecked at 780/780 bytes. No adoption.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 kind);
void OverlayObject_WaitUntilIdle(struct FieldActor *object);
void SceneActor_PickHighestSlotAtSameTileAndRelease(s32 actor);
s32 SceneActor_SetHeightAboveLinkedRecord(struct FieldActor *object);
void VinasuHeya_LowerFloatingBlocks(s32 wait);

/* LOWER_BLOCKS consumes this field as a signed height-table index. */
struct FloatingBlockHeight {
    u8 unknown_00[0x64];
    s16 index;
};

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2,
                            s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ void FloatingBlock_ResetMotion(struct FieldActor *block, s32 value)
{
    block->target_y = ACTOR_NO_TARGET;
    *(s32 *)block->unknown_14 = value;
    block->velocity_y = value;
    block->motion_flags = value;
    ((struct FloatingBlockHeight *)block)->index = value;
}

void VinasuHeya_ResolveFloatingBlock(void)
{
    struct FieldActor work;
    struct FieldActor *first = NULL;
    struct FieldActor *second = NULL;
    struct FieldActor *temp;
    struct FieldActor *block;
    struct FieldActor *other;
    s32 i;
    s32 id;
    s32 x;
    s32 z;
    s32 j;
    s32 slot;
    s32 none;

    Engine_EventBegin();
    temp = &work;
    i = 0;
    id = 10;
again:
    {
        block = Object_GetById(id);
        x = block->x.fixed >> 20;
        if (x == 13) {
            z = block->z.fixed >> 20;
            if (z == 7) {
                none = Engine_GameFlagIsSet(0x200 + i);
                if (none == 0) {
                    OverlayObject_WaitUntilIdle(block);
                    Engine_GameFlagSet(0x200 + i);
                    block->priority_flags |= 2;
                    block->collision_flags = none;
                    block->motion_flags = none;
                    Call6((void (*)())Engine_MapCopyCellAttributes, 4, 19, 1, 1, x, z);
                    goto done;
                }
            }
        }
        if (block->sprite->priority == 3 && !Engine_GameFlagIsSet(0x200 + i)) {
            Engine_ActorSetSpritePriority(id, 1);
            *(s32 *)block->unknown_44 = 0;
            if (block->z.fixed >> 20 <= 12) {
                first = OverlayObject_SpawnWithMode14(block->x.fixed, 0, 0xe00000, 253);
                second = OverlayObject_SpawnWithMode14(block->x.fixed, 0, 0xf00000, 253);
            }
            OverlayObject_WaitUntilIdle(block);
            Engine_ActorSetPosition(id, 0, 0);
            Engine_ObjectDispatchRelease(first);
            Engine_ObjectDispatchRelease(second);
            Engine_GameFlagSet(0x200 + i);
            goto done;
        }
        goto check_height;

swap_coords:
        other = Object_GetById(j + 10);
        temp->x.fixed = block->x.fixed;
        temp->y.fixed = block->y.fixed;
        temp->z.fixed = block->z.fixed;
        block->x.fixed = other->x.fixed;
        block->y.fixed = other->y.fixed;
        block->z.fixed = other->z.fixed;
        other->x.fixed = temp->x.fixed;
        other->y.fixed = temp->y.fixed;
        other->z.fixed = temp->z.fixed;
        slot = j;
        goto apply_height;

check_height:
        if (block->z.fixed >> 20 != 19) {
            goto next;
        }
        {
            s32 clear = Engine_GameFlagIsSet(0x200 + i);

            if (clear != 0) {
                goto next;
            }
            FloatingBlock_ResetMotion(block, clear);
        }
        j = 0;
        slot = i;
        if (j < i) {
            do {
                if (!Engine_GameFlagIsSet(0x200 + j)) {
                    goto swap_coords;
                }
                j++;
            } while (j < i);
        }
apply_height:
        other = Object_GetById(slot + 10);
        FloatingBlock_ResetMotion(other, 0);
        Camera_SetSpeed(0x30000, 0x6000);
        Engine_EventGetViewCenter()->motion_flags = 0;
        Camera_MoveTo(0x880000, 0x80000, 0x1580000, 1);
        Engine_CameraWaitForMove();
        SceneActor_PickHighestSlotAtSameTileAndRelease(slot + 10);
        other = Object_GetById(slot + 10);
        if (other->x.fixed >> 20 == 6) {
            ((struct FloatingBlockHeight *)Object_GetById(8))->index++;
            ((struct FloatingBlockHeight *)Object_GetById(9))->index--;
        } else {
            ((struct FloatingBlockHeight *)Object_GetById(8))->index--;
            ((struct FloatingBlockHeight *)Object_GetById(9))->index++;
        }
        other = Object_GetById(slot + 10);
        other->update = (void (*)(union FieldObject *))SceneActor_SetHeightAboveLinkedRecord;
        VinasuHeya_LowerFloatingBlocks(40);
        Object_GetById(slot + 10)->priority_flags |= 2;
        Engine_GameFlagSet(slot + 0x200);
        goto done;
    }
next:
    i++;
    id++;
    if (i <= 3) {
        goto again;
    }
done:
    Engine_EventEnd();
}
