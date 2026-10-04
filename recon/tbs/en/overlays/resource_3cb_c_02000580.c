/* NONMATCHING (2026-10-04, wave 3): alchemy diff 34 (was 105). Gains: the
 * first poll loop's retry failure jumps into the offer phase's failure
 * block (goto fail), which is where the ROM's cross-jumped copy sits; the
 * offer phase polls through labels like the pad phase, so loop.c leaves its
 * failure in place; the pad and offer sends lost their do/while(0) devices;
 * heap[0x12a] = tries = 0 shares the zero as the ROM does; the sends pass
 * size; the offer remap reads slots[] directly (the ROM hoists it into ip);
 * timeout = 600 is set before the party count. Only the first send keeps
 * its one-pass device. Remaining: reload registers are one step behind
 * the ROM's round robin from the first loop's "add sl, 2" on (ROM r1, r2,
 * r3, r1; here r3, r1, r2, r3), so the pad failure (r1) cross-jumps into
 * nothing and the pad send compares in r2. The ROM probably reloaded one
 * more constant in the first loop: its retry failure there was most likely
 * inline (reload r3) and only later cross-jumped into the offer block.
 * A labelled inline block (fail1:) is still moved by loop.c (71). Local
 * declaration order and the i - 128 spellings change nothing. Two small
 * scheduling differences remain in the first loop (i - 128 before the
 * table reload, r0 = heap before the size constant).
 * Earlier notes: */
/* NONMATCHING (2026-10-04): score 1815 (17 register-only, 13 reordered,
 * 6 inserted). The callees now carry the overlay's own veneer names and the
 * copy goes through IWRAM_CALL.H's Iwram_CopyWords, so no name differences
 * remain. Everything up to the first send's retry failure matches. Remaining:
 * the ROM loads that failure's -1 into r3 in a block it shares with the offer
 * send's failure (cross-jumped, after the third poll loop), while here the
 * -1 is r2 (the send check's constant) and the block stays separate; the pad
 * send's own failure uses r2 in the ROM and r1 here; the offer phase copies
 * its pointers in another order. The plain-loop draft that stood beside this
 * one (score 3360 under the old names) noted: slot in r7 and retries in r5,
 * the 600-frame timeout set during the clear loop, the offer items walked
 * with a pointer beside the index and the count read through the item base,
 * and the renumbered owner tested with lsls #24. */
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
#include "IWRAM_CALL.H"

u8 *Runtime_BumpAllocateAlternatePool(u32 size);
void Runtime_BumpFree(u8 *heap);
s32 SceneData_CopyUpToThreeEntries(u16 *owners);
u8 *Owner_GetState(s32 owner);
s32 SerialRuntime_BeginTransferA(u8 *data, u32 size);
void Engine_TaskWait(s32 frames);
s32 SerialRuntime_GetActiveTransfers(void);
u8 *Trade_GetOfferState(s32 mode);

typedef s32 (*WordCopyFn)(void *destination, const void *source, s32 size);

extern u8 gLinkStatus[];
#define LINK_STAT (*(volatile u16 *)gLinkStatus)

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
    heap = Runtime_BumpAllocateAlternatePool(size);
    result = 0;
    timeout = 600;
    count = SceneData_CopyUpToThreeEntries(owners);
    table = slots;
    for (i = 7; i >= 0; i--) {
        table[i] = 0;
    }
    for (i = 0; i < count; i++) {
        Iwram_CopyWords(heap, Owner_GetState(owners[i]), size);
        heap[0x12a] = 2;
        table[owners[i]] = i - 128;
        /* FAKEMATCH: one-pass send/check boundary retains its failure exit. */
        do {
            ret = SerialRuntime_BeginTransferA(heap, size);
            tries = 0;
            if (ret == -1) {
                result = ret;
                goto done;
            }
        } while (0);
        while (SerialRuntime_GetActiveTransfers() != 0) {
            Engine_TaskWait(1);
            if (--timeout < 0 || (LINK_STAT & 3) != 3) {
                if (++tries > 24) {
                    goto fail;
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
    if (SerialRuntime_GetActiveTransfers() != 0) {
        goto wait;
    }
    Engine_TaskWait(2);
    i++;
next:
    if (i <= 2) {
        heap[0x12a] = tries = 0;
        if ((ret = SerialRuntime_BeginTransferA(heap, size)) != -1) {
            goto test;
        }
        result = ret;
        goto done;
    }
    Runtime_BumpFree(heap);
    size = 0x140;
    heap = Runtime_BumpAllocateAlternatePool(size);
    Iwram_CopyWords(heap, Trade_GetOfferState(0), size);
    {
        s32 timeout;
        s32 tries;

        list = (struct LinkList *)(heap + 8);
        tries = 0;
        timeout = 600;
        for (j = 0; j < list->count; j++) {
            list->entries[j].owner = slots[list->entries[j].owner];
            if ((s8)list->entries[j].owner == 0) {
                for (k = j; k < list->count - 1; k++) {
                    list->entries[k] = list->entries[k + 1];
                }
                list->count--;
                j--;
            }
        }
        if ((ret = SerialRuntime_BeginTransferA(heap, 0x140)) == -1) {
            result = ret;
            goto done;
        }
        goto test3;
    wait3:
        Engine_TaskWait(1);
        if (--timeout < 0 || (LINK_STAT & 3) != 3) {
            if (++tries > 24) {
            fail:
                result = -1;
                goto done;
            }
        }
    test3:
        if (SerialRuntime_GetActiveTransfers() != 0) {
            goto wait3;
        }
        Engine_TaskWait(1);
        Engine_TaskWait(2);
    }
done:
    Runtime_BumpFree(heap);
    return result;
}
