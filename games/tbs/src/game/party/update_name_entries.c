#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "mem.h"
#include "scene.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "btltype.h"
#include "btlwork.h"
#include "btlpty.h"
#include "serial_runtime.h"
#include "btlsys.h"
#include "text/msg_ids.h"
void Ui_AdjustValueWithoutLimitFar(s32, u16 *);
struct BtlUnit *UnitGetFar(s32 object_id);

#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || \
    defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
#define NAME_LIMIT 7
#define APPEND_NAME_SPACE 1
#else
#define NAME_LIMIT 4
#define APPEND_NAME_SPACE 0
#endif

void WaitFrames(s32);

enum LinkPartyPacketSize {
    LINK_PARTY_UNIT_BYTES = 340,
    LINK_PARTY_OFFER_BYTES = 320
};

s32 UpdateNameEntries(void)
{
    u16 name_text[24];
    void *buffer;
    struct BtlUnit *name_entry;
    s32 named_count;
    s32 index;
    s32 len;
    s32 i;

    buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_UNIT_BYTES);
    named_count = 0;
    index = 0;
    while (index <= 2) {
        name_entry = UnitGetFar(index + 128);
        if (SerialRuntime_BeginTransferB(name_entry) == -1) {
            break;
        }
        SerialRuntime_WaitForTransferB();
        if (name_entry->side != 0) {
            named_count += 1;
        }
        WaitFrames(2);
        Ui_AdjustValueWithoutLimitFar((s32)MsgEnemyLabel, name_text);
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
            name_entry->name[i] = name_text[i];
        }
        name_entry->name[14] = 0;
        index += 1;
    }
    MemFree(buffer);
    buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_OFFER_BYTES);
    if (SerialRuntime_BeginTransferB(Trade_GetOfferStateFar(1)) != -1) {
        SerialRuntime_WaitForTransferB();
        WaitFrames(2);
    }
    MemFree(buffer);
    return named_count;
}

s32 LinkBattle_SendParty(void)
{
    u8 *buffer;
    struct BtlWork *work;
    s32 j;
    struct DjinnRecoveryList *list;
    s32 size;
    s32 count;
    s32 i;
    s32 mark;
    u16 owners[8];

    size = LINK_PARTY_UNIT_BYTES;
    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
    mark = 0xff;
    work = BtlWk;
    for (i = 7; i >= 0; i--)
        work->owner_slots[i] = mark;
    count = BattleParty_PrepareActiveOwners(owners);
    for (i = 0; i < count; i++) {
        Iwram_CopyWords(buffer, UnitGetFar(owners[i]), size);
        ((struct BtlUnit *)buffer)->side = 2;
        work->owner_slots[owners[i]] = i - 128;
        if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_UNIT_BYTES) == -1)
            break;
        SerialRuntime_WaitForTransferA();
        WaitFrames(2);
    }
    for (; i <= 2; i++) {
        ((struct BtlUnit *)buffer)->side = 0;
        if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_UNIT_BYTES) == -1)
            break;
        SerialRuntime_WaitForTransferA();
        WaitFrames(2);
    }
    size = LINK_PARTY_OFFER_BYTES;
    MemFree(buffer);
    buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
    /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the list pointer's copy is emitted before the count's address as in the reference */
    do {
        Iwram_CopyWords(buffer, Trade_GetOfferStateFar(0), size);
    } while (0);
    list = &((struct DjinnRecoveryTable *)buffer)->list;
    for (j = 0; j < list->count; j++)
        list->entries[j].unit_id = work->owner_slots[list->entries[j].unit_id];
    if (SerialRuntime_BeginTransferA(buffer, LINK_PARTY_OFFER_BYTES) != -1) {
        SerialRuntime_WaitForTransferA();
        WaitFrames(1);
        WaitFrames(2);
    }
    MemFree(buffer);
}
