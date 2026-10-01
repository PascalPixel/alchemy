/* NONMATCHING (2026-10-01): complete current EN compile/link is 352 bytes
 * against the complete 352-byte native owner, with 144 differing bytes.
 * Finite source scheduling/carrier experiment; no exact credit. The ordinary
 * game compiler and option set were used, including every emitted pool word.
 * The current preserved base remains preferable; no production source changed.
 */
/* 2026-09-30 (Mercury's helper, stopped at the wind-down): 5 differing
   halfwords, 352 of 352 bytes, plain C (the previous draft was 165 at 348).
   2026-10-01 (☀️ matcher 1): the one remaining difference is sched2's, in
   the block after the offer copy: the reference emits the list pointer's
   copy (adds r4, r6, #0) first and its +8 after the count load; here the
   copy follows the load. After reload both drafts have the same insns
   (copy, +8, j = 0, the 264 constant, the count address, its load, the
   exit test); the scheduler's priorities are 6 for the constant chain and
   2 for the copy, so the constant goes first. The reference's order needs
   the copy at priority 6 or a scheduling barrier between the copy and the
   +8, which one C statement assigning list does not give. Tried without
   change: list declared first, a u32 j, a (u8 *) + 8 list, a while loop,
   j = 0 before list, a u8 temporary for the id, do {} while (0) barriers;
   worse: no list pointer (40), a walking entry pointer (60), a table
   pointer for the whole offer (52), an entry temporary (66). Two permuter
   runs (60 s focused, 300 s from seed 100, 169,000 candidates) found
   nothing below 60. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PARTY.H"

void *Runtime_BumpAllocateAlternatePool(s32 size);
void Runtime_BumpFree(void *block);
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
s32 SerialRuntime_BeginTransferA(void *data, s32 size);
void SerialRuntime_WaitForTransferA(void);
struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);

s32 Func_080b5f0c(void)
{
    u8 *buffer;
    struct BattleSession *table;
    s32 j;
    /* FAKEMATCH: pin the measured local carrier in this preserved allocation trial. */
    register struct DjinnRecoveryList *list __asm__("r4");
    s32 size;
    s32 count;
    s32 i;
    s32 mark;
    u16 owners[8];

    size = 340;
    buffer = Runtime_BumpAllocateAlternatePool(size);
    mark = 0xff;
    table = gBattleWork;
    for (i = 7; i >= 0; i--)
        table->owner_slots[i] = mark;
    count = BattleParty_PrepareActiveOwners(owners);
    for (i = 0; i < count; i++) {
        Iwram_CopyWords(buffer, Owner_GetStateFar(owners[i]), size);
        ((struct BattleUnit *)buffer)->status_12a = 2;
        table->owner_slots[owners[i]] = i - 128;
        if (SerialRuntime_BeginTransferA(buffer, 340) == -1)
            break;
        SerialRuntime_WaitForTransferA();
        WaitFrames(2);
    }
    for (; i <= 2; i++) {
        ((struct BattleUnit *)buffer)->status_12a = 0;
        if (SerialRuntime_BeginTransferA(buffer, 340) == -1)
            break;
        SerialRuntime_WaitForTransferA();
        WaitFrames(2);
    }
    size = 320;
    Runtime_BumpFree(buffer);
    buffer = Runtime_BumpAllocateAlternatePool(size);
    Iwram_CopyWords(buffer, Trade_GetOfferStateFar(0), size);
    /* FAKEMATCH: fixed r4 carrier and empty-assembly boundary test list-pointer copy scheduling. */
    list = (struct DjinnRecoveryList *)buffer;
    /* FAKEMATCH: keep the measured list-copy boundary in this preserved scheduling trial. */
    __asm__ volatile ("" : "+l" (list));
    list = (struct DjinnRecoveryList *)((u8 *)list + 8);
    for (j = 0; j < list->count; j++)
        list->entries[j].unit_id = table->owner_slots[list->entries[j].unit_id];
    if (SerialRuntime_BeginTransferA(buffer, 320) != -1) {
        SerialRuntime_WaitForTransferA();
        WaitFrames(1);
        WaitFrames(2);
    }
    Runtime_BumpFree(buffer);
}
