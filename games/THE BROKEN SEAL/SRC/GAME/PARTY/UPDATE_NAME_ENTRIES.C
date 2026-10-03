#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "SCENE.H"
#include "TBS_EDITION.H"
#include "IWRAM_CALL.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_WORK.H"
#include "BATTLE_PARTY.H"
s32 SerialRuntime_BeginTransferB(void);
void SerialRuntime_WaitForTransferB(void);
void Party_Apply(s32, u16 *);
void Party_Do(void *);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define NAME_LIMIT 7
#define APPEND_NAME_SPACE 1
#else
#define NAME_LIMIT 4
#define APPEND_NAME_SPACE 0
#endif

void Runtime_BumpFree(void *block);
struct BattleUnit *Owner_GetStateFar(s32 unit_id);
s32 SerialRuntime_BeginTransferA(void *data, s32 size);
void SerialRuntime_WaitForTransferA(void);

void WaitFrames(s32);
struct BattleUnit *Runtime_GetObject(s32);
void *Trade_GetOfferStateFar(s32);

extern char MsgEnemyLabel;

s32 UpdateNameEntries(void)
{
    u16 name_text[24];
    void *buffer;
    struct BattleUnit *name_entry;
    s32 named_count;
    s32 index;
    s32 len;
    s32 i;

    buffer = Runtime_BumpAllocateAlternatePool(340);
    named_count = 0;
    index = 0;
    while (index <= 2) {
        name_entry = Runtime_GetObject(index + 128);
        if (SerialRuntime_BeginTransferB() == -1) {
            break;
        }
        SerialRuntime_WaitForTransferB();
        if (name_entry->status_12a != 0) {
            named_count += 1;
        }
        WaitFrames(2);
        Party_Apply((s32)&MsgEnemyLabel, name_text);
        i = 0;
        if (name_text[i] != 0) {
            do {
                i += 1;
                if (i > NAME_LIMIT) {
                    break;
                }
            } while (name_text[i] != 0);
        }
#if APPEND_NAME_SPACE
        name_text[i] = ' ';
        i += 1;
#endif
        len = i;
        for (i = 14; i >= len; i--) {
            name_entry->name[i] = name_entry->name[i - len];
        }
        for (i = 0; i < len; i++) {
            name_entry->name[i] = (u8)name_text[i];
        }
        name_entry->name[14] = 0;
        index += 1;
    }
    Party_Do(buffer);
    buffer = Runtime_BumpAllocateAlternatePool(320);
    Trade_GetOfferStateFar(1);
    if (SerialRuntime_BeginTransferB() != -1) {
        SerialRuntime_WaitForTransferB();
        WaitFrames(2);
    }
    Party_Do(buffer);
    return named_count;
}

/* Sends this side's battle party over the link cable: one unit state per
 * transfer, padded to three, then the Djinn offer with its owners renamed to
 * the member slots the other side knows them by. It is declared to return a
 * value and returns none: the epilogue keeps r0 free for one. */
s32 LinkBattle_SendParty(void)
{
    u8 *buffer;
    struct BattleSession *work;
    s32 j;
    struct DjinnRecoveryList *list;
    s32 size;
    s32 count;
    s32 i;
    s32 mark;
    u16 owners[8];

    size = 340;
    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
    mark = 0xff;
    work = gBattleWork;
    for (i = 7; i >= 0; i--)
        work->owner_slots[i] = mark;
    count = BattleParty_PrepareActiveOwners(owners);
    for (i = 0; i < count; i++) {
        Iwram_CopyWords(buffer, Owner_GetStateFar(owners[i]), size);
        ((struct BattleUnit *)buffer)->status_12a = 2;
        work->owner_slots[owners[i]] = i - 128;
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
    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
    /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the list pointer's
     * copy is emitted before the count's address as in the reference */
    do {
        Iwram_CopyWords(buffer, Trade_GetOfferStateFar(0), size);
    } while (0);
    list = &((struct DjinnRecoveryTable *)buffer)->list;
    for (j = 0; j < list->count; j++)
        list->entries[j].unit_id = work->owner_slots[list->entries[j].unit_id];
    if (SerialRuntime_BeginTransferA(buffer, 320) != -1) {
        SerialRuntime_WaitForTransferA();
        WaitFrames(1);
        WaitFrames(2);
    }
    Runtime_BumpFree(buffer);
}
