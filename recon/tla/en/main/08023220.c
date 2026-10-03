/* CANONICAL DRAFT, 2026-10-02, uncredited.
 * Four-neighbor family before existing TLA DISPATCH.C:36/88/392/40 bytes.
 * Retained whole-module candidate876/native876,68 differences all six;
 * lookup36/0,release88/0,constructor392/62,initialize40/6.
 * Existing320-byte suffix incl all pools/calls remains0diff;120 whole-module
 * actual BL/pool operands equal. Raw resource create336/release72 independently
 * symbolic-linked complete all six; their proposed production names remain
 * absent. These private context links establish physical identities, not credit.
 * Callee pointer declarations are local draft views. Closing the actual
 * shared API is still required before these calls can be adopted.
 * Below records are English instruction/pool scores; each tuple is extent/diff
 * for lookup,release,constructor,initialize, then whole extent/diff. Retained
 * ordinary baseline and every retained phase were also whole-linked all six.
 * plain_typed_bank:36/4,88/15,392/78,40/6;whole876/103
 * early_return_and_for:36/4,88/15,392/223,40/6;whole876/248
 * sequential_cursor:36/4,88/15,388/257,40/6;whole872/613
 * kind_if_chain:36/4,88/15,392/130,40/6;whole876/155
 * branch_child_lifetimes:36/4,88/15,392/139,40/6;whole876/164
 * coordinate_locals:36/4,88/15,392/170,40/6;whole876/195
 * scale_unit_local:36/4,88/15,392/78,40/6;whole876/103
 * metadata_local:36/4,88/15,392/78,40/6;whole876/103
 * lookup_tied_outputs:36/0,88/15,392/78,40/6;whole876/99
 * release_pointer:36/0,88/0,392/78,40/6;whole876/84
 * constructor_pointer:36/0,88/0,392/70,40/6;whole876/76
 * constructor_store_order:36/0,88/0,392/62,40/6;whole876/68
 * Phase has four forms total; no further trials. Retained boundaries only tie
 * initialized, used pointer/result values; no new reads,volatile,unused frame
 * storage,instruction ASM,compiler options or output patch. Dma_Set unchanged.
 * S3 whole876 adoption remains blocked by constructor/initialize mismatches.
 * A separate adjacent lookup/release prefix can belong to the moved existing
 * TBS OBJECT-to-COMMON owner after real API, source-bank and game/order proof;
 * constructor/initialize then stay raw before the unchanged DISPATCH320 owner.
 * Local resource pointer declarations below are explicitly draft views,
 * not a closed production API. The opaque actual-owner API is a prerequisite.
 */

/* This listing proposal owns FieldObject_Create only; native complete function/pool390 bytes.
 * Its392-byte raw listing also carries a trailing anonymous zero
 * alignment halfword; this standalone draft emits no final halfword.
 * Unmarked internal compiler alignment can count under the current C2 span
 * convention; final linker gaps do not. No counting marker is introduced. */

#include "OBJECT_RUNTIME.H"
#include "METADATA_LOOKUP.H"
#include "OBJECT_DISPATCH.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "DMA.H"

/* LOCAL DRAFT VIEW:observed fields of the existing128-byte runtime owner.
 * This self-contained attempt does not add a maintained second header/type. */
struct ObjectRuntimeDraft {
    s32 *script;
    s16 step;
    u16 angle;
    s32 x;
    s32 y;
    s32 z;
    s32 terrain_height;
    s32 scale_x;
    s32 scale_y;
    u16 radius;
    u8 terrain_id;
    u8 unknown_23;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 speed_limit;
    s32 acceleration;
    s32 target_x;
    s32 target_y;
    s32 target_z;
    s32 value_44;
    s32 value_48;
    s32 value_4c;
    void *animation;
    u8 animation_kind;
    u8 flags;
    u8 motion_flags;
    u8 unknown_57[2];
    u8 value_59;
    u8 action_flags;
    u8 movement_state;
    u8 unknown_5c[0x04];
    void *callback;
    /* Motion/visual code uses the first word as an angle target; construction
       initializes the two tile-coordinate words. */
    union {
        struct { s16 action; s16 unknown; } motion;
        struct { s16 x; s16 z; } tile;
    } value_64;
    struct ObjectRuntimeDraft *linked_object;
    u8 unknown_6c[0x04];
    s32 movement_dx;
    s32 movement_dy;
    s32 movement_dz;
    struct ObjectRuntimeDraft *support_object;
};

/* LOCAL DRAFT VIEW:92-byte object/camera work at heap slot+24; only the
 * observed signed constructor cursor is described, not a child-array capacity. */
struct ObjectDraftWork {
    u8 unknown_00[24];
    s32 child_list_cursor;
    u8 unknown_1c[64];
};

struct DispatchChild *ResourceObject_Create(s32 resource);
void *ObjectDispatch_FindFreeObject(void);

typedef char RuntimeDraft_size[sizeof(struct ObjectRuntimeDraft)==128?1:-1];

struct ObjectRuntimeDraft *FieldObject_Create(s32 resource, s32 x, s32 y, s32 z)
{
    struct ObjectRuntimeDraft *object;
    struct DispatchChild *child;
    struct DispatchChild **entry;
    struct ObjectDraftWork *work;
    s32 kind;
    u32 zero;
    u32 *src;

    kind = resource /4096;
    resource &=0xfff;
    object = ObjectDispatch_FindFreeObject();
    if (object !=0) {
        object->radius =16;
        switch (kind) {
        case 0:
            child = ResourceObject_Create(resource);
            if (child !=0) {
                object->animation_kind =1;
                object->animation = child;
                object->radius = Resource_GetMetadataRecordFar(resource)->box_y >>1;
            } else {
                object->animation_kind =0;
            }
            break;
        case 2:
            work = Ram_HeapSlots->unknown_18[0];
            entry = (struct DispatchChild **)((u8 *)work +8 +work->child_list_cursor++ *4);
            object->animation_kind =kind;
            src =&zero;
            /* FAKEMATCH:ordinary constructor392/78 includes16 differing bytes in the real DMA-zero pointer/store/setup sequence. As with the measured release, tie only the initialized used src pointer before its existing store; no zero-word read. */
            asm("" : "+r"(src));
            object->animation = entry;
            *src =0;
            Dma_Set(src, entry, 0x85000004, REG_DMA3);
            child = ResourceObject_Create(resource);
            if (child !=0) {
                object->radius = Resource_GetMetadataRecordFar(resource)->box_y >>1;
                *entry++ = child;
            }
            child = ResourceObject_Create(resource +1);
            if (child !=0)
                *entry = child;
            break;
        }
    }
    if (object !=0) {
        Object_SetPositionAndResetMotion((struct ObjectRuntime *)object, x, y, z);
        object->script = (s32 *)ObjectDispatch_Table6;
        object->speed_limit =0x20000;
        object->step =0;
        object->scale_x =0x10000;
        object->scale_y =0x10000;
        object->acceleration =0x10000;
        object->flags =3;
        object->value_48 =0x10000;
        object->value_44 =0x4000;
        object->value_59 =0;
        object->action_flags =1;
        object->value_4c =0;
        object->angle =0x4000;
        object->value_64.tile.x = x /65536;
        object->value_64.tile.z = z /65536;
        object->support_object =0;
        object->movement_dx =0;
        object->movement_dy =0;
        object->movement_dz =0;
    }
    return object;
}
