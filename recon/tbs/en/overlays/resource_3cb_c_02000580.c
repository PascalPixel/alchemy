/* NONMATCHING: admitted H7 restored after rejected H8 at 784af7a33.
 * 568/560 bytes, 203 differing halfwords / 84 aligned edits. Bounded
 * copier/phase/tail probes complete; remaining topology, map reload and
 * copy scheduling require new evidence. Native proofs and credits unchanged.
 * REJECTED H8 2026-09-27 shared retry tail: 568/560 bytes, 158 differing
 * halfwords / 106 aligned edits. Explicit first-to-final retry-failure
 * goto emits the desired first ble/poll topology, but jump2 merges all
 * three retry failures, including the ROM's separate second failure.
 * Heap/index exchange r7/r8: admission fails despite the lower raw count.
 * Frame32, word compaction and three independent send-result stores
 * survive; map still reloads. Pools remain +8. Full normalized diff read.
 * Checkpoint this rejected source, then restore admitted H7 at 9cedc5ed6.
 * STOP shared-tail axis without a new causal fact. DONE/alignment +0.
 * NONMATCHING H7 2026-09-27: 568/560 bytes, 203 differing halfwords /
 * 84 aligned edits. First retry counter now begins after Send: the exact
 * movs r5,#0 between the -1 materialization and result compare is restored.
 * All three independent r0 failure stores, frame32, heap r8 and word
 * compaction survive. Full normalized diff read. Remaining: cold retry
 * tails, map reload and copy/call ordering; literal values agree at +8.
 * Retain this phase-lifetime fact; DONE/alignment +0.
 * NONMATCHING H6 2026-09-27: transfer exact battle modules' inline CopyWords
 * boundary. 568/560 bytes, 209 differing halfwords / 87 aligned edits:
 * byte-identical to direct-call H5, including both wrong copy setups and
 * all three independent send-failure stores. Full normalized diff read.
 * STOP copier wrapper axis; direct callback retained, DONE/alignment +0.
 * NONMATCHING H5: 568/560 bytes, 209 differing halfwords / 87 aligned edits.
 * Admitted: all three independent send-failure r0 stores survive loop and
 * jump2 (insns 151/358/593); frame32, heap r8 and word compaction retained.
 * Full normalized diff read; -da output equals ordinary assembly exactly.
 * Remaining: retry failures form three cold blocks (ROM shares first/last
 * near the final poll), map still reloads per iteration, and first copier
 * setup regresses. Pools move +8. Keep this admitted exit-topology witness;
 * H3's lower-score 556/85 model remains at 17cf30899, rejected H4 at 8684ab881.
 * Three models complete; STOP without a new causal fact. DONE +0.
 * Failure-boundary H5: baseline loop pass moves first send failure to cold
 * label762; jump2 then merges all three r0 result stores. loop.c's guarded
 * exit motion is disabled when the guard target exits the inner loop region.
 * A one-pass send/check boundary tests that explicit source ownership;
 * admission is three independent result stores, with frame32 and copy loop.
 * Offer-phase H4 rejected: 560/560 bytes, 204 halfwords / 136 edits.
 * Formal map parameter coalesces back into pseudo33: 133 insns / 14 calls,
 * still stack-preferring, still reloaded in-loop. Heap/index exchange r7/r8;
 * list takes ip instead of map. Frame32 and pool offsets alone now agree,
 * but the map admission fails. Full diff read, -da equals normal assembly.
 * Preserve this trial, then restore H3. No declaration/type follow-up.
 * Offer-phase H4: baseline map pseudo33 spans 134 insns / 14 calls and
 * spills at sp+0; reference reloads it once into ip before the call-free
 * remapping loop. The entry copy is already SImode, not a BLK copy defect.
 * Isolate this complete phase as an inline list/map operation; predict a
 * short map parameter lifetime while retaining frame32 and word compaction.
 * Copy-interface H3 result: 556/560 bytes, 204 differing halfwords /
 * 85 aligned edits. The first call now has the exact routine-load-before-r0
 * order; the second still differs. Full diff read: frame32, record-copy
 * loop and polling gains retained; merged exits/map reload remain. DONE +0.
 * Copy-interface H3: exact PROCESS_PENDING_GRAPHICS_TRANSFER.C and
 * QUEUE_SORT_BY_PRIORITY.C use a value-returning resident word copier.
 * The own callee advances r0 and returns through lr. Transfer that interface;
 * predict routine-load-before-destination setup at both calls, without
 * changing the admitted frame, copy extents or packet polling behavior.
 * NONMATCHING: 556/560 bytes, 205 differing halfwords, 86 aligned edits.
 * Complete owner 02000580..020007b0; return 020007a6, pool 020007a8..7b0.
 * Caller 007b0, exact RECEIVE_PARTY.C and DIGIT_VALUE.C, transfer start/
 * active-query callees, allocator and offer-state interfaces audited.
 * Runtime bindings are registered in link-lobby-send-party-candidate.
 * H1 (2026-09-27): transfer the explicit allocation-size lifetime from
 * RECEIVE_PARTY.C and separate final offer timeout/retries from party
 * polling. Old 548/560 model had 262 differing halfwords / 131 aligned
 * edits; heap already occupied r8, contrary to the stale old header.
 * The new model restores size/owner address r5, party index r7, and final
 * timeout/retries r7/sl. Both full normalized diffs reviewed.
 * H2: the exact receiver waits before decrementing its local budget in C.
 * Transfer that polling interface to all three phases: 556/205/86, with
 * every wait-argument/decrement pair now in reference machine order.
 * H1 is preserved at 3daaadc4e. H2 retains its register-lifetime gains;
 * raw halfword count rises because the surrounding blocks remain shifted.
 * Remaining: three send-failure stores merge, retry-failure exits merge,
 * offer-map pointer reloads, and copy-call scheduling. Same 32-byte frame
 * and two literal values as ROM. STOP this bounded family transfer; no
 * further control/spelling trials without new evidence. RECEIVE_PARTY.C
 * rechecked 420/420 exact. No shared header or exact sibling edited;
 * no DONE credit. */
#include "TYPES.H"

u8 *Main_08000170(u32 size);
void Main_08000178(u8 *heap);
s32 SceneData_CopyUpToThreeEntries(u16 *owners);
u8 *Engine_OwnerGetState(s32 owner);
s32 Main_08000380(u8 *data, u32 size);
void Engine_TaskWait(s32 frames);
s32 Main_080003a8(void);
u8 *Main_08077000(s32 mode);

typedef s32 (*WordCopyFn)(void *destination, const void *source, s32 size);

#define LINK_STAT (*(volatile u16 *)0x03001f64)

struct LinkEntry {
    u8 a;
    u8 b;
    u8 owner;
    u8 c;
};

struct LinkList {
    struct LinkEntry entries[64];
    s32 count;
};

s32 LinkLobby_SendPartyRecords(void)
{
    s32 result;
    u8 *table;
    u16 owners[8];
    u8 slots[8];
    s32 ret;
    u8 *heap;
    s32 count;
    s32 timeout;
    s32 tries;
    s32 i;
    s32 j;
    s32 k;
    u32 size;
    struct LinkList *list;

    size = 0x154;
    heap = Main_08000170(size);
    result = 0;
    count = SceneData_CopyUpToThreeEntries(owners);
    table = slots;
    timeout = 600;
    for (i = 7; i >= 0; i--) {
        table[i] = 0;
    }
    for (i = 0; i < count; i++) {
        ((WordCopyFn)0x03001388)(heap, Engine_OwnerGetState(owners[i]), 0x154);
        heap[0x12a] = 2;
        table[owners[i]] = i - 128;
        /* FAKEMATCH: one-pass send/check boundary retains its failure exit. */
        do {
            ret = Main_08000380(heap, 0x154);
            tries = 0;
            if (ret == -1) {
                result = ret;
                goto done;
            }
        } while (0);
        while (Main_080003a8() != 0) {
            Engine_TaskWait(1);
            if (--timeout < 0 || (LINK_STAT & 3) != 3) {
                if (++tries > 24) {
                    result = -1;
                    goto done;
                }
            }
        }
        Engine_TaskWait(2);
    }
    goto next;
wait:
    Engine_TaskWait(1);
    if (--timeout < 0 || (LINK_STAT & 3) != 3) {
        if (++tries > 24) {
            result = -1;
            goto done;
        }
    }
test:
    if (Main_080003a8() != 0) {
        goto wait;
    }
    Engine_TaskWait(2);
    i++;
next:
    if (i <= 2) {
        heap[0x12a] = 0;
        tries = 0;
        /* FAKEMATCH: keep this send/check boundary distinct from polling. */
        do {
            if ((ret = Main_08000380(heap, 0x154)) != -1) {
                goto test;
            }
            result = ret;
            goto done;
        } while (0);
    }
    Main_08000178(heap);
    size = 0x140;
    heap = Main_08000170(size);
    ((WordCopyFn)0x03001388)(heap, Main_08077000(0), size);
    {
        s32 timeout;
        s32 tries;

        list = (struct LinkList *)(heap + 8);
        tries = 0;
        timeout = 600;
        for (j = 0; j < list->count; j++) {
            list->entries[j].owner = table[list->entries[j].owner];
            if ((s8)list->entries[j].owner == 0) {
                for (k = j; k < list->count - 1; k++) {
                    list->entries[k] = list->entries[k + 1];
                }
                list->count--;
                j--;
            }
        }
        /* FAKEMATCH: one-pass send/check boundary retains its failure exit. */
        do {
            if ((ret = Main_08000380(heap, 0x140)) == -1) {
                result = ret;
                goto done;
            }
        } while (0);
        while (Main_080003a8() != 0) {
            Engine_TaskWait(1);
            if (--timeout < 0 || (LINK_STAT & 3) != 3) {
                if (++tries > 24) {
                    result = -1;
                    goto done;
                }
            }
        }
        Engine_TaskWait(1);
        Engine_TaskWait(2);
    }
done:
    Main_08000178(heap);
    return result;
}
