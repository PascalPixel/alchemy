/* CANONICAL DRAFT, 2026-10-02, uncredited.
 * Four-neighbor family before existing TLA DISPATCH.C:36/88/392/40 bytes.
 * Retained whole-module candidate876/native876,68 differences all six;
 * lookup36/0,release88/0,constructor392/62,initialize40/6.
 * Existing320-byte suffix incl all pools/calls remains0diff;120 whole-module
 * actual BL/pool operands equal. Raw resource create336/release72 independently
 * symbolic-linked complete all six; their proposed production names remain
 * absent. These private context links establish physical identities, not credit.
 * Earlier maintained draft reported three halfword differences against TBS
 * source; the actual six-edition function/pool proof above supersedes it.
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

/* This listing proposal owns ObjectDispatch_Release only; native complete function/pool86 bytes.
 * Its88-byte raw listing also carries a trailing anonymous zero
 * alignment halfword; this standalone draft emits no final halfword.
 * Unmarked internal compiler alignment can count under the current C2 span
 * convention; final linker gaps do not. No counting marker is introduced. */

#include "OBJECT_DISPATCH.H"
#include "IO_REG.H"
#include "DMA.H"

/* LOCAL DRAFT VIEW:maintained role-specific prefix with opaque full-slot tail;
 * named motion-tail roles remain solely owned by the runtime view. */
struct DispatchObjectDraft {
    u32 value_00;
    s16 value_04;
    u8 unknown_06[0x2a];
    s32 value_30;
    s32 value_34;
    u8 unknown_38[0x18];
    union {
        struct DispatchChild *child;
        struct DispatchChild **children;
    } target;
    u8 kind;
    u8 unknown_55[2];
    u8 value_57;
    u8 unknown_58[3];
    u8 value_5b;
    u8 unknown_5c;
    u8 value_5d;
    u8 unknown_5e[6];
    s16 value_64;
    s16 value_66;
    s32 argument;
    u8 unknown_6c[4];
    /* The full TLA allocation has a16-byte motion tail owned by OBJECT_RUNTIME.H. */
    u8 unknown_70[16];
};

void ResourceObject_Release(struct DispatchChild *child);

typedef char DispatchDraft_size[sizeof(struct DispatchObjectDraft)==128?1:-1];

void ObjectDispatch_Release(struct DispatchObjectDraft *object)
{
    u32 zero;
    u32 *src;
    s32 count;
    struct DispatchChild **children;

    if (object !=0) {
        switch (object->kind &15) {
        case 1:
            ResourceObject_Release(object->target.child);
            break;
        case 2:
            children = object->target.children;
            count =3;
            do {
                struct DispatchChild *child = *children++;
                if (child !=0)
                    ResourceObject_Release(child);
            } while (--count >=0);
            break;
        }
        src =&zero;
        /* FAKEMATCH:ordinary release88/15 captures the real DMA-zero address after its word store and constant setup; native captures it before them. Tie only the initialized used pointer at this empty boundary, without reading the uninitialized zero word. */
        asm("" : "+r"(src));
        *src =0;
        Dma_Set(src, object, 0x85000000 | (sizeof(*object) /4), REG_DMA3);
    }
}
