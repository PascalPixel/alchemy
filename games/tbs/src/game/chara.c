#include "unit/game/chara.h"
#include "unit/game/item.h"
#include "unit/game/flag.h"
#include "unit/game/party.h"
#include "unit/game/class.h"
#include "unit/game/btlcalc.h"
#include "unit/game/djinn.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/heap.h"
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
#include "owner_state.h"
#include "character.h"
#include "party_state.h"
#include "runtime_interfaces.h"
#include "item.h"
#include "game_flags.h"
#include "inventory.h"
#include "resource.h"
#include "resource_ids.h"
#include "dma.h"
#include "game_state.h"
#include "frame.h"
#include "save_state.h"
#include "io_reg.h"
#include "fixed_math.h"
#include "battle_unit.h"
#include "lib.h"
#include "edition.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "btlsmn.h"
#include "preset_table.h"
#include "iwram_call.h"
#include "btlcalc.h"
#include "battle_random.h"
#include "btltype.h"
#include "battle_effect_chance.h"
#include "btlpty.h"
#include "field_event.h"

struct StatWork
{
	s32					   hp;
	s32					   ep;
	s32					   atk;
	s32					   def;
	s32					   agi;
	s32					   unknown_14;
	s32					   luck;
	s32					   turns;
	s32					   hp_regen;
	s32					   ep_regen;
	s32					   element[4][2];
	s32					   kind;
	s32					   unknown_4c[2];
	s32					   amount;
	struct ItemDefinition *item;
	s32					   unknown_5c;
};

#define STAT_DIFF(a, b) ((a) - (b) < 0 ? (b) - (a) : (a) - (b))

void Owner_RefreshAndResetZero(void)
{
	GameState_InitDefaults();
	Game_ResetForNewGameFar(0);
}

void *Trade_GetOfferState(s32 arg0)
{
	if (arg0 != 0)
	{
		return UnitGet(0x83);
	}
	return &gGameState.unknown_008[4];
}

u32 Party_GetAverageLevel(void)
{
	s32 count;
	s32 total;
	s32 i;

	total = 0;
	count = Party_CountActiveOwners();
	if (count == 0)
	{
		return 0;
	}
	for (i = 0; i < count; i++)
	{
		total += UnitGet(gGameState.active_owners[i])->level;
	}
	total = total / count;
	return total;
}

struct BtlUnit *UnitGet(u32 owner)
{
	struct BtlUnit *states;
	struct BtlUnit *unit;

	{
		void *caller = __builtin_return_address(0);

		/* Native copies the unused return
         * address from lr into r3 at entry. This existing empty asm use
         * requires a Thumb low register, so allocation chooses r3 without
         * a pin (28/28 instructions, differ 0). With an unrestricted "r"
         * operand or no use, only that copy is absent (28/27, differ 1,
         * same 0x44-byte English extent with a final alignment nop). */
		/* FAKEMATCH: needed to match: low-register use preserves mov r3,lr; without it, 28/27 instructions, differ 1. 2026-10-11: arm_return_addr emits the lr copy after the argument copies and flow deletes it without a codeless use; every plain use (compare, store, fold) either emits code or folds away (differ 7). */
		asm("" : : "l"(caller));
	}
	states = gGameState.owners;
	if (owner < 8)
	{
		unit = &states[owner];
		return unit;
	}
	if (owner - 0x80 < 6 && gBattleOwnerStates != NULL)
	{
		unit = &((struct BtlUnit *)gBattleOwnerStates)[owner - 0x80];
		return unit;
	}
	return NULL;
}

const u8 *Owner_GetRecord(s32 selector)
{
	u32 record_index;

	record_index = selector - 8;
	if (record_index > 0xF9U)
	{
		record_index = 0;
	}
	return (const u8 *)&Enemy_Definitions[record_index];
}

void Runtime_CopyBytesDirectional(u8 *first, u8 *second, s32 count, s32 direction)
{
	s32 index;

	if (direction != 0)
	{
		index = 0;
		if (index < count)
		{
			do
			{
				*second++ = *first++;
				index++;
			} while (index < count);
		}
	}
	else if (count > 0)
	{
		index = count;
		do
		{
			*first++ = *second++;
			index--;
		} while (index != 0);
	}
}

void UnitCalc(s32 owner)
{
	struct StatWork *work;
	struct BtlUnit	*st;
	s32				 i, j, el;
	s32				 value;
	s32				 flag;
	s32				 kind;
	s32				 amount;
	s32				 cap;
	s16				 old;

	work = Runtime_BumpAllocateAlternatePool(sizeof(struct StatWork));
	st = UnitGet(owner);
	work->hp = st->base_hp;
	work->ep = st->base_ep;
	work->atk = st->base_attack;
	work->def = st->base_defense;
	work->agi = st->base_agility;
	work->luck = st->base_luck;
	work->turns = st->base_action_count & 15;
	work->hp_regen = st->base_hp_regen;
	work->ep_regen = st->base_ep_regen;
	{
		s16 *src = &st->base_elm[0].pow;
		s32 *dst = work->element[0];
		for (i = 0; i < 4; i++)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			src += 2;
			dst += 2;
		}
	}

	if (STAT_DIFF(st->max_hp * st->hp_gauge / 0x4000, st->hp) > 1
		|| STAT_DIFF(st->max_ep * st->ep_gauge / 0x4000, st->ep) > 1)
	{
		st->hp_gauge = 0x4000;
		st->ep_gauge = 0x4000;
		st->hp = st->max_hp;
		st->ep = st->max_ep;
	}

	st->restraint &= ~3;
	if (st->restraint & 4)
		st->restraint |= 1;
	if (st->sonae)
		work->turns++;
	st->unknown_142[0] = 0;
	st->unknown_142[1] = 0;

	if (st->class_index)
	{
		for (i = 0; i < 15; i++)
		{
			if (!(st->inventory[i] & INVENTORY_EQUIPPED))
				continue;
			work->item = Item_GetDirect(st->inventory[i]);
			if (work->item->flags & ITEM_CURSED)
				st->restraint |= 3;
			/* FAKEMATCH: a do-while barrier keeps the defense load after the item bonus load, as the ROM schedules it */
			do
			{
				work->atk += work->item->primary_bonus;
			} while (0);
			work->def += work->item->secondary_bonus;
			for (j = 0; j < 4; j++)
			{
				kind = work->item->effects[j].kind;
				amount = work->item->effects[j].amount;
				work->kind = kind;
				work->amount = amount;
				switch (work->kind)
				{
				case ITEM_EFFECT_NONE:
					break;
				case ITEM_EFFECT_ADD_HP:
					work->hp += work->amount;
					break;
				case ITEM_EFFECT_ADD_HP_REGEN:
					work->hp_regen += work->amount;
					break;
				case ITEM_EFFECT_ADD_PP:
					work->ep += work->amount;
					break;
				case ITEM_EFFECT_ADD_PP_REGEN:
					work->ep_regen += work->amount;
					break;
				case ITEM_EFFECT_ADD_AGILITY:
					work->agi += work->amount;
					break;
				case ITEM_EFFECT_ADD_LUCK:
					work->luck += work->amount;
					break;
				case 15:
					work->element[0][0] += work->amount;
					break;
				case 16:
					work->element[1][0] += work->amount;
					break;
				case 17:
					work->element[2][0] += work->amount;
					break;
				case 18:
					work->element[3][0] += work->amount;
					break;
				case 19:
					work->element[0][1] += work->amount;
					break;
				case 20:
					work->element[1][1] += work->amount;
					break;
				case 21:
					work->element[2][1] += work->amount;
					break;
				case 22:
					work->element[3][1] += work->amount;
					break;
				case 23:
					st->unknown_142[0] += work->amount;
					break;
				case 24:
					st->unknown_142[1] += work->amount;
					break;
				case 25:
					st->restraint |= 8;
					break;
				case 26:
					work->turns += work->amount;
					break;
				}
			}
		}
		if (st->restraint & 8)
			st->restraint &= ~9;

		for (el = 0; el < 4; el++)
		{
			u32 bits = st->djinn_active[el];

			for (i = 0; i < 20; i++)
			{
				if (bits & (1 << i))
				{
					struct DjinnDefinition *djinn = Djinn_GetDefinition(el, i);

					work->hp += djinn->hp;
					work->ep += djinn->ep;
					work->atk += djinn->atk;
					work->def += djinn->def;
					work->agi += djinn->agi;
					work->luck += djinn->luck;
				}
			}
		}

		{
			struct ClassDefinition *class = Owner_GetRecordStride84(st->class_index);

			work->hp = work->hp * class->hp / 10;
			work->ep = work->ep * class->ep / 10;
			work->atk = work->atk * class->atk / 10;
			work->def = work->def * class->def / 10;
			work->agi = work->agi * class->agi / 10;
			work->luck = work->luck * class->luck / 10;
		}

		for (i = 0; i < 15; i++)
		{
			if (!(st->inventory[i] & INVENTORY_EQUIPPED))
				continue;
			work->item = Item_GetDirect(st->inventory[i]);
			for (j = 0; j < 4; j++)
			{
				kind = work->item->effects[j].kind;
				amount = work->item->effects[j].amount;
				work->kind = kind;
				kind -= ITEM_EFFECT_SCALE_HP;
				work->amount = amount;
				switch (kind)
				{
				case 0:
					work->hp = work->hp * work->amount / 10;
					break;
				case 1:
					work->hp_regen = work->hp_regen * work->amount / 10;
					break;
				case 2:
					work->ep = work->ep * work->amount / 10;
					break;
				case 3:
					work->ep_regen = work->ep_regen * work->amount / 10;
					break;
				case 4:
					work->atk = work->atk * work->amount / 10;
					break;
				case 5:
					work->def = work->def * work->amount / 10;
					break;
				case 6:
					work->agi = work->agi * work->amount / 10;
					break;
				case 7:
					work->luck = work->luck * work->amount / 10;
					break;
				}
			}
		}
	}

	work->atk = work->atk * (st->atk_lv + 8) / 8;
	work->def = work->def * (st->def_lv + 8) / 8;
	work->agi = work->agi * (st->agi_lv + 8) / 8;
	for (i = 0; i < 4; i++)
		work->element[i][0] += (st->elm_lv[i] * st->elm_lv[i] + st->elm_lv[i]) * 5;
	for (i = 0; i < 4; i++)
		work->element[i][1] += st->res_lv * 20;

	if (st->class_index)
	{
		flag = 0;
		switch (st->class_id)
		{
		case 0:
			flag = GameFlag_Test(0x110);
			break;
		case 1:
			flag = GameFlag_Test(0x112);
			break;
		case 2:
			flag = GameFlag_Test(0x113);
			break;
		case 3:
			flag = GameFlag_Test(0x111);
			break;
		case 5:
			flag = GameFlag_Test(0x112);
			break;
		}
		if (flag)
			work->ep_regen += 4;
	}

	if (work->atk < 0)
		work->atk = 0;
	if (work->atk > 999)
		work->atk = 999;
	if (work->def < 0)
		work->def = 0;
	if (work->def > 999)
		work->def = 999;
	if (work->agi < 0)
		work->agi = 0;
	if (work->agi > 999)
		work->agi = 999;
	if (work->luck < 0)
		work->luck = 0;
	if (work->luck > 99)
		work->luck = 99;
	if (work->turns < 0)
		work->turns = 0;
	if (work->turns > 2)
		work->turns = 2;
	if (work->hp_regen < 0)
		work->hp_regen = 0;
	if (work->hp_regen > 10000)
		work->hp_regen = 10000;
	if (work->ep_regen < 0)
		work->ep_regen = 0;
	if (work->ep_regen > 200)
		work->ep_regen = 200;
	for (i = 0; i < 4; i++)
	{
		if (work->element[i][0] < 0)
			work->element[i][0] = 0;
		if (work->element[i][0] > 200)
			work->element[i][0] = 200;
		if (work->element[i][1] < 0)
			work->element[i][1] = 0;
		if (work->element[i][1] > 200)
			work->element[i][1] = 200;
	}

	st->atk = work->atk;
	st->def = work->def;
	st->agi = work->agi;
	st->luck = work->luck;
	st->action_entry_count = work->turns;
	st->hp_regen = work->hp_regen;
	st->ep_regen = work->ep_regen;
	for (i = 0; i < 4; i++)
	{
		st->elm[i].pow = work->element[i][0];
		st->elm[i].res = work->element[i][1];
	}

	cap = 9999;
	if (st->class_index)
		cap = 1999;

	old = st->max_hp;
	if (work->hp < 0)
		work->hp = 0;
	if (work->hp > cap)
		work->hp = cap;
	st->max_hp = work->hp;
	if (old != st->max_hp)
	{
		value = work->hp * st->hp_gauge / 0x4000;
		if (value < 0)
			value = 0;
		if (value > cap)
			value = cap;
		if (st->hp != 0 && value == 0)
			value = 1;
		st->hp = value;
	}

	old = st->max_ep;
	if (work->ep < 0)
		work->ep = 0;
	if (work->ep > cap)
		work->ep = cap;
	st->max_ep = work->ep;
	if (old != st->max_ep)
	{
		value = work->ep * st->ep_gauge / 0x4000;
		if (value < 0)
			value = 0;
		if (value > cap)
			value = cap;
		if (st->ep != 0 && value == 0)
			value = 1;
		st->ep = value;
	}

	MemFree(work);
}

void GameFlag_RefreshLureCap(void)
{
	s32 count;
	s32 n;

	GameFlag_ClearBit(0x167);
	count = Party_CountActiveOwners();
	for (n = 0; n < count; n++)
	{
		struct BtlUnit *owner;
		s32				i;

		owner = UnitGet(gGameState.active_owners[n]);
		for (i = 0; i < 15; i++)
		{
			if (owner->inventory[i] & INVENTORY_EQUIPPED)
			{
				struct ItemEffect *effect;
				s32				   j;

				effect = Item_GetDirect(owner->inventory[i])->effects;
				for (j = 0; j < 4; j++)
				{
					u8 kind;

					kind = effect->kind;
					effect++;
					if (kind == 27)
					{
						GameFlag_SetBit(0x167);
					}
				}
			}
		}
	}
}

u32 Runtime_GetBuildStampTime(void)
{
	u8 *digits;
	s32 hourTens;
	s32 hourUnits;
	s32 minuteTens;
	s32 minuteUnits;
	s32 secondTens;
	s32 secondUnits;
	s32 hours;
	s32 minutes;
	s32 seconds;
	s32 packed;
	s32 shifted;
	s32 result;

	digits = Resource_GetTableEntry((s32)&ResourceId_BuildStamp);
	hourTens = *digits;
	hours = (hourTens - '0') * 10;
	digits++;
	hourUnits = *digits;
	digits++;
	hours += hourUnits - '0';
	minuteTens = *digits;
	minutes = (minuteTens - '0') * 10;
	digits++;
	minuteUnits = *digits;
	digits++;
	minutes += minuteUnits - '0';
	secondTens = digits[0];
	seconds = (secondTens - '0') * 10;
	secondUnits = digits[1];
	seconds += secondUnits - '0';
	packed = (((hours << 4) + minutes) << 6) + seconds;
	shifted = 0x80 << 21;
	shifted |= packed << 16;
	result = shifted >> 16;
	if (gDebugMode != 0)
	{
		result |= 0xffff8000;
	}
	return (u16)result;
}

struct SceneStateTail
{
	u8	unknown_000[0x100];
	u32 play_time;
	u8	unknown_104;
};

void GameState_InitDefaults(void)
{
	volatile s32 zero;

	zero = 0;
	START_DMA(&zero, &gGameState, 0x850000b0);
	zero = 0;
	START_DMA(&zero, gSceneState, 0x850003e1);
	WAIT_DMA();
	zero = 0;
	START_DMA(&zero, GameFlagBytes, 0x85000080);
	((struct SceneStateTail *)gSceneState)->unknown_104 = 0xff;
	zero = 0;
	START_DMA(&zero, gSaveStamp + 0x1c, 0x85000298);
	Owner_InitRecords();
	gGameState.a_keys = KEY_A;
	gGameState.b_keys = KEY_B;
	gGameState.select_keys = KEY_SELECT;
	gGameState.start_keys = KEY_START;
	gGameState.first_shortcut_keys = KEY_L;
	gGameState.second_shortcut_keys = KEY_R;
	gGameState.dash_keys = KEY_B;
	gGameState.first_shortcut = 0;
	gGameState.second_shortcut = 0;
	gGameState.selected_actor = 0;
	Party_AddActiveOwner(0);
	gGameState.coins = 0;
	gGameState.message_speed = 1;
	gGameState.speech = 1;
	gGameState.unknown_20b = 1;
	gGameState.palette_glow[0] = 0;
	gGameState.palette_glow[1] = 8;
	gGameState.build_stamp = 0;
	gGameState.start_stamp = Runtime_GetBuildStampTime();
	gLoadedStateWord = 0;
	gGameState.play_time = 0;
	gOptionMirror = 0;
	gGameState.saved_options = gOptionMirror;
	gPostLoadCounter = 0;
	gSaveSlot = -1;
	gGameState.prize_weights[0] = 4;
	gGameState.prize_weights[1] = 4;
	gGameState.prize_weights[2] = 4;
	gGameState.prize_weights[3] = 8;
	gGameState.prize_weights[4] = 8;
	gGameState.prize_weights[5] = 8;
	gGameState.prize_weights[6] = 16;
	gGameState.prize_weights[7] = 16;
	gGameState.prize_weights[8] = 16;
	gGameState.prize_weights[9] = 32;
	gGameState.prize_weights[10] = 32;
	gGameState.prize_weights[11] = 32;
	gGameState.prize_weights[12] = 64;
	gGameState.prize_weights[13] = 64;
	gGameState.prize_weights[14] = 64;
}

void Party_SetFlag32AndRefreshMembers(void)
{
	GameFlag_SetBit(0x20);
	Owner_RefreshDerivedData(0);
	Owner_RefreshDerivedData(1);
	Owner_RefreshDerivedData(5);
	UnitCalc(0);
	UnitCalc(1);
	UnitCalc(5);
}

void Party_ApplyStatePreset(void)
{
	s32 id;

	GameFlag_ClearBit(32);
	GameFlag_ClearBit(33);
	GameFlag_SetBit(0x901);
	Owner_RefreshDerivedData(5);
	UnitCalc(5);
	GameFlag_ClearBit(0x11b);
	GameFlag_SetBit(282);

	for (id = 0; id <= 1; id++)
	{
		struct BtlUnit *unit = UnitGet(id);
		s32				ratio;
		s32				rate;
		s32				slot;

		Unit_Recover(unit);
		{
			s16 max_hp = unit->max_hp;

			ratio = (max_hp << 14) / max_hp;
		}
		rate = 0x4000;
		if (ratio <= rate)
		{
			rate = 0;
			if (ratio >= 0)
			{
				rate = ratio;
			}
		}
		unit->hp_gauge = rate;
		if ((rate << 16) == 0 && unit->hp != 0)
		{
			unit->hp_gauge = 1;
		}

		ratio = (unit->ep << 14) / unit->max_ep;
		rate = 0x4000;
		if (ratio <= rate)
		{
			rate = 0;
			if (ratio >= 0)
			{
				rate = ratio;
			}
		}
		unit->ep_gauge = rate;
		if ((rate << 16) == 0 && unit->ep != 0)
		{
			unit->ep_gauge = 1;
		}

		for (slot = 0; slot <= 14; slot++)
		{
			if ((unit->inventory[slot] & ITEM_ID_MASK) == 15)
			{
				unit->inventory[slot] = 16;
				Inventory_Equip(id, slot);
				break;
			}
		}

		Owner_RefreshDerivedData(id);
		UnitCalc(id);
	}

	OwnerAction_Add(0, 140);
	OwnerAction_Add(0, 149);
	OwnerAction_Add(1, 140);
	OwnerAction_Add(2, 141);

	gGameState.coins += 300;
}

void Owner_RefreshActiveRatios(s32 arg0)
{
	s32				count;
	s32				n;
	struct BtlUnit *obj;

	count = Party_CountActiveOwners();
	for (n = 0; n < count; n++)
	{
		obj = UnitGet(gGameState.active_owners[n]);

		Unit_Recover(obj);
		obj->hp_gauge = Vitals_Ratio(obj->hp, obj->max_hp);
		if (obj->hp_gauge == 0 && obj->hp != 0)
			obj->hp_gauge = 1;
		obj->ep_gauge = Vitals_Ratio(obj->ep, obj->max_ep);
		if (obj->ep_gauge == 0 && obj->ep != 0)
			obj->ep_gauge = 1;

		if (arg0 == 1)
		{
			obj->doku = 0;
			obj->akuryo = 0;
		}
	}
}

void Owner_RefreshRatiosOnFlag(void)
{
	s32				count;
	s32				n;
	u8				ownerId;
	s32				group;
	s32				doRefresh;
	struct BtlUnit *obj;

	count = Party_CountActiveOwners();
	for (n = 0; n < count; n++)
	{
		ownerId = gGameState.active_owners[n];
		group = Character_ElementGroupTable[ownerId];
		doRefresh = 0;
		if (group == 0)
		{
			if (GameFlag_Test(0x110) || GameFlag_Test(0x112))
			{
				doRefresh = 1;
			}
		}
		else
		{
			if (GameFlag_Test(0x111) || GameFlag_Test(0x113))
			{
				doRefresh = 1;
			}
		}

		if (doRefresh == 0)
		{
			continue;
		}

		obj = UnitGet(ownerId);
		/* FAKEMATCH: the one-pass block keeps the EP store ahead of the HP
		 * gauge's loads, as in Owner_UpdateSecondInputAndRatios. Measured:
		 * the plain store differs 4, with the gauges through Vitals_Ratio
		 * or written out in full. */
		do
		{
			obj->ep = obj->max_ep;
		} while (0);
		obj->hp_gauge = Vitals_Ratio(obj->hp, obj->max_hp);
		if (obj->hp_gauge == 0 && obj->hp != 0)
			obj->hp_gauge = 1;
		obj->ep_gauge = Vitals_Ratio(obj->ep, obj->max_ep);
		if (obj->ep_gauge == 0 && obj->ep != 0)
			obj->ep_gauge = 1;
	}
}

void Owner_RatioNoOp(void)
{
}

void UnitRatio(s32 owner_no)
{
	s32				first;
	s32				second;
	s32				first_value;
	s32				second_value;
	struct BtlUnit *owner;

	owner = UnitGet(owner_no);
	first = (owner->hp << 14) / owner->max_hp;
	first_value = 0x4000;
	if (first <= 0x4000)
	{
		first_value = 0;
		if (first >= 0)
		{
			first_value = first;
		}
	}
	owner->hp_gauge = first_value;
	if (((first_value << 16) == 0) && (owner->hp != 0))
	{
		first_value = 1;
		owner->hp_gauge = first_value;
	}
	second = (owner->ep << 14) / owner->max_ep;
	second_value = 0x4000;
	if (second <= 0x4000)
	{
		second_value = 0;
		if (second >= 0)
		{
			second_value = second;
		}
	}
	owner->ep_gauge = second_value;
	if (((second_value << 16) == 0) && (owner->ep != 0))
	{
		second_value = 1;
		owner->ep_gauge = second_value;
	}
}

void Owner_UpdateRatioPair(struct BtlUnit *state, s32 input)
{
	s32 value;

	if (input > state->max_hp)
	{
		value = state->max_hp;
	}
	else
	{
		value = 0;
		if (input >= 0)
		{
			value = input;
		}
	}
	state->hp = value;
	value = ((value << 16) >> 2) / state->max_hp;

	{
		s32 output = 0x4000;

		if (value <= output)
		{
			output = 0;
			if (value >= 0)
			{
				output = value;
			}
		}
		state->hp_gauge = output;
		if ((output << 16) == 0 && state->hp != 0)
		{
			state->hp_gauge = 1;
		}
	}

	value = (state->ep << 14) / state->max_ep;
	{
		s32 output = 0x4000;

		if (value <= output)
		{
			output = 0;
			if (value >= 0)
			{
				output = value;
			}
		}
		state->ep_gauge = output;
		if ((output << 16) == 0 && state->ep != 0)
		{
			state->ep_gauge = 1;
		}
	}
}

//--------------------------------------------------------------------------

// ratios_second_input.c

// キャラクタ：EP の設定とゲージの計算

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// EP を 0 から最大 EP の間で設定し、HP と EP のゲージを計算し直す
// ゲージは 0x4000 で満タン。残っていれば最低 1 にする
// unit : ユニット
// ep   : 新しい EP
//--------------------------------------------------------------------------
void Owner_UpdateSecondInputAndRatios(struct BtlUnit *unit, s32 ep)
{
	s32 cur;
	s32 rate;

	if (ep > unit->max_ep)
	{
		cur = unit->max_ep;
	}
	else
	{
		cur = 0;
		if (ep >= 0)
			cur = ep;
	}
	/* FAKEMATCH: the one-pass block keeps the EP store ahead of the HP
	 * gauge's loads. Measured: the plain store lets the scheduler lift
	 * the hp load above it (differ 2); the store in each arm of the clamp
	 * differs 40 to 43; the gauges through Vitals_Ratio, or through an
	 * inline Unit_UpdateGauges as object.c has, still differ 2. */
	do
	{
		unit->ep = cur;
	} while (0);

	// HP のゲージ
	rate = unit->hp;
	rate <<= 14;
	rate = rate / unit->max_hp;
	{
		s32 gauge = 0x4000;

		if (rate <= gauge)
		{
			gauge = 0;
			if (rate >= 0)
				gauge = rate;
		}
		unit->hp_gauge = gauge;
		if ((gauge << 16) == 0 && unit->hp != 0)
			unit->hp_gauge = 1;
	}

	// EP のゲージ
	{
		s32 num;
		s32 max;

		num = unit->ep;
		max = unit->max_ep;
		rate = (num << 14) / max;
	}
	{
		s32 gauge = 0x4000;

		if (rate <= gauge)
		{
			gauge = 0;
			if (rate >= 0)
				gauge = rate;
		}
		unit->ep_gauge = gauge;
		if ((gauge << 16) == 0 && unit->ep != 0)
			unit->ep_gauge = 1;
	}
}

s32 Owner_AdjustFirstValue(s32 owner, s32 delta)
{
	struct BtlUnit *state = UnitGet(owner);
	s32				cur = state->hp;
	s32				max = state->max_hp;
	s32				value = cur + delta;
	s32				result;

	if (value > max)
		result = max;
	else
	{
		result = 0;
		if (value >= 0)
			result = value;
	}
	state->hp = result;
	UnitRatio(owner);
	return state->hp;
}

s16 UnitEpAdd(s32 owner, s32 delta)
{
	struct BtlUnit *state = UnitGet(owner);
	s32				cur = state->ep;
	s32				max = state->max_ep;
	s32				value = cur + delta;
	s32				result;

	if (value > max)
		result = max;
	else
	{
		result = 0;
		if (value >= 0)
			result = value;
	}
	state->ep = result;
	UnitRatio(owner);
	return state->ep;
}

//--------------------------------------------------------------------------

// definition.c

// アイテム：定義データの参照と装備できるかの判定

//--------------------------------------------------------------------------
