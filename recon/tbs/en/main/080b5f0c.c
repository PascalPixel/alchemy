/* Whole owner [080b5f0c, 080b606c), 352 bytes including two pool words.
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

typedef void (*CopyWordsFn)(void *, const void *, s32);

s16 *Runtime_BumpAllocateAlternatePool(s32);
struct BattleUnit *Owner_GetStateFar(s32);
s32 SerialRuntime_BeginTransferA(s32, s32);
void SerialRuntime_WaitForTransferA(void);
void Runtime_BumpFree(void *);
struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32);

s32 Func_080b5f0c(void)
{
    u8 *table;
    u8 *buffer;
    u16 sp_names[8];
    s32 count;
    s32 i;
    s32 result;

    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(340);
    table = gBattleWork;

    for (i = 7; i >= 0; i--) {
        table[72 + i] = 0xff;
    }

    count = BattleParty_ListActiveMembers(sp_names);
    for (i = 0; i < count; i++) {
            struct BattleUnit *object = Owner_GetStateFar(sp_names[i]);
            ((CopyWordsFn)0x03001388)(buffer, object, 340);
            ((struct BattleUnit *)buffer)->status_12a = 2;

            table[sp_names[i] + 72] = (u8)(i - 128);

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
        ((CopyWordsFn)0x03001388)(buffer, unit, 320);
    }

    {
        s32 count2 = *(s32 *)(buffer + 264);
        u8 *entry = buffer + 8;
        for (i = 0; i < count2; i++) {
            entry[2] = table[entry[2] + 72];
            count2 = *(s32 *)(buffer + 264);
            entry += 4;
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
