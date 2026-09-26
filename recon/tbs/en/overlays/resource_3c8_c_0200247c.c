/* NONMATCHING: 624/616 bytes, 279 differing halfwords, 162 aligned edits.
 * H0 recovers the exact 132-byte frame; loop topology and pools differ.
 * 2026-09-26 own-ROM audit: 0200247c..020026e4 includes the sole pool word
 * 020026e0 = callback 0200a2a5. The 132-byte frame has a full 112-byte actor
 * scratch record at sp+20, not the old single s32 local with out-of-bounds
 * coordinate stores. Old header: 676 bytes / 254 differing HW / 147 edits.
 * Copy the exact SETTLE_BLOCKS/LOWER_BLOCKS actor fields and map interface.
 * Correct the camera getter (0808a228), void camera setters, signed search
 * index, and the old duplicate ActorGet calls in height-index updates.
 */
#include "TYPES.H"
#include "FIELD_EVENT.H"

void *OverlayObject_SpawnWithMode14(s32 x, s32 y, s32 z, s32 kind);
void OverlayObject_WaitUntilIdle(struct FieldActor *object);
void SceneActor_PickHighestSlotAtSameTileAndRelease(s32 actor);
s32 SceneActor_SetHeightAboveLinkedRecord(struct FieldActor *object);
void VinasuHeya_LowerFloatingBlocks(s32 wait);

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
    struct FieldActor *temp = &work;
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
    for (i = 0, id = 10; i <= 3; i++, id++) {
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
                    break;
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
            break;
        }
        if (block->z.fixed >> 20 != 19) {
            continue;
        }
        none = Engine_GameFlagIsSet(0x200 + i);
        if (none != 0) {
            continue;
        }
        block->target_y = ACTOR_NO_TARGET;
        *(s32 *)block->unknown_14 = none;
        block->velocity_y = none;
        block->motion_flags = none;
        block->unknown_64 = none;
        slot = i;
        for (j = 0; j < i; j++) {
            if (!Engine_GameFlagIsSet(0x200 + j)) {
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
                break;
            }
        }
        other = Engine_ActorGet(slot + 10);
        other->target_y = ACTOR_NO_TARGET;
        *(s32 *)other->unknown_14 = 0;
        other->velocity_y = 0;
        other->motion_flags = 0;
        other->unknown_64 = 0;
        Camera_SetSpeed(0x30000, 0x6000);
        Engine_EventGetViewCenter()->motion_flags = 0;
        Camera_MoveTo(0x880000, 0x80000, 0x1580000, 1);
        Engine_CameraWaitForMove();
        SceneActor_PickHighestSlotAtSameTileAndRelease(slot + 10);
        other = Engine_ActorGet(slot + 10);
        if (other->x.fixed >> 20 == 6) {
            Engine_ActorGet(8)->unknown_64++;
            Engine_ActorGet(9)->unknown_64--;
        } else {
            Engine_ActorGet(8)->unknown_64--;
            Engine_ActorGet(9)->unknown_64++;
        }
        other = Engine_ActorGet(slot + 10);
        other->update = (void (*)(union FieldObject *))SceneActor_SetHeightAboveLinkedRecord;
        VinasuHeya_LowerFloatingBlocks(40);
        Engine_ActorGet(slot + 10)->priority_flags |= 2;
        Engine_GameFlagSet(slot + 0x200);
        break;
    }
    Engine_EventEnd();
}
