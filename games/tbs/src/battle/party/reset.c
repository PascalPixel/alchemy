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
#include "btltype.h"
#include "scene.h"
#include "btlsmn.h"
#include "btlpty.h"
#include "owner_state.h"
#include "btlsys.h"
#include "mem.h"
#include "tbs_edition.h"
#include "iwram_call.h"
#include "btlwork.h"
#include "serial_runtime.h"
#include "text/msg_ids.h"

void UnitCalcFar(s32 unit_id);

#define Trade_AddOfferFar ((void (*)(s32, s32, s32))Trade_AddOfferFar)

s32 Djinn_ActivateFar(s32 id, s32 element, s32 index);
s32 Trade_RemoveOfferFar(s32 id, s32 element, s32 index);

struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
struct BtlObj			  *BtlObjGet(s32 object_id);

s32 BattleParty_ResetActiveRuntimeFields(void)
{
	u16 owners[10];
	s32 count;
	s32 i;

	count = BattleParty_PrepareActiveOwners(owners);

	i = 0;
	if (i < count)
	{
		struct BtlUnit *unit;
		u8			   *cursor;
		s32				remaining;

		do
		{
			unit = UnitGetFar(owners[i]);
			cursor = (u8 *)&unit->elm_lv[3];
			remaining = 3;

			do
			{
				remaining--;
				*cursor-- = 0;
			} while (remaining >= 0);

			unit->atk_cnt = 0;
			unit->atk_lv = 0;
			unit->def_cnt = 0;
			unit->def_lv = 0;
			unit->res_cnt = 0;
			unit->res_lv = 0;
			unit->maboroshi = 0;
			unit->konran = 0;
			unit->charm = 0;
			unit->shibire = 0;
			unit->nemuri = 0;
			unit->fuuji = 0;
			unit->refrain = 0;
			unit->reflect = 0;
			unit->shiryo = 0;
			unit->unknown_142[0] = 0;
			unit->unknown_142[1] = 0;
			unit->sonae = 0;
			unit->miugoki = 0;
			unit->agi_cnt = 0;
			unit->agi_lv = 0;
			unit->chouhatsu = 0;

			UnitCalcFar(owners[i]);
			i++;
		} while (i < count);
	}
}

s32 BattleParty_ReconcileDjinnRecovery(void)
{
	u16 owners[10];
	s32 count;
	s32 i;
	s32 owner;
	s32 element;
	s32 index;

	count = BattleParty_PrepareActiveOwners(owners);

	for (i = 0; i < count; i++)
	{
		owner = owners[i];
		for (element = 0; element <= 3; element++)
		{
			for (index = 0; index <= 19; index++)
			{
				if (Trade_CanOfferDjinnFar(owner, element, index) != 0)
				{
					struct DjinnRecoveryList *list =
						&Trade_GetOfferStateFar((owner < 0 || owner > 7) ? 1 : 0)->list;
					s32 j;

					for (j = 0; j < list->count; j++)
					{
						if (element == list->entries[j].element && index == list->entries[j].index)
							break;
					}
					if (j == list->count)
						Trade_AddOfferFar(owner, element, index);
				}
			}
		}
	}

	if (GameFlag_TestFar(364) != 0)
		return;

	{
		struct DjinnRecoveryList  *list = &Trade_GetOfferStateFar(0)->list;
		struct DjinnRecoveryEntry *entry;

		i = 0;
		if (i < list->count)
		{
			s32 permanent_timer = -1;

			entry = list->entries;
			do
			{
				if (entry->turns == permanent_timer && BtlObjGet(entry->unit_id) == 0)
				{
					u8 id = entry->unit_id;
					u8 element = entry->element;
					u8 index = entry->index;

					Djinn_ActivateFar(id, element, index);
					Trade_RemoveOfferFar(id, element, index);
				}
				i++;
				entry++;
			} while (i < list->count);
		}
	}
}

struct BattleSummonState *BattleSummon_UpdateAvailability(void)
{
	u8	totals[4];
	u16 party_members[10];
	s32 party_size;
	s32 element;
	u32 available_mask;

	party_size = BattleParty_PrepareActiveOwners(party_members);
	available_mask = 0;

	element = 0;
	do
	{
		totals[element] = 0;
		{
			s32 party_slot;

			for (party_slot = 0; party_slot < party_size; party_slot++)
			{
				struct BtlUnit *member = UnitGetFar(party_members[party_slot]);

				totals[element] += member->djinn_owned_counts[element];
			}
		}
		element++;
	} while (element <= 3);

	element = 0;
	do
	{
		const struct SummonDefinition *summon = SummonDefinition_Get(element);
		s32							   elements_met;

		if (summon != 0)
		{
			const u8 *required = summon->djinn_required;

			elements_met = 0;
			if (totals[0] >= required[0])
			{
				u8 *total = totals;

				do
				{
					elements_met++;
					if (elements_met > 3)
						break;
					total++;
					required++;
				} while (*total >= *required);
			}

			if (elements_met == 4)
				available_mask |= 1u << element;
		}
		element++;
	} while (element <= 31);

	{
		struct BattleSummonState *state = Trade_GetOfferStateFar(0);
		state->available_mask = available_mask;
		return state;
	}
}

void BattleSummon_ReservedNoOp(void)
{
}

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
