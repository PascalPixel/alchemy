/* 2026-09-30 (Mercury's helper, stopped at the wind-down): 5 differing
   halfwords, 352 of 352 bytes, plain C (the previous draft was 165 at 348).
   The remaining difference was not yet analysed. */
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
    struct DjinnRecoveryList *list;
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
    list = &((struct DjinnRecoveryTable *)buffer)->list;
    for (j = 0; j < list->count; j++)
        list->entries[j].unit_id = table->owner_slots[list->entries[j].unit_id];
    if (SerialRuntime_BeginTransferA(buffer, 320) != -1) {
        SerialRuntime_WaitForTransferA();
        WaitFrames(1);
        WaitFrames(2);
    }
    Runtime_BumpFree(buffer);
}
