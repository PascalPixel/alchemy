/* CANONICAL DRAFT, 2026-10-02, uncredited.
 * Four-neighbor family before existing TLA DISPATCH.C:36/88/392/40 bytes.
 * Retained whole-module candidate876/native876,68 differences all six;
 * lookup36/0,release88/0,constructor392/62,initialize40/6.
 * Existing320-byte suffix incl all pools/calls remains0diff;120 whole-module
 * actual BL/pool operands equal. Raw resource create336/release72 independently
 * symbolic-linked complete all six; their proposed production names remain
 * absent. These private context links establish physical identities, not credit.
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
 */

/* This listing proposal owns ObjectDispatch_FindFreeObject only; native complete function/pool34 bytes.
 * Its36-byte raw listing also carries a trailing anonymous zero
 * alignment halfword; this standalone draft emits no final halfword.
 * Unmarked internal compiler alignment can count under the current C2 span
 * convention; final linker gaps do not. No counting marker is introduced. */

#include "OBJECT_RUNTIME.H"
#include "RAM_BUFFER.H"

void *ObjectDispatch_FindFreeObject(void)
{
    struct ObjectRuntime *object = Ram_HeapSlots->script_objects;
    void *result =0;
    s32 index =0;

    /* FAKEMATCH:ordinary eight-form cohort leaves this36-byte lookup with4 differing bytes:result-zero setup follows the first script load. Tie the existing used pointer/result values at one empty boundary so result initialization precedes the pointer-dependent script read. */
    asm("" : "+r"(object), "+r"(result));

    while (index <=63) {
        if (object->script ==0) {
            result = object;
            break;
        }
        index++;
        object++;
    }
    return result;
}
