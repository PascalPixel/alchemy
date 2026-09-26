/* NONMATCHING: 628/616 bytes, 205 differing halfwords, 118 aligned edits.
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
        block = Engine_ActorGet(id);
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
        other = Engine_ActorGet(j + 10);
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
        none = Engine_GameFlagIsSet(0x200 + i);
        if (none != 0) {
            goto next;
        }
        block->target_y = ACTOR_NO_TARGET;
        *(s32 *)block->unknown_14 = none;
        block->velocity_y = none;
        block->motion_flags = none;
        ((struct FloatingBlockHeight *)block)->index = none;
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
        other = Engine_ActorGet(slot + 10);
        other->target_y = ACTOR_NO_TARGET;
        *(s32 *)other->unknown_14 = 0;
        other->velocity_y = 0;
        other->motion_flags = 0;
        ((struct FloatingBlockHeight *)other)->index = 0;
        Camera_SetSpeed(0x30000, 0x6000);
        Engine_EventGetViewCenter()->motion_flags = 0;
        Camera_MoveTo(0x880000, 0x80000, 0x1580000, 1);
        Engine_CameraWaitForMove();
        SceneActor_PickHighestSlotAtSameTileAndRelease(slot + 10);
        other = Engine_ActorGet(slot + 10);
        if (other->x.fixed >> 20 == 6) {
            ((struct FloatingBlockHeight *)Engine_ActorGet(8))->index++;
            ((struct FloatingBlockHeight *)Engine_ActorGet(9))->index--;
        } else {
            ((struct FloatingBlockHeight *)Engine_ActorGet(8))->index--;
            ((struct FloatingBlockHeight *)Engine_ActorGet(9))->index++;
        }
        other = Engine_ActorGet(slot + 10);
        other->update = (void (*)(union FieldObject *))SceneActor_SetHeightAboveLinkedRecord;
        VinasuHeya_LowerFloatingBlocks(40);
        Engine_ActorGet(slot + 10)->priority_flags |= 2;
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
