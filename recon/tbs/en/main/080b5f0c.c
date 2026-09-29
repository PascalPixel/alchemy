/* 2026-09-29 alchemy permute: score 1545 to 895 on the permuter's scorer
   (0 is exact); remaining 22 register-only, 2 operand, 4 reordered, 1
   inserted, 4 deleted. Kept rewrites: 1x change loop form. */
/* Whole owner [080b5f0c, 080b606c), 352 bytes including two pool words.
   2026-09-26 H3 (bounded stop): typed owner-map/Djinn records and a separate
   final-loop counter. Candidate 344/352, 166 differing halfwords, 59 aligned
   edits. Owner-map indexed accesses, copy-call argument order and -1 tests
   improve. The typed Djinn view proves count disjoint from entry writes, so
   GCC turns the last loop into a countdown and removes the ROM's count
   reload. A future model must retain that aliasing, not sweep statement
   order. Remaining also includes initial size residency and r4 reload
   scratch use. All three hypotheses are preserved; no new DONE bytes.
   2026-09-26 H2: reuse the exact modules' value-returning IWRAM copier
   behind a void inline wrapper. Candidate 344/352, 164 differing halfwords,
   73 aligned edits. The unwanted 340-byte size live range disappears;
   frame 16, r9 table, sl owner array and fp status pointer now agree. The
   remaining raw byte indexing forms base+index instead of the reference's
   indexed byte access, and the last loop still shares the sent counter.
   2026-09-26 H1: restore gBattleWork, actual indirect copy calls, transfer
   lengths and continuation of the sent index. Candidate 348/352, 122
   differing halfwords, 75 aligned edits. Control flow is credible but size
   340 is shared in sl across copy/send, spilling the unit status pointer:
   frame 20/16. Battle table is fp rather than r9; final loop incorrectly
   inherits the first loop's i lifetime. Copy-call return convention needs
   checking against the actual ARM callee before any ordering experiment.
   The eight stack halfwords hold active owner IDs. The first three serial
   packets are 340-byte unit records; the last is a 320-byte Djinn table.
   The ROM continues the sent count through the empty-packet loop. There is
   no direct caller named by the maintained listings or source registry.
   Untouched template baseline: 320/352, 173 differing halfwords, 101 edits. */
#include "TYPES.H"
#include "SYSTEM.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PARTY.H"

struct BattleLinkWork {
    u8 unknown_00[72];
    u8 owner_map[8];
};

typedef s32 (*CopyWordsFn)(void *, const void *, s32);

static __inline__ void CopyWords(void *dst, const void *src, s32 size)
{
    ((CopyWordsFn)0x03001388)(dst, src, size);
}

s16 *Runtime_BumpAllocateAlternatePool(s32);
struct BattleUnit *Owner_GetStateFar(s32);
s32 SerialRuntime_BeginTransferA(s32, s32);
void SerialRuntime_WaitForTransferA(void);
void Runtime_BumpFree(void *);
struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32);

s32 Func_080b5f0c(void)
{
    struct BattleLinkWork *table;
    u8 *buffer;
    u16 sp_names[8];
    s32 count;
    s32 i;
    s32 result;

    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(340);
    table = gBattleWork;
    for (i = 7; i >= 0; i--) {
        table->owner_map[i] = 0xff;
    }
    count = BattleParty_PrepareActiveOwners(sp_names);
    for (i = 0; i < count; i++) {
        struct BattleUnit *object = Owner_GetStateFar(sp_names[i]);
        CopyWords(buffer, object, 340);
        ((struct BattleUnit *)buffer)->status_12a = 2;
        table->owner_map[sp_names[i]] = (u8)(i - 128);
        result = SerialRuntime_BeginTransferA((s32)buffer, 340);
        if (result == -1) {
            break;
        }
        SerialRuntime_WaitForTransferA();
        WaitFrames(2);
    }
    while (i <= 2) {
        ((struct BattleUnit *)buffer)->status_12a = 0;
        result = SerialRuntime_BeginTransferA((s32)buffer, 340);
        if (result == -1) {
            break;
        }
        SerialRuntime_WaitForTransferA();
        WaitFrames(2);
        i++;
    }
    Runtime_BumpFree(buffer);
    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(320);
    {
        struct DjinnRecoveryTable *unit = Trade_GetOfferStateFar(0);
        CopyWords(buffer, unit, 320);
    }
    {
        struct DjinnRecoveryList *list = &((struct DjinnRecoveryTable *)buffer)->list;
        struct DjinnRecoveryEntry *entry = list->entries;
        s32 i;
        i = 0;
        if (i < list->count) {
            do {
                entry->unit_id = table->owner_map[entry->unit_id];
                entry++;
                i++;
            } while (i < list->count);
        }
    }
    result = SerialRuntime_BeginTransferA((s32)buffer, 320);
    if (result != -1) {
        SerialRuntime_WaitForTransferA();
        WaitFrames(1);
        WaitFrames(2);
    }
    Runtime_BumpFree(buffer);
    /* FAKEMATCH: preserve the reference's value-returning epilogue although
       the last callee and the observable operation return no value. */
}
