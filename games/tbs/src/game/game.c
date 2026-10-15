#include "unit/game/game.h"
#include "unit/runtime/runtime.h"
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
#include "battle_system.h"
#include "text/msg_ids.h"
#include "battle_summon.h"
#include "preset_table.h"
#include "iwram_call.h"
#include "battle_calc.h"
#include "battle_random.h"
#include "battle_type.h"
#include "battle_effect_chance.h"
#include "battle_party.h"
#include "field_event.h"
#include "unit/game/starting_equipment.h"

struct StatWork
{
	s32					   hp;
	s32					   pp;
	s32					   atk;
	s32					   def;
	s32					   agi;
	s32					   unk14;
	s32					   luck;
	s32					   turns;
	s32					   hpRegen;
	s32					   ppRegen;
	s32					   element[4][2];
	s32					   kind;
	s32					   unk4c[2];
	s32					   amount;
	struct ItemDefinition *item;
	s32					   unk5c;
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
	return &gGameState.unk008[4];
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
		total += UnitGet(gGameState.activeOwners[i])->level;
	}
	total = total / count;
	return total;
}

struct BattleUnit *UnitGet(u32 owner)
{
	struct BattleUnit *states;
	struct BattleUnit *unit;

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
		unit = &((struct BattleUnit *)gBattleOwnerStates)[owner - 0x80];
		return unit;
	}
	return NULL;
}

const u8 *Owner_GetRecord(s32 selector)
{
	u32 recordIndex;

	recordIndex = selector - 8;
	if (recordIndex > 0xF9U)
	{
		recordIndex = 0;
	}
	return (const u8 *)&gEnemy_Definitions[recordIndex];
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
	struct BattleUnit	*st;
	s32				 i, j, el;
	s32				 value;
	s32				 flag;
	s32				 kind;
	s32				 amount;
	s32				 cap;
	s16				 old;

	work = Runtime_BumpAllocateAlternatePool(sizeof(struct StatWork));
	st = UnitGet(owner);
	work->hp = st->baseHp;
	work->pp = st->basePp;
	work->atk = st->baseAttack;
	work->def = st->baseDefense;
	work->agi = st->baseAgility;
	work->luck = st->baseLuck;
	work->turns = st->baseActionCount & 15;
	work->hpRegen = st->baseHpRegen;
	work->ppRegen = st->basePpRegen;
	{
		s16 *src = &st->baseElement[0].power;
		s32 *dst = work->element[0];
		for (i = 0; i < 4; i++)
		{
			dst[0] = src[0];
			dst[1] = src[1];
			src += 2;
			dst += 2;
		}
	}

	if (STAT_DIFF(st->maxHp * st->hpGauge / 0x4000, st->hp) > 1
		|| STAT_DIFF(st->maxPp * st->ppGauge / 0x4000, st->pp) > 1)
	{
		st->hpGauge = 0x4000;
		st->ppGauge = 0x4000;
		st->hp = st->maxHp;
		st->pp = st->maxPp;
	}

	st->restraint &= ~3;
	if (st->restraint & 4)
		st->restraint |= 1;
	if (st->ready)
		work->turns++;
	st->unk142[0] = 0;
	st->unk142[1] = 0;

	if (st->classIndex)
	{
		for (i = 0; i < 15; i++)
		{
			if (!(st->inventory[i] & INVENTORY_EQUIPPED))
				continue;
			work->item = Item_GetDirect(st->inventory[i]);
			if (work->item->flags & ITEM_CURSED)
				st->restraint |= 3;
			/* FAKEMATCH: the one-pass block keeps the defense load after the
			 * item's second bonus load, as the ROM schedules it. Measured:
			 * plain, the scheduler lifts work->def above that load (differ 2). */
			do
			{
				work->atk += work->item->primaryBonus;
			} while (0);
			work->def += work->item->secondaryBonus;
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
					work->hpRegen += work->amount;
					break;
				case ITEM_EFFECT_ADD_PP:
					work->pp += work->amount;
					break;
				case ITEM_EFFECT_ADD_PP_REGEN:
					work->ppRegen += work->amount;
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
					st->unk142[0] += work->amount;
					break;
				case 24:
					st->unk142[1] += work->amount;
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
			u32 bits = st->djinnActive[el];

			for (i = 0; i < 20; i++)
			{
				if (bits & (1 << i))
				{
					struct DjinnDefinition *djinn = Djinn_GetDefinition(el, i);

					work->hp += djinn->hp;
					work->pp += djinn->pp;
					work->atk += djinn->atk;
					work->def += djinn->def;
					work->agi += djinn->agi;
					work->luck += djinn->luck;
				}
			}
		}

		{
			struct ClassDefinition *class = Owner_GetRecordStride84(st->classIndex);

			work->hp = work->hp * class->hp / 10;
			work->pp = work->pp * class->pp / 10;
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
					work->hpRegen = work->hpRegen * work->amount / 10;
					break;
				case 2:
					work->pp = work->pp * work->amount / 10;
					break;
				case 3:
					work->ppRegen = work->ppRegen * work->amount / 10;
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

	work->atk = work->atk * (st->atkLv + 8) / 8;
	work->def = work->def * (st->defLv + 8) / 8;
	work->agi = work->agi * (st->agiLv + 8) / 8;
	for (i = 0; i < 4; i++)
		work->element[i][0] += (st->elementLevels[i] * st->elementLevels[i] + st->elementLevels[i]) * 5;
	for (i = 0; i < 4; i++)
		work->element[i][1] += st->resLv * 20;

	if (st->classIndex)
	{
		flag = 0;
		switch (st->classId)
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
			work->ppRegen += 4;
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
	if (work->hpRegen < 0)
		work->hpRegen = 0;
	if (work->hpRegen > 10000)
		work->hpRegen = 10000;
	if (work->ppRegen < 0)
		work->ppRegen = 0;
	if (work->ppRegen > 200)
		work->ppRegen = 200;
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
	st->actionEntryCount = work->turns;
	st->hpRegen = work->hpRegen;
	st->ppRegen = work->ppRegen;
	for (i = 0; i < 4; i++)
	{
		st->element[i].power = work->element[i][0];
		st->element[i].res = work->element[i][1];
	}

	cap = 9999;
	if (st->classIndex)
		cap = 1999;

	old = st->maxHp;
	if (work->hp < 0)
		work->hp = 0;
	if (work->hp > cap)
		work->hp = cap;
	st->maxHp = work->hp;
	if (old != st->maxHp)
	{
		value = work->hp * st->hpGauge / 0x4000;
		if (value < 0)
			value = 0;
		if (value > cap)
			value = cap;
		if (st->hp != 0 && value == 0)
			value = 1;
		st->hp = value;
	}

	old = st->maxPp;
	if (work->pp < 0)
		work->pp = 0;
	if (work->pp > cap)
		work->pp = cap;
	st->maxPp = work->pp;
	if (old != st->maxPp)
	{
		value = work->pp * st->ppGauge / 0x4000;
		if (value < 0)
			value = 0;
		if (value > cap)
			value = cap;
		if (st->pp != 0 && value == 0)
			value = 1;
		st->pp = value;
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
		struct BattleUnit *owner;
		s32				i;

		owner = UnitGet(gGameState.activeOwners[n]);
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
	u8	unk000[0x100];
	u32 playTime;
	u8	unk104;
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
	START_DMA(&zero, gGameFlagBytes, 0x85000080);
	((struct SceneStateTail *)gSceneState)->unk104 = 0xff;
	zero = 0;
	START_DMA(&zero, gSaveStamp + 0x1c, 0x85000298);
	Owner_InitRecords();
	gGameState.aKeys = KEY_A;
	gGameState.bKeys = KEY_B;
	gGameState.selectKeys = KEY_SELECT;
	gGameState.startKeys = KEY_START;
	gGameState.firstShortcutKeys = KEY_L;
	gGameState.secondShortcutKeys = KEY_R;
	gGameState.dashKeys = KEY_B;
	gGameState.firstShortcut = 0;
	gGameState.secondShortcut = 0;
	gGameState.selectedActor = 0;
	Party_AddActiveOwner(0);
	gGameState.coins = 0;
	gGameState.messageSpeed = 1;
	gGameState.speech = 1;
	gGameState.unk20b = 1;
	gGameState.paletteGlow[0] = 0;
	gGameState.paletteGlow[1] = 8;
	gGameState.buildStamp = 0;
	gGameState.startStamp = Runtime_GetBuildStampTime();
	gLoadedStateWord = 0;
	gGameState.playTime = 0;
	gOptionMirror = 0;
	gGameState.savedOptions = gOptionMirror;
	gPostLoadCounter = 0;
	gSaveSlot = -1;
	gGameState.prizeWeights[0] = 4;
	gGameState.prizeWeights[1] = 4;
	gGameState.prizeWeights[2] = 4;
	gGameState.prizeWeights[3] = 8;
	gGameState.prizeWeights[4] = 8;
	gGameState.prizeWeights[5] = 8;
	gGameState.prizeWeights[6] = 16;
	gGameState.prizeWeights[7] = 16;
	gGameState.prizeWeights[8] = 16;
	gGameState.prizeWeights[9] = 32;
	gGameState.prizeWeights[10] = 32;
	gGameState.prizeWeights[11] = 32;
	gGameState.prizeWeights[12] = 64;
	gGameState.prizeWeights[13] = 64;
	gGameState.prizeWeights[14] = 64;
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
		struct BattleUnit *unit = UnitGet(id);
		s32				slot;

		unit->hp = unit->maxHp;
		unit->pp = unit->maxPp;
		Unit_UpdateGauges(unit);

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
	struct BattleUnit *obj;

	count = Party_CountActiveOwners();
	for (n = 0; n < count; n++)
	{
		obj = UnitGet(gGameState.activeOwners[n]);

		obj->hp = obj->maxHp;
		obj->pp = obj->maxPp;
		Unit_UpdateGauges(obj);

		if (arg0 == 1)
		{
			obj->poison = 0;
			obj->haunt = 0;
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
	struct BattleUnit *obj;

	count = Party_CountActiveOwners();
	for (n = 0; n < count; n++)
	{
		ownerId = gGameState.activeOwners[n];
		group = gCharacter_ElementGroupTable[ownerId];
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
		obj->pp = obj->maxPp;
		Unit_UpdateGauges(obj);
	}
}

void Owner_RatioNoOp(void)
{
}

void UnitRatio(s32 owner_id)
{
	struct BattleUnit *owner;

	owner = UnitGet(owner_id);
	Unit_UpdateGauges(owner);
}

void Owner_UpdateRatioPair(struct BattleUnit *state, s32 input)
{
	s32 value;

	if (input > state->maxHp)
	{
		value = state->maxHp;
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
	Unit_UpdateGauges(state);
}

//--------------------------------------------------------------------------
// ratios_second_input.c
// キャラクタ：PP の設定とゲージの計算
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// PP を 0 から最大 PP の間で設定し、HP と PP のゲージを計算し直す
// ゲージは 0x4000 で満タン。残っていれば最低 1 にする
// unit : ユニット
// pp   : 新しい PP
//--------------------------------------------------------------------------
void Owner_UpdateSecondInputAndRatios(struct BattleUnit *unit, s32 pp)
{
	s32 cur;

	if (pp > unit->maxPp)
	{
		cur = unit->maxPp;
	}
	else
	{
		cur = 0;
		if (pp >= 0)
			cur = pp;
	}
	unit->pp = cur;
	Unit_UpdateGauges(unit);
}

s32 Owner_AdjustFirstValue(s32 owner, s32 delta)
{
	struct BattleUnit *state = UnitGet(owner);
	s32				cur = state->hp;
	s32				max = state->maxHp;
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

s16 UnitPpAdd(s32 owner, s32 delta)
{
	struct BattleUnit *state = UnitGet(owner);
	s32				cur = state->pp;
	s32				max = state->maxPp;
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
	state->pp = result;
	UnitRatio(owner);
	return state->pp;
}

//--------------------------------------------------------------------------
// definition.c
// アイテム：定義データの参照と装備できるかの判定
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// アイテムの定義データを返す
// item : アイテム番号（持ち物の値なら上位の装備・個数ビットを無視する）
//--------------------------------------------------------------------------
struct ItemDefinition *Item_GetDirect(s32 item)
{
	return gItem_DefinitionTable + (item & ITEM_ID_MASK);
}

//--------------------------------------------------------------------------
// キャラクターがアイテムを装備できるか
// equipMask の classId 番目のビットで決まり、classId が 8 以上なら 0
//--------------------------------------------------------------------------
s32 Item_CanOwnerEquipDirect(s32 unit, s32 item)
{
	struct BattleUnit		  *st = UnitGet(unit);
	struct ItemDefinition *def = Item_GetDirect(item);
	s32					   mask = def->equipMask;

	if (st->classId >= 8)
	{
		return 0;
	}
	return (mask >> st->classId) & 1;
}

//--------------------------------------------------------------------------
// 装備品でなければ 1、装備品ならキャラクターが装備できるかを返す
//--------------------------------------------------------------------------
s32 Item_IsCompatibleWithOwner(s32 unit, s32 item)
{
	if (Item_GetEquipmentGroup(item) == 0)
	{
		return 1;
	}
	return Item_CanOwnerEquipDirect(unit, item);
}

s32 Item_GetEquipmentGroup(s32 item_id)
{
	s32					   group;
	struct ItemDefinition *item;

	item = Item_GetDirect(item_id);
	group = 0;
	if (item->type == 1)
		group = 1;
	else if (item->type == 2)
		group = 2;
	else if (item->type == 3)
		group = 2;
	else if (item->type == 4)
		group = 2;
	else if (item->type == 5)
		group = 2;
	else if (item->type == 9)
		group = 2;
	return group;
}

s32 Inventory_GetQuantity(s32 owner, s32 slot)
{
	s32 item_id;

	owner = UnitGet(owner)->inventory[slot];
	item_id = ITEM_ID_MASK;
	item_id &= owner;
	owner = (u32)owner >> INVENTORY_QUANTITY_SHIFT;
	owner++;
	if (item_id == 0)
	{
		owner = 0;
	}
	return owner;
}

s32 Inventory_Count(s32 owner)
{
	struct BattleUnit *inv = UnitGet(owner);
	s32				count = 0;

	if (inv->inventory[count] != 0)
	{
		do
		{
			count++;
			if (count > 14)
				break;
		} while (inv->inventory[count] != 0);
	}
	return count;
}

s32 PartyInventory_HasSpace(void)
{
	s16	 owners[10];
	s32	 ownerCount;
	s32	 ownerIndex;
	s16 *ownerCursor;

	if (Inventory_Count(gGameState.selectedActor) != 15)
		return 1;
	ownerCount = Party_ListActiveOwners(owners);
	ownerCursor = owners;
	ownerIndex = 0;
	if (ownerIndex < ownerCount)
	{
		do
		{
			if (Inventory_Count(*ownerCursor++) != 15)
				return 1;
			ownerIndex++;
		} while (ownerIndex < ownerCount);
	}
	return 0;
}

s32 PartyInventory_CountFreeSlots(void)
{
	s16	 owners[10];
	s32	 ownerCount = Party_ListActiveOwners(owners);
	s32	 cnt = 0;
	s16 *ownerCursor = owners;

	if (cnt < ownerCount)
	{
		s32 n = ownerCount;

		do
		{
			cnt = cnt - Inventory_Count(*ownerCursor++) + 15;
			n--;
		} while (n != 0);
	}
	return cnt;
}

s32 Inventory_AddItem(s32 owner_id, s32 item_id)
{
	struct BattleUnit		  *inv = UnitGet(owner_id);
	struct ItemDefinition *item = Item_GetDirect(item_id);
	s32					   slot;

	if ((item->flags & ITEM_STACKABLE) != 0)
	{
		slot = 0;
		if (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0)
		{
			do
			{
				slot++;
				if (slot >= INVENTORY_SLOTS)
					break;
			} while (((inv->inventory[slot] ^ item_id) & ITEM_ID_MASK) != 0);
		}
		if (slot != INVENTORY_SLOTS)
		{
			s32 entry = inv->inventory[slot];
			u32 count = ((u32)entry >> INVENTORY_QUANTITY_SHIFT) + 1;

			if (count > 29)
				return -1;
			{
				s32 value = INVENTORY_ENTRY_MASK;

				value &= entry;
				value |= count << INVENTORY_QUANTITY_SHIFT;
				inv->inventory[slot] = value;
			}
			return slot;
		}
	}

	slot = 0;
	do
	{
		if (inv->inventory[slot] == 0)
		{
			inv->inventory[slot] = item_id;
			return slot;
		}
		slot++;
	} while (slot < INVENTORY_SLOTS);
	return -1;
}

s32 PartyInventory_Add(s32 itemId)
{
	s16	 owners[10];
	s32	 ownerCount;
	s32	 ownerIndex;
	s16 *ownerCursor;

	ownerCount = Party_ListActiveOwners(owners);
	ownerCursor = owners;
	ownerIndex = 0;
	if (ownerIndex < ownerCount)
	{
		do
		{
			s16 ownerId = *ownerCursor++;

			if (Inventory_AddItem(ownerId, itemId) >= 0)
				return ownerId;
			ownerIndex++;
		} while (ownerIndex < ownerCount);
	}
	return -1;
}

s32 Inventory_Find(s32 owner, s32 item_id)
{
	struct BattleUnit *inv = UnitGet(owner);
	s32				slot = 0;
	u16			   *entry = inv->inventory;

	do
	{
		if (((*entry++) & ITEM_ID_MASK) == item_id)
		{
			return slot;
		}
		slot++;
	} while (slot < INVENTORY_SLOTS);
	return -1;
}

s32 PartyInventory_FindOwner(s32 itemId)
{
	s16	 owners[10];
	s32	 ownerCount;
	s32	 ownerIndex;
	s16 *ownerCursor;
	s16	 owner;

	if (Inventory_Find(gGameState.selectedActor, itemId) != -1)
		return gGameState.selectedActor;
	ownerCount = Party_ListActiveOwners(owners);
	ownerCursor = owners;
	ownerIndex = 0;
	if (ownerIndex < ownerCount)
	{
		do
		{
			owner = *ownerCursor++;
			if (Inventory_Find(owner, itemId) != -1)
				return owner;
			ownerIndex++;
		} while (ownerIndex < ownerCount);
	}
	return -1;
}

s32 Inventory_Equip(s32 owner, s32 slot)
{
	struct BattleUnit		  *inv = UnitGet(owner);
	u32					   item_id = inv->inventory[slot];
	struct ItemDefinition *item;
	u8					   type;
	s32					   other;

	if (Item_CanOwnerEquipDirect(owner, item_id) == 0)
		return -1;
	if (item_id & INVENTORY_EQUIPPED)
		return 0;

	item = Item_GetDirect(item_id);
	type = item->type;
	if (type != 6)
	{
		for (other = 0; other <= 14; other++)
		{
			if ((inv->inventory[other] & INVENTORY_EQUIPPED) == 0)
				continue;
			if (Item_GetDirect(inv->inventory[other])->type == type)
				break;
		}

		if (other != 15)
		{
			item = Item_GetDirect(inv->inventory[other]);
			if (item->flags & ITEM_CANNOT_UNEQUIP)
				return -2;
			inv->inventory[other] &= 0xfdff;
		}
	}

	inv->inventory[slot] |= INVENTORY_EQUIPPED;
	Owner_RefreshClassActions(owner);
	UnitCalc(owner);
	return 0;
}

s32 Inventory_FindEquipped(s32 owner, s32 type)
{
	struct BattleUnit		  *base = UnitGet(owner);
	s32					   index;
	struct ItemDefinition *item;

	for (index = 0; index < INVENTORY_SLOTS; index++)
	{
		if (base->inventory[index] & INVENTORY_EQUIPPED)
		{
			item = Item_GetDirect(base->inventory[index]);
			if (item->type == type)
				break;
		}
	}
	if (index == 15)
		index = -1;
	return index;
}

struct ItemDefinition *Inventory_GetEquippedDefinition(struct BattleUnit *inv, s32 type)
{
	s32					   slot;
	struct ItemDefinition *item;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (inv->inventory[slot] & INVENTORY_EQUIPPED)
		{
			item = Item_GetDirect(inv->inventory[slot]);
			if (item->type == type)
			{
				return item;
			}
		}
	}
	return 0;
}

s32 Inventory_GetEquippedItem(struct BattleUnit *inv, s32 type)
{
	s32 slot;

	for (slot = 0; slot < INVENTORY_SLOTS; slot++)
	{
		if (inv->inventory[slot] & INVENTORY_EQUIPPED)
		{
			struct ItemDefinition *item = Item_GetDirect(inv->inventory[slot]);

			if (item->type == type)
			{
				return inv->inventory[slot] & ITEM_ID_MASK;
			}
		}
	}
	return 0;
}

s32 Inventory_Remove(s32 owner, s32 slot)
{
	struct BattleUnit *inv = UnitGet(owner);
	u16				item = inv->inventory[slot];
	s32				result = -1;

	if (item != 0)
	{
		if (item & INVENTORY_QUANTITY_MASK)
		{
			inv->inventory[slot] = item - INVENTORY_QUANTITY_STEP;
			result = 1;
		}
		else
		{
			s16 *list;
			s32	 count;
			s32	 i;

			inv->inventory[slot] = 0;
			list = inv->inventory;
			count = 0;
			for (i = 0; i < 15; i++)
			{
				if (list[i] != 0)
					list[count++] = list[i];
			}
			for (; count < 15; count++)
				list[count] = 0;
			result = 2;
		}
	}
	UnitCalc(owner);
	return result;
}

s32 Inventory_Discard(s32 owner, s32 slot)
{
	s32 item = UnitGet(owner)->inventory[slot];
	s32 removed_slot = Inventory_Remove(owner, slot);

	if (removed_slot != -1)
	{
		Item_AdjustCounter(item, 1);
		Event_ClearInvalidPackedValuesFar();
	}
	return removed_slot;
}

s32 Inventory_CheckDiscard(s32 owner, s32 slot)
{
	struct BattleUnit		  *inv = UnitGet(owner);
	s32					   item_id = inv->inventory[slot] & ITEM_ID_MASK;
	struct ItemDefinition *item = Item_GetDirect(item_id);

	if (item_id == 0)
	{
		return -1;
	}
	if ((item->flags & ITEM_INDISPENSABLE) != 0)
	{
		return -4;
	}
	if ((inv->inventory[slot] & INVENTORY_EQUIPPED) != 0
		&& (item->flags & ITEM_CANNOT_UNEQUIP) != 0)
	{
		return -3;
	}
	return 0;
}

s32 PartyInventory_Remove(s32 item_id)
{
	s32 owner = PartyInventory_FindOwner(item_id);

	if (owner == -1)
		return 0;
	Inventory_Remove(owner, Inventory_Find(owner, item_id));
	return 0;
}

s32 PartyInventory_Discard(s32 item_id)
{
	s32 owner = PartyInventory_FindOwner(item_id);

	if (owner == -1)
		return 0;
	Inventory_Discard(owner, Inventory_Find(owner, item_id));
	return 0;
}

s32 Inventory_Break(s32 owner, s32 slot)
{
	struct BattleUnit *inv = UnitGet(owner);
	if (inv->inventory[slot] == 0)
	{
		return -1;
	}
	inv->inventory[slot] |= INVENTORY_BROKEN;
	return 0;
}

s32 Inventory_Repair(s32 owner, s32 slot)
{
	struct BattleUnit *inv = UnitGet(owner);
	if (inv->inventory[slot] == 0)
	{
		return -1;
	}
	inv->inventory[slot] &= ~INVENTORY_BROKEN;
	return 0;
}

u8 Item_GetTargetMode(s32 itemId)
{
	return BattleAction_GetDirect(Item_GetDirect(itemId)->actionId)->targetMode;
}

s32 ItemCounter_Adjust(s32 index, s32 delta)
{
	s32 counter_slot = index;
	u8 *data = gItemCounters;

	index = 0;
	if (counter_slot <= 127)
	{
		s32 value = data[counter_slot];

		value += delta;

		if (value < 0)
		{
			value = 0;
		}
		else if (value > 99)
		{
			value = 99;
			index = 99;
		}
		else
		{
			index = value;
		}
		data[counter_slot] = value;
	}
	return index;
}

s32 Item_AdjustCounter(s32 itemId, s32 delta)
{
	s32 itemIdMask = ITEM_ID_MASK;
	u8	counter;
	s32 result = 0;

	counter = gItem_ArtifactSlotTable[itemId & itemIdMask];
	if (counter != 0)
	{
		result = ItemCounter_Adjust(counter - 1, delta);
	}
	return result;
}

s32 Inventory_CountItem(s32 owner, s32 item_id)
{
	struct BattleUnit *base = UnitGet(owner);
	s32				count = 0;
	s32				target = item_id & ITEM_ID_MASK;
	s32				index = 0;

	do
	{
		if ((base->inventory[index] & ITEM_ID_MASK) == target)
		{
			struct ItemDefinition *item = Item_GetDirect(target);

			if (item->flags & ITEM_STACKABLE)
			{
				count = (base->inventory[index] >> INVENTORY_QUANTITY_SHIFT) + 1;
				break;
			}
			count++;
		}
		index++;
	} while (index < INVENTORY_SLOTS);
	return count;
}

s32 PartyInventory_CountItem(s32 itemId)
{
	u16 owners[16];
	s32 itemCount = 0;
	s32 ownerCount = Party_ListActiveOwners(owners);

	if (itemCount < ownerCount)
	{
		u16 *ownerCursor = owners;
		s32	 n = ownerCount;

		do
		{
			itemCount += Inventory_CountItem(*ownerCursor++, itemId);
			n--;
		} while (n != 0);
	}
	return itemCount;
}

struct BattleAct *BattleAction_GetDirect(s32 actionId)
{
	u32 entryIndex;

	entryIndex = actionId & 0x3fff;
	if (entryIndex >= 0x208U)
	{
		entryIndex = 0;
	}
	return &gBattleAction_DefinitionTable[entryIndex];
}

s32 Equipment_HasValue(s32 owner, s32 value)
{
	struct BattleUnit		   *unit = UnitGet(owner);
	struct OwnerActionSlot *entry;
	s32						mask = 0x3fff;
	s32						index = 0;

	entry = unit->actionSlots;
	do
	{
		s32 current = entry->encodedAction;

		current &= mask;
		entry++;
		if (current == value)
		{
			return 1;
		}
		index++;
	} while (index <= 31);
	return 0;
}

enum
{
	OWNER_ACTION_CLASS = 0x8000,
	OWNER_ACTION_ITEM = 0x4000
};

s32 Owner_RefreshClassActions(s32 owner)
{
	struct BattleUnit		   *st;
	u16					   *ability;
	struct ClassDefinition *cls;
	struct ItemDefinition  *def;
	s32						i;
	s32						j;
	u16						mask;
	struct ClassAbility	   *cg;

	st = UnitGet(owner);
	ability = (u16 *)st->actionSlots;
	cls = Owner_GetRecordStride84(st->classIndex);
	if (st->classIndex == 0)
		return 0;
	mask = 0x8000;
	for (j = 0; j < 32; j++)
	{
		if (ability[j * 2] & mask)
			ability[j * 2] = 0;
	}
	mask = 0x4000;
	for (j = 0; j < 32; j++)
	{
		if (ability[j * 2] & mask)
			ability[j * 2] = 0;
	}
	for (i = 31, j = 31; i >= 0;)
	{
		if (ability[i * 2] == 0)
		{
			i--;
		}
		else
		{
			ability[j * 2] = ability[i * 2];
			i--;
			j--;
		}
	}
	for (; j >= 0; j--)
		ability[j * 2] = 0;

	mask = OWNER_ACTION_CLASS;
	for (j = 0; j < 16; j++)
	{
		cg = &cls->abilities[j];
		if (cls->abilities[j].ability == 0)
			continue;
		if (st->level < cls->abilities[j].level)
			continue;
		i = 0;
		while (i < 32 && ability[i * 2] != cls->abilities[j].ability)
			i++;
		if (i != 32)
			continue;
		for (i = 0; i < 32; i++)
		{
			if (ability[i * 2] == 0)
			{
				ability[i * 2] = cg->ability | mask;
				break;
			}
		}
		if (i == 32)
			break;
	}

	for (j = 0; j < 15; j++)
	{
		if (st->inventory[j] == 0)
			continue;
		if (!(st->inventory[j] & INVENTORY_EQUIPPED))
			continue;
		def = Item_GetDirect(st->inventory[j]);
		if (def->useType != 3)
			continue;
		for (i = 0; i < 32; i++)
		{
			if ((ability[i * 2] & OWNER_ACTION_ID_MASK) == def->actionId)
				break;
		}
		if (i != 32)
			continue;
		for (i = 0; i < 32; i++)
		{
			if (ability[i * 2] == 0)
			{
				ability[i * 2] = def->actionId | OWNER_ACTION_ITEM;
				break;
			}
		}
		if (i == 32)
			break;
	}

	for (i = 0, j = 0; i < 32;)
	{
		if (ability[i * 2] == 0)
		{
			i++;
		}
		else
		{
			ability[j * 2] = ability[i * 2];
			i++;
			j++;
		}
	}
	for (; j < 32; j++)
		ability[j * 2] = 0;
	return 0;
}

struct LevelUpWork
{
	s32							classIndex;
	s32							level;
	struct CharacterDefinition *growth;
	u8							unk0c[0x20];
};

s32 OwnerAction_Add(s32 stateIndex, s32 value)
{
	struct BattleUnit *state = UnitGet(stateIndex);
	s32				key = value & OWNER_ACTION_ID_MASK;
	s32				found = -1;
	s32				index;

	for (index = 0; index <= 30; index++)
	{
		s32 masked = state->actionSlots[index].encodedAction & OWNER_ACTION_ID_MASK;

		if ((masked ^ key) == 0)
		{
			state->actionSlots[index].encodedAction = masked;
			found = index;
			break;
		}
	}

	if (found < 0)
	{
		/* FAKEMATCH: the scalar member-address walk: the slot offset is index * 4 + 88 from state, recomputed each pass as the bytes do. Plain forms measured 2026-10-13: state->actionSlots[index].encoded_action differ 22 (75 instructions, loop.c walks the slot address); the slot through a u16 pointer, a struct OwnerActionSlot pointer or a u16 view of the array differ 22 (79 instructions). */
		for (index = 0; index <= 30; index++)
		{
			s32 offset = index * sizeof(state->actionSlots[0]) + (s32)
						 & ((struct BattleUnit *)0)->actionSlots;

			if (*(u16 *)((u8 *)state + offset) == 0)
			{
				*(u16 *)((u8 *)state + offset) = key;
				found = index;
				break;
			}
		}
		if (found < 0)
		{
			return -1;
		}
	}

	Owner_RefreshClassActions(stateIndex);
	for (index = 0; index <= 31; index++)
	{
		if (state->actionSlots[index].encodedAction == key)
		{
			break;
		}
	}
	return index;
}

struct LevelUpResult *OwnerAction_CheckLevelThreshold(s32 owner, struct LevelUpResult *value)
{
	return Owner_GetValueIfLevelThresholdReached(owner, value);
}

struct CharacterDefinition *Character_GetDefinition(s32 index)
{
	struct CharacterDefinition *base;

	base = gCharacter_DefinitionTable;
	return &base[index];
}

void Owner_InitRecords(void)
{
	struct BattleUnit			   *state;
	struct CharacterDefinition *tmpl;
	u16							nameBuf[16];
	s32							owner;
	s32						   *remote = gCharacter_StartingEquipOwnerIds;
	s32							i;
	s32							slot;
	u8						   *name;

	for (owner = 0; owner <= 7; owner++)
	{
		state = UnitGet(owner);
		Ui_AdjustValueWithoutLimitFar(owner + MsgCharacterName, nameBuf);
		name = state->name;
		name[0] = nameBuf[0];
		i = 0;
		if (nameBuf[0] != 0)
		{
			do
			{
				i++;
				if (i > 13)
					break;
				name[i] = nameBuf[i];
			} while (nameBuf[i] != 0);
		}
		state->name[14] = 0;
	}

	if (*remote != -1)
	{
		do
		{
			state = UnitGet(*remote);
			if (state != 0)
			{
				state->classId = *remote;
				tmpl = Character_GetDefinition(state->classId);

				for (i = 14; i >= 0; i--)
					state->inventory[i] = 0;

				for (i = 0; i < sizeof(tmpl->items) / sizeof(tmpl->items[0]); i++)
				{
					slot = Inventory_AddItem(*remote, tmpl->items[i] & 0x1ff);
					Inventory_Equip(*remote, slot);
				}

				Owner_RefreshDerivedData(*remote);
				state->ppGauge = 0x4000;
				state->hpGauge = 0x4000;
				Party_AdvanceOwnerCountToTarget(*remote, tmpl->startingLevel);
				UnitCalc(*remote);
			}
			remote++;
		} while (*remote != -1);
	}
}

void Owner_LevelNoOp(void)
{
}

u32 Owner_GetLevelThreshold(s32 owner, s32 level)
{
	struct BattleUnit *state = UnitGet(owner);

	if (state->classIndex != 0)
	{
		if (level <= 0)
		{
			return 0;
		}
		if (level <= 99 && state->classId <= 7)
		{
			return gCharacter_LevelExpTable[state->classId * 99 + level - 1];
		}
	}
	return -1;
}

struct LevelUpResult *Owner_LevelUp(s32 owner, struct LevelUpResult *res)
{
	struct BattleUnit	   *st;
	struct LevelUpWork *work;
	u32					threshold;
	s16					band;
	s32					diff;
	s32					level;

	st = UnitGet(owner);
	work = Runtime_BumpAllocateAlternatePool(sizeof(struct LevelUpWork));
	work->classIndex = st->classIndex;
	level = st->level;
	work->level = level;
	res->level = level;
	res->ability = 0xffff;
	res->hp = 0;
	res->pp = 0;
	res->atk = 0;
	res->def = 0;
	res->agi = 0;
	res->luck = 0;
	if (level < 99)
	{
		st->level++;
		res->level = level + 1;
		threshold = Owner_GetLevelThreshold(owner, st->level);
		if (threshold != -1 && st->experience < threshold)
			st->experience = threshold;
		work->growth = Character_GetDefinition(owner);
		if (res->level == 1)
		{
			res->hp += work->growth->hp[0];
			res->pp += work->growth->pp[0];
			res->atk += work->growth->atk[0];
			res->def += work->growth->def[0];
			res->agi += work->growth->agi[0];
			res->luck += work->growth->luck[0];
		}
		band = res->level / 20;
		if (band < 0)
			band = 0;
		if (band > 4)
			band = 4;
		diff = work->growth->hp[band + 1] - work->growth->hp[band];
		res->hp += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->pp[band + 1] - work->growth->pp[band];
		res->pp += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->atk[band + 1] - work->growth->atk[band];
		res->atk += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->def[band + 1] - work->growth->def[band];
		res->def += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->agi[band + 1] - work->growth->agi[band];
		res->agi += ((Random16() * 20 >> 16) + diff) / 20;
		diff = work->growth->luck[band + 1] - work->growth->luck[band];
		res->luck += ((Random16() * 20 >> 16) + diff) / 20;
		st->baseHp += res->hp;
		st->basePp += res->pp;
		st->baseAttack += res->atk;
		st->baseDefense += res->def;
		st->baseAgility += res->agi;
		st->baseLuck += res->luck;
		st->baseActionCount = 1;
		st->baseHpRegen = 0;
		st->basePpRegen = 0;
		Owner_RefreshClassActions(owner);
		UnitCalc(owner);
	}
	MemFree(work);
	return res;
}

struct LevelUpResult *Owner_GetValueIfLevelThresholdReached(
	s32 owner_id, struct LevelUpResult *value)
{
	struct BattleUnit *owner;

	owner = UnitGet(owner_id);
	if ((owner->experience >= Owner_GetLevelThreshold(owner_id, owner->level + 1))
		&& (Owner_LevelUp(owner_id, value) != NULL))
	{
		return value;
	}
	return 0;
}

//--------------------------------------------------------------------------
// advance_owner_count_to_target.c
// パーティ：キャラクターを指定のレベルまで上げる
//--------------------------------------------------------------------------
// 目標のレベルになるまで 1 つずつレベルアップさせ、能力を計算しなおす
// unit : キャラクター番号
// lv   : 目標のレベル
//--------------------------------------------------------------------------
void Party_AdvanceOwnerCountToTarget(s32 unit, s32 lv)
{
	u8	res[16]; // struct LevelUpResult の受け取り（使わない）
	s32 cur = UnitGet(unit)->level;

	while (cur < lv)
	{
		Owner_LevelUp(unit, (struct LevelUpResult *)res);
		cur++;
	}
	UnitCalc(unit);
}

s32 GameFlag_Test(s32 flag)
{
	s32 flagMask;
	u8 *flagBytes;
	s32 flagValue;

	flagMask = 1 << (7 & flag);
	flag = ((u32)flag << 0x14) >> 0x17;
	flagBytes = gGameFlagBytes;
	flagValue = flagBytes[flag] & flagMask;
	return (u32)((0 - flagValue) | flagValue) >> 0x1F;
}

s32 GameFlag_SetBit(s32 flag)
{
	s32 flagMask;
	u8 *flagBytes;

	flagMask = 1 << (7 & flag);
	flagBytes = gGameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	flagBytes[flag] |= flagMask;
	return flag;
}

void GameFlag_ClearBit(s32 flag)
{
	s32 flagMask;
	u8 *flagBytes;

	flagMask = ~(1 << (7 & flag));
	flagBytes = gGameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	flagBytes[flag] &= flagMask;
}

u32 GameFlag_Toggle(s32 flag)
{
	s32 flagMask;
	u8 *flagBytes;
	s32 flagValue;

	flagMask = 1 << (7 & flag);
	flagBytes = gGameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	flagBytes[flag] ^= flagMask;
	flagValue = flagBytes[flag] & flagMask;
	return (u32)((0 - flagValue) | flagValue) >> 31;
}

s32 GameFlag_GetByte(s32 flag)
{
	u32 byteIndex;

	byteIndex = flag << 20;
	flag = byteIndex >> 23;
	return gGameFlagBytes[flag];
}

void GameFlag_SetByte(s32 flag, s32 value)
{
	u32 shifted = flag << 20;
	flag = shifted >> 23;
	gGameFlagBytes[flag] = value;
}

u8 GameFlag_IncrementByte(s32 flag)
{
	u8 *flagBytes;

	flagBytes = gGameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	if (flagBytes[flag] <= 0xFE)
	{
		flagBytes[flag]++;
	}
	return flagBytes[flag];
}

u8 GameFlag_DecrementByte(s32 flag)
{
	u8 *flagBytes;

	flagBytes = gGameFlagBytes;
	flag = ((u32)flag << 0x14) >> 0x17;
	if (flagBytes[flag] != 0)
	{
		flagBytes[flag] += 0xFF;
	}
	return flagBytes[flag];
}

s32 GameFlag_GetNibble(u32 flag)
{
	u32 shifted = flag << 20;
	u32 shift;
	u32 mask;
	s32 result;

	shift = flag & 4;
	mask = 15;
	flag = shifted >> 23;
	result = gGameFlagBytes[flag];
	result &= mask << shift;
	return result >> shift;
}

void GameFlag_SetNibble(s32 flag, s32 value)
{
	s32 fieldMask = 0xF;
	s32 shift = 4 & flag;
	s32 mask = fieldMask << shift;
	u8 *bytes = gGameFlagBytes;

	flag = ((u32)flag << 0x14) >> 0x17;
	bytes[flag] = (bytes[flag] & ~mask) | ((value & fieldMask) << shift);
}

#if EDITION_INTERNATIONAL
#define ENEMY_SUFFIX_FIRST '1'
#define ENEMY_SUFFIX_LAST 8
#else
#define ENEMY_SUFFIX_FIRST 'A'
#define ENEMY_SUFFIX_LAST 25
#endif

s32 BattleUnitSet(s32 unitId, s32 enemyId, s32 suffix)
{
	struct BattleUnit				 *unit;
	const struct EnemyDefinition *enemy;
	u16							  name[15];
	s32							  i;
	u32							  index;
	s32							  q;
	s32							  count;

	index = enemyId - 8;
	if (unitId <= 127)
		return 0;
	if (unitId > 134)
		return 0;
	if (index > 242)
		return 0;
	unit = UnitGet(unitId);
	Iwram_ClearWords(unit, BATTLE_UNIT_SIZE);
	if (index > 164)
		index = 0;
	enemy = &gEnemy_Definitions[index];
	unit->level = enemy->level;
	unit->baseHp = enemy->hp;
	unit->hp = enemy->hp;
	unit->maxHp = enemy->hp;
	unit->basePp = enemy->pp;
	unit->pp = enemy->pp;
	unit->maxPp = enemy->pp;
	unit->hpGauge = 0x4000;
	unit->ppGauge = 0x4000;
	unit->baseAttack = enemy->atk;
	unit->baseDefense = enemy->def;
	unit->baseAgility = enemy->agi;
	unit->baseLuck = enemy->luck;
	unit->baseActionCount = enemy->turns;
	unit->baseHpRegen = enemy->hpRegen;
	unit->basePpRegen = enemy->ppRegen;
	UiText_DecodeMessageFar(index + MsgEnemyName, name, 15);
	for (i = 0; i < 14 && name[i] != 0; i++)
		unit->name[i] = name[i];
	if (suffix <= ENEMY_SUFFIX_LAST)
	{
		unit->name[i] = ENEMY_SUFFIX_FIRST + suffix;
		i++;
	}
	unit->name[i] = 0;
	unit->name[14] = 0;
	count = 0;
	for (i = 0; i < 4; i++)
	{
		if (enemy->items[i] != 0)
		{
			for (q = 0; q < enemy->counts[i]; q++)
			{
				if (count < 15)
					unit->inventory[count++] = enemy->items[i];
			}
		}
	}
	unit->rewards = enemy->rewards;
	unit->classIndex = 0;
	unit->classId = enemyId;
	Owner_CalculateBaseElements(unitId, (s16(*)[2])unit->baseElement);
	UnitCalc(unitId);
	unit->side = 1;
	switch (unit->classId)
	{
	case 158:
	case 159:
	case 160:
	case 161:
	case 162:
	case 163:
	case 164:
	case 165:
	case 166:
	case 167:
	case 168:
	case 169:
	case 170:
	case 171:
		unit->side = 2;
		break;
	}
	return 1;
}

s32 Party_CountActiveOwners(void)
{
	s32 owner;
	s32 count;

	count = 0;
	owner = 0;
	do
	{
		if (GameFlag_Test(owner) != 0)
			count++;
		owner++;
	} while (owner <= 7);
	return count;
}

s32 Party_AddActiveOwner(s32 value)
{
	s32 count = Party_CountActiveOwners();
	s32 index;

	GameFlag_SetBit(value);
	index = 0;
	while (index < count)
	{
		if (gGameState.activeOwners[index] == value)
			return count;
		index++;
	}
	gGameState.activeOwners[index] = value;
	return count + 1;
}

s32 Party_RemoveActiveOwner(s32 value)
{
	s32 count = Party_CountActiveOwners();
	s32 i;
	s32 j;

	GameFlag_ClearBit(value);
	for (i = 0; i < count; i++)
	{
		if (gGameState.activeOwners[i] == value)
			break;
	}
	for (j = i; j < count - 1; j++)
		gGameState.activeOwners[j] = gGameState.activeOwners[j + 1];
	return Party_CountActiveOwners();
}

s32 Party_ListActiveOwners(s16 *owners)
{
	s32 count = 0;

	if (owners != NULL)
	{
		s32 index;

		count = Party_CountActiveOwners();
		index = 0;
		if (count != 0)
		{
			do
			{
				*owners++ = gGameState.activeOwners[index];
				index++;
			} while (index != count);
		}
		*owners = 0xff;
	}
	return count;
}

s32 Party_AdjustCoins(s32 amount)
{
	s32				  value;
	struct GameState *work;
	struct GameState *store;

	work = &gGameState;
	value = work->coins;
	value += amount;
	store = work;
	if (value > 0xF423F)
	{
		value = 0xF423F;
	}
	if (value < 0)
	{
		value = 0;
	}
	work = store;
	work->coins = value;
	return value;
}

struct TradeFlags
{
	u32 flags;
};

struct ElementLevelBonus
{
	u16 power;
	u16 res;
};

extern struct ElementLevelBonus gElement_PowerResistByLevel[16];

// gGameState の field に amount を足し、0 から max までにおさめて返す
#define DEFINE_PARTY_ADJUST(name, field, max)     \
	s32 name(s32 amount)                          \
	{                                             \
		s32 value;                                \
		struct GameState *progress = &gGameState; \
                                                  \
		value = progress->field;                  \
		value += amount;                          \
		if (value > max)                          \
			value = max;                          \
		if (value < 0)                            \
			value = 0;                            \
		progress->field = value;                  \
		return value;                             \
	}

DEFINE_PARTY_ADJUST(Party_AdjustSixDigitCounterB, shopCredit, 0xf423f)
DEFINE_PARTY_ADJUST(Party_AdjustCounterCappedAt28, shopGifts, 28)

s32 Trade_ListFlaggedEntries(u8 *output)
{
	u8 *source = gSummon_OrderList;
	u8 *end = gSummon_OrderList + 15;
	s32 count = 0;

	do
	{
		u8 value = *source++;
		if ((((struct TradeFlags *)Trade_GetOfferState(0))->flags & (1 << value)) != 0)
		{
			*output++ = value;
			count++;
		}
	} while (source <= end);
	*output = 32;
	return count;
}

const struct SummonDefinition *SummonDefinition_GetNear(u32 summonId)
{
	if (summonId > 15)
		return NULL;
	return &gSummon_DefinitionTable[summonId];
}

s32 Owner_LookupFourColumnTable(s32 row, s32 column)
{
	return gOwner_FourColumnValues[(row * 4) + column];
}

s32 Owner_GetDigitValues(s32 record, const u8 *source, s32 output[4])
{
	s32	 i;
	s32 *cursor;

	if (record > 7)
	{
		u32 index;

		index = ((const struct EnemyDefinition *)Owner_GetRecord(record))->elementPreset;
		if (index > 43)
			index = 0;

		i = 0;
		cursor = output;
		for (; i <= 3; i++)
			*cursor++ = gEnemy_ElementPresetTable[index].digits[i] * 10;
	}
	else
	{
		cursor = output;
		source += (u8 *)&((struct BattleUnit *)0)->djinnActiveCounts
				  - (u8 *)&((struct BattleUnit *)0)->djinnAvailable;
		for (i = 3; i >= 0; i--)
		{
			u32 value = *source;
			source++;
			*cursor++ = value * 10;
		}

		if (record <= 7)
		{
			for (i = 0; i <= 3; i++)
			{
				*output += Character_GetDefinition(record)->elementBonus[i];
				output++;
			}
		}
	}

	return 0;
}

s32 Owner_GetResistanceValue(s32 owner, s32 index)
{
	struct BattleUnit *state = UnitGet(owner);
	s32				values[4];
	s32				result = 0;

	if (index <= 3)
	{
		Owner_GetDigitValues(state->classId, (u8 *)state->djinnAvailable, values);
		result = values[index] / 10;
	}
	return result;
}

s32 Owner_GetDefaultElement(struct BattleUnit *state)
{
	const struct EnemyDefinition *record =
		(const struct EnemyDefinition *)Owner_GetRecord(state->classId);
	u8 value = record->elementPreset;

	if (value > 43)
		value = 0;
	return gEnemy_ElementPresetTable[value].first;
}

u32 Owner_CalculateBaseElements(s32 owner, s16 destination[4][2])
{
	struct BattleUnit *state = UnitGet(owner);
	u32				index;
	s32				values[4];
	s32				i;

	if (state->classIndex == 0)
	{
		index = ((const struct EnemyDefinition *)Owner_GetRecord(state->classId))->elementPreset;
		if (index > 43)
			index = 0;

		i = 0;
		for (;;)
		{
			((s32 *)destination)[i] = gEnemy_ElementPresetTable[index].values[i];
			i++;
			if (i > 3)
				return index;
		}
	}

	Owner_GetDigitValues(state->classId, (u8 *)state->djinnAvailable, values);
	for (i = 0; i < 4; i++)
	{
		s32 value;
		s32 ones;
		s32 tens;

		value = values[i];
		ones = value % 10;
		tens = value / 10;

		if (tens > 15)
			tens = 15;
		if (tens < 0)
			tens = 0;

		destination[i][0] = gElement_PowerResistByLevel[tens].power + ones;
		destination[i][1] = gElement_PowerResistByLevel[tens].res + ones;
	}
	// 戻り値なし（r0 には表のアドレスが残る）
}

s32 Owner_DetermineClass(s32 character, const u8 *djinn)
{
	s32 levels[4];
	s32 primary;
	s32 best;
	s32 series;
	s32 result;
	s32 j;
	s32 i;

	result = -1;
	if (character > 7)
		return 0;
	Owner_GetDigitValues(character, djinn, levels);
	if (GameFlag_Test(32))
	{
		if (character == 0)
			return 200;
		if (character == 1)
			return 201;
	}
	if (character == 5)
		return 202;
	if (result != -1)
		return result;
	best = result;
	primary = result;
	for (i = 0; i < 4; i++)
	{
		if (best < levels[i])
		{
			best = levels[i];
			primary = i;
		}
	}
	j = -1;
	best = -1;
	for (i = 0; i < 4; i++)
	{
		if (i != primary && best < levels[i])
		{
			best = levels[i];
			j = i;
		}
	}
	series = Owner_LookupFourColumnTable(primary, levels[j] > 9 ? j : primary);
	for (i = 202; i >= 0; i--)
	{
		if (gClass_DefinitionTable[i].series != series)
			continue;
		for (j = 0; j < 4; j++)
		{
			if (levels[j] < gClass_DefinitionTable[i].required[j] * 10)
				break;
		}
		if (j == 4)
		{
			result = i;
			break;
		}
	}
	if (result == -1)
		result = 0;
	return result;
}

struct ClassDefinition *Owner_GetRecordStride84(s32 arg0)
{
	return &gClass_DefinitionTable[arg0];
}

void Owner_RefreshDerivedData(s32 ownerId)
{
	struct BattleUnit *owner = UnitGet(ownerId);

	owner->classIndex = Owner_DetermineClass(owner->classId, (u8 *)owner->djinnAvailable);
	Owner_RefreshClassActions(ownerId);
	Owner_CalculateBaseElements(ownerId, (s16(*)[2])owner->baseElement);
}

s32 Curve_LookupScaledValue(s32 input, s32 halve)
{
	s32 min;
	s32 max;
	s32 n;
	s32 i;
	s32 value;
	s32 dx;
	s32 dy;

	max = gElement_PowerResistCurve[0][0];
	min = gElement_PowerResistCurve[4][0];
	n = 5;
	if (input > max)
		input = max;
	else if (input < min)
		input = min;
	for (i = 0; i < n; i++)
	{
		if (input > gElement_PowerResistCurve[i][0])
			break;
	}
	if (i == n)
	{
		value = gElement_PowerResistCurve[i - 1][1];
	}
	else
	{
		dx = gElement_PowerResistCurve[i - 1][0] - gElement_PowerResistCurve[i][0];
		dy = gElement_PowerResistCurve[i - 1][1] - gElement_PowerResistCurve[i][1];
		value = (input - gElement_PowerResistCurve[i][0]) * dy / dx + gElement_PowerResistCurve[i][1];
	}
	switch (halve)
	{
	case 0:
		break;
	case 1:
		value /= 2;
		break;
	}
	return value + 0x100;
}

//--------------------------------------------------------------------------
// random16.c
// バトル：乱数と、ダメージ・回復量の計算
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// バトル用の乱数（線形合同法）。種の 8-23 ビット目の 16 ビットを返す
//--------------------------------------------------------------------------
s32 BattleRandom(void)
{
	s32 seed;

	seed = (gBattleRandomSeed * 0x41c64e6d) + 0x3039;
	gBattleRandomSeed = seed;
	return (u32)(seed << 8) >> 16;
}

//--------------------------------------------------------------------------
// 0 から 99 までの乱数
//--------------------------------------------------------------------------
u32 BattleRandomPercent(void)
{
	return ((u32)BattleRandom() * 100) >> 16;
}

//--------------------------------------------------------------------------
// こうげきのダメージ（BattleAtkCalc）
// (atk - def + add * 2) / 2 に、属性の差による倍率をかける
// 倍率は 256 で 1 倍。差の効きは BattlePowerCalc の半分（128 から 384）
// atk   : こうげき
// def   : ぼうぎょ（atk - def が負なら 0 にする）
// add   : わざの威力
// bonus : 属性の差（力 - たいせい）
//--------------------------------------------------------------------------
s32 Owner_ScaleAdjustedAmount(s32 atk, s32 def, s32 add, s32 bonus)
{
	s32 dmg;
	s32 val;

	atk -= def;
	dmg = bonus;
	if (atk < 0)
		atk = 0;
	val = Curve_LookupScaledValue(dmg, 1) * (atk + (add * 2));
	if (val < 0)
		val += 0x1ff;
	dmg = val >> 9;
	if (dmg < 0)
		dmg = 0;
	return dmg;
}

//--------------------------------------------------------------------------
// エナジーの威力（BattlePowerCalc）
// power に属性の差による倍率（256 で 1 倍、0 から 512）と rate をかける
// power   : 威力
// bonus : 属性の差（力 - たいせい）
// rate  : 倍率（256 で 1 倍）
//--------------------------------------------------------------------------
s32 Owner_ScaleValueByCurve(s32 power, s32 bonus, s32 rate)
{
	s32 val;

	val = Curve_LookupScaledValue(bonus, 0) * power * rate;
	if (val < 0)
		val = val + 0xffff;
	return val >> 16;
}

//--------------------------------------------------------------------------
// かいふくの量（BattleRecoveryCalc）
// 属性の力が 100 をこえた分の 2 倍で倍率（256 で 1 倍）を引き、
// power と rate にかける
// power  : 威力
// element  : 行動側の属性の力（100 で 1 倍、200 以上で 2 倍、0 以下で 0）
// rate : 倍率（256 で 1 倍）
//--------------------------------------------------------------------------
s32 Owner_ScaleValueByOffsetCurve(s32 power, s32 element, s32 rate)
{
	s32 val;
	u32 prod;

	prod = Curve_LookupScaledValue(element * 2 - 200, 0) * power;
	prod = rate * prod;
	val = prod;
	if (val < 0)
		val = val + 0xffff;
	return val >> 16;
}

//--------------------------------------------------------------------------
// ユニットの攻撃の属性を返す（4 は属性なし）
// 敵は種類ごとの属性、味方は装備した武器の属性
// id : ユニット番号
//--------------------------------------------------------------------------
s32 Item_GetEquippedElement(s32 id)
{
	struct ItemDefinition *item;
	struct BattleUnit		  *unit;

	unit = UnitGet(id);
	if (unit->classIndex == 0)
		return Owner_GetDefaultElement(unit);
	item = Inventory_GetEquippedDefinition(unit, 1); // 1 : 武器
	if (item != NULL)
		return item->element;
	return 4;
}

s32 Equipment_GetUnleashRateBonus(struct BattleUnit *owner)
{
	s32					   sum;
	s32					   i;
	s32					   j;
	struct ItemDefinition *item;

	sum = 0;
	for (i = 0; i < 15; i++)
	{
		if (owner->inventory[i] & 0x200)
		{
			item = Item_GetDirect(owner->inventory[i]);
			for (j = 0; j < 4; j++)
			{
				if (item->effects[j].kind == 23)
					sum += item->effects[j].amount;
			}
		}
	}
	if (sum < 0)
		sum = 0;
	return sum;
}

s32 RollWeaponUnleash(struct BattleUnit *owner)
{
	struct ItemDefinition *item;
	s32					   rate;

	if (owner->classIndex == 0)
	{
		return 1;
	}
	item = Inventory_GetEquippedDefinition(owner, 1);
	if (item == NULL)
	{
		return 1;
	}
	if (item->descriptionMessage == 0)
	{
		return 1;
	}
	rate = ((Equipment_GetUnleashRateBonus(owner) + (item->secondaryFlags * 5)) << 0x10) / 100;
	if (rate > (BattleRandom() & 0xFFFF))
	{
		return item->descriptionMessage;
	}
	return 1;
}

s32 BattleFx_GetBaseSuccessRate(s32 effect_id)
{
	s32 battle_result;
	u32 entry_index;

	entry_index = effect_id - 8;
	switch (entry_index)
	{
	case 4:
	case 5:
		return 0x46;
	case 8:
	case 9:
		return 0x4B;
	case 14:
		return 0x1E;
	case 15:
		return 0x28;
	case 16:
		return 0x2D;
	case 10:
	case 11:
	case 17:
		return 0x37;
	case 18:
		return 0x19;
	case 19:
		return 0x14;
	case 12:
	case 23:
		return 0x41;
	case 13:
	case 26:
		return 0x23;
	case 27:
		return 0x32;
	case 48:
		battle_result = 0x3C;
		break;
	case 49:
		battle_result = 0x5A;
		break;
	case 0:
	case 1:
	case 20:
	case 24:
		return 0x3C;
	default:
		battle_result = 0x64;
		break;
	}
	return 0 - battle_result;
}

s32 BattleTarget_IsWeakToEffect(const struct BattleUnit *target, s32 effectId)
{
	const u8 *entries;
	s32		  i;

	if (target->classIndex == 0)
	{
		entries = Owner_GetRecord(target->classId) + 0x48;
		for (i = 0; i < 3; i++)
		{
			if (*entries == effectId)
				return 1;
			entries++;
		}
	}
	else
	{
		entries = Owner_GetRecordStride84(target->classIndex)->unk50;
		for (i = 0; i < 3; i++)
		{
			if (*entries++ == effectId)
				return 1;
		}
	}
	return 0;
}

s32 BattleEffectRevCheck(s32 effect_id)
{
	if ((effect_id == 5) || (effect_id == 0x38) || (effect_id == 0x39))
	{
		return 1;
	}
	return 0;
}

s32 BattleFx_RollSuccess(
	s32 caster, s32 target, s32 resistanceCategory, s32 effectId, s32 successScale)
{
	/* FAKEMATCH: three status reads (psySeal after the effect 4 test, confusion in it, haunt for effect 0x40) go through the byte view state[offset], so each builds its address from a fresh offset constant as the native does; read as unit fields, CSE reuses a neighbouring address. All eleven reads as fields differ 120 (184 instructions for 204); every other read is a field at no cost. */
	u8			   *state = (u8 *)UnitGet(target);
	struct BattleUnit *unit = (struct BattleUnit *)state;
	s32				attempts = 1;
	s32				score;
	s32				attempt;
	s8			   *poison;
	u8			   *delusion;
	u8			   *confusion;
	u8			   *charm;
	u8			   *stun;
	u8			   *sleep;

	if (BattleEffectRevCheck(effectId) != 0 && unit->hp != 0)
	{
		return 0;
	}

	if (effectId == 3 && unit->poison == 0)
	{
		return 0;
	}

	goto action4_check;
action4_tail:
	if (unit->stun == 0 && unit->sleep == 0 && state[(u32) & ((struct BattleUnit *)0)->psySeal] == 0
		&& unit->grimReaper == 0)
	{
		return 0;
	}
	goto action4_done;
action4_check:
	if (effectId == 4)
	{
		if (unit->delusion == 0 && state[(u32) & ((struct BattleUnit *)0)->confusion] == 0
			&& unit->charm == 0)
		{
			goto action4_tail;
		}
	}

action4_done:
	poison = &unit->poison;
	delusion = &unit->delusion;
	confusion = (u8 *)&unit->confusion;
	charm = &unit->charm;
	stun = &unit->stun;
	sleep = &unit->sleep;
	if (effectId == 0x40 && *poison == 0 && *delusion == 0 && *confusion == 0 && *charm == 0
		&& *stun == 0 && *sleep == 0 && unit->psySeal == 0 && unit->grimReaper == 0
		&& state[(u32) & ((struct BattleUnit *)0)->haunt] == 0)
	{
		return 0;
	}

	if (effectId == 0x1C && unit->grimReaper == 1)
	{
		return 0;
	}

	score = BattleFx_GetBaseSuccessRate(effectId);
	if (score > 0)
	{
		s32 difference = Owner_GetResistanceValue(caster, resistanceCategory)
						 - Owner_GetResistanceValue(target, resistanceCategory)
						 - (unit->luck >> 1);
		score += difference * 3;
		if (BattleTarget_IsWeakToEffect(unit, effectId) != 0)
		{
			score += 25;
		}
	}
	else
	{
		score = -score;
	}

	if (effectId == 0x43)
	{
		attempts = 3;
	}

	for (attempt = 0; attempt < attempts; attempt++)
	{
		if (score * successScale / 100 >= (s32)BattleRandomPercent())
		{
			return 1;
		}
	}
	return 0;
}

const u16 *Djinn_GetDefinition(u32 group, u32 index)
{
	s32 entry;

	entry = 0;
	if ((group <= 3U) && (index <= 0x13U))
	{
		entry = (group * 0x14) + index;
	}
	return (const u16 *)((u8 *)gDjinn_DefinitionTable + entry * 0xC);
}

s32 Djinn_AddToLeastLoadedOwner(s32 index, u8 *state)
{
	void *entry = state + index * 20 + 48;
	s32	  bestNo = 0;
	s32	  bestVal = 999;
	s32	  count;
	s32	  i;
	s32	  j;

	if (GameFlag_Test(entry) != 0)
		return -1;

	count = Party_CountActiveOwners();
	for (i = 0; i < count; i++)
	{
		struct BattleUnit *unit = UnitGet(gGameState.activeOwners[i]);

		if (unit->djinnOwnedCounts[index] <= 9)
		{
			s32 value = 0;

			for (j = 0; j < 4; j++)
				value += unit->djinnOwnedCounts[j];
			if (bestVal > value)
			{
				bestVal = value;
				bestNo = gGameState.activeOwners[i];
			}
		}
	}

	if (bestVal == 999)
		return -2;

	Djinn_AddToOwner(bestNo, index, (s32)state);
	Trade_AddOffer(bestNo, index, (u32)state);
	GameFlag_SetBit((u32)entry);
	return bestNo;
}

s32 Djinn_AddToOwner(s32 owner, s32 index, s32 bit)
{
	struct BattleUnit *state = UnitGet(owner);

	if (state->djinnOwnedCounts[index] > 9)
		return -1;
	if ((state->djinnAvailable[index] & (1 << bit)) != 0)
		return -1;
	state->djinnOwnedCounts[index]++;
	state->djinnAvailable[index] |= 1 << bit;
	return 0;
}

s32 Trade_CanOfferDjinn(s32 owner, s32 index, s32 bit)
{
	struct BattleUnit			 *state = UnitGet(owner);
	struct DjinnRecoveryList *table;
	s32						  i;
	s32						  status;

	if (state->djinnOwnedCounts[index] == 0)
		return 0;
	if (state->djinnActiveCounts[index] > 9)
	{
		state->djinnActiveCounts[index] = 10;
		return 0;
	}
	if ((state->djinnAvailable[index] & (1 << bit)) == 0)
		return 0;
	if ((state->djinnActive[index] & (1 << bit)) != 0)
		return 0;

	table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((owner < 0 || owner > 7)))->list;
	for (i = 0; i < table->count; i++)
	{
		if (index == table->entries[i].element && bit == table->entries[i].index)
			break;
	}
	if (i == table->count || ((status = table->entries[i].turns) <= 0 && status != -2))
		return 1;
	return 0;
}

u32 Djinn_IsActive(s32 owner, s32 index, s32 bit)
{
	s32 value = UnitGet(owner)->djinnActive[index] & (1 << bit);

	return (u32)(-value | value) >> 31;
}

s32 Djinn_Activate(s32 owner, s32 index, s32 bit)
{
	struct BattleUnit *state = UnitGet(owner);
	s32				result = Trade_CanOfferDjinn(owner, index, bit);

	if (result != 0)
	{
		if (state->djinnAvailable[index] & (1 << bit))
		{
			state->djinnActive[index] |= 1 << bit;
		}
		else
		{
			return 0;
		}
		state->djinnActiveCounts[index]++;
		Owner_RefreshDerivedData(owner);
	}
	return result;
}

u32 Djinn_Deactivate(s32 owner, s32 index, s32 bit)
{
	struct BattleUnit *state = UnitGet(owner);
	u32				present = Djinn_IsActive(owner, index, bit);

	if (present != 0)
	{
		state->djinnActiveCounts[index]--;
		state->djinnActive[index] &= ~(1 << bit);
		Owner_RefreshDerivedData(owner);
	}
	return present;
}

s32 Trade_RemoveOffer(s32 owner, s32 index, s32 bit)
{
	struct DjinnRecoveryList *table;
	s32						  found = 0;
	s32						  i;

	table = &((struct DjinnRecoveryTable *)Trade_GetOfferState((owner < 0 || owner > 7)))->list;
	for (i = 0; i < table->count; i++)
	{
		if (index == table->entries[i].element && bit == table->entries[i].index)
		{
			table->count--;
			found = 1;
			break;
		}
	}
	for (; i < table->count; i++)
	{
		table->entries[i] = table->entries[i + 1];
	}
	return found;
}

u32 *Trade_AddOffer(u32 kind, u32 first, u32 second)
{
	struct DjinnRecoveryList *table;
	s32						  count;

	Trade_RemoveOffer(kind, first, second);
	table = &((struct DjinnRecoveryTable *)Trade_GetOfferState(kind > 7))->list;
	count = table->count;
	table->entries[count].element = first;
	table->entries[count].index = second;
	table->entries[count].unitId = kind;
	table->entries[count].turns = -1;
	table->count = count + 1;
	return (u32 *)&table->count;
}

s32 Djinn_Transfer(s32 source, s32 index, s32 bit, s32 target)
{
	struct BattleUnit *state = UnitGet(source);
	u32				present;

	if ((state->djinnAvailable[index] & (1 << bit)) != 0)
	{
		present = Djinn_IsActive(source, index, bit);
		if (Djinn_AddToOwner(target, index, bit) == 0)
		{
			Djinn_Deactivate(source, index, bit);
			state->djinnAvailable[index] &= ~(1 << bit);
			state->djinnOwnedCounts[index]--;

			if (present != 0)
			{
				Djinn_Activate(target, index, bit);
			}
			else
			{
				Trade_RemoveOffer(source, index, bit);
				Trade_AddOffer(target, index, bit);
			}
			return 0;
		}
	}
	return -1;
}

s32 Trade_CountPendingOffers(u8 *counts)
{
	s32						   found = 0;
	struct DjinnRecoveryTable *base = Trade_GetOfferState(0);
	struct DjinnRecoveryEntry *entry = base->list.entries;
	s32						   index;

	if (counts != 0)
	{
		u8 *slot;

		slot = counts + 3;
		*slot = found;
		slot = counts + 2;
		*slot = found;
		slot = counts + 1;
		*slot = found;
		counts[0] = found;
	}
	index = 0;
	if (base->list.count != 0)
	{
		do
		{
			if (entry->turns == -1)
			{
				if (counts != 0)
					counts[entry->element]++;
				found++;
			}
			index++;
			entry++;
		} while (index != base->list.count);
	}
	return found;
}

s32 Djinn_GetDefinitionHeader(u32 group, u32 index)
{
	return *Djinn_GetDefinition(group, index);
}

s32 Party_SumDjinnCounts(s32 index)
{
	u16 owners[16];
	s32 result = 0;
	s32 count = Party_ListActiveOwners(owners);

	if (result < count)
	{
		u16 *owner = owners;
		s32	 remaining = count;

		do
		{
			struct BattleUnit *state = UnitGet(*owner++);

			if (index == -1)
			{
				result += state->djinnOwnedCounts[0];
				result += state->djinnOwnedCounts[1];
				result += state->djinnOwnedCounts[2];
				result += state->djinnOwnedCounts[3];
			}
			else
			{
				result += state->djinnOwnedCounts[index];
			}
			remaining--;
		} while (remaining != 0);
	}
	return result;
}

void Inventory_AddAndEquip(s32 owner, s32 target)
{
	struct BattleUnit *state = UnitGet(owner);
	u16			   *entry;
	s32				index;

	Inventory_AddItem(owner, target);
	index = 0;
	entry = state->inventory;
	do
	{
		if (*entry++ == target)
			Inventory_Equip(owner, index);
		index++;
	} while (index < INVENTORY_SLOTS);
}

#define INVENTORY_SNAPSHOT_SENTINEL 0x6774

void InventorySnapshot_Save(void)
{
	u16			   *save;
	struct BattleUnit *unit;
	s16			   *inventory;
	s32				owner;
	s32				i;
	s32				n;
	s16				first;
	s16				second;

	save = gInventorySnapshot;
	if (*save != INVENTORY_SNAPSHOT_SENTINEL)
	{
		*save++ = INVENTORY_SNAPSHOT_SENTINEL;
		first = gGameState.firstShortcut;
		second = gGameState.secondShortcut;
		for (owner = 0; owner < 4; owner++)
		{
			unit = UnitGet(owner);
			for (n = 0; n < 15; n++)
				*save++ = unit->inventory[n];
			for (n = 0; n < 15; n++)
			{
				if (Item_GetDirect(unit->inventory[n])->type != 6)
					unit->inventory[n] = 0;
			}
			n = 0;
			inventory = unit->inventory;
			for (i = 0; i < 15; i++)
			{
				if (inventory[i] != 0)
					inventory[n++] = inventory[i];
			}
			for (; n < 15; n++)
				inventory[n] = 0;
			Owner_RefreshDerivedData(owner);
			UnitCalc(owner);
		}
		*save++ = first;
		*save++ = second;
		inventory = (s16 *)gGameState.activeOwners;
		save[0] = inventory[0];
		save[1] = inventory[1];
		Inventory_AddAndEquip(0, 16);
		GameFlag_SetBit(0x952);
	}
	Owner_RefreshActiveRatios(1);
}

void InventorySnapshot_Restore(void)
{
	s32	 owner;
	u16 *source = gInventorySnapshot;

	if (*source++ == INVENTORY_SNAPSHOT_SENTINEL)
	{
		owner = 0;
		do
		{
			u16 *inventory = UnitGet(owner)->inventory;
			s32	 remaining = 14;

			do
			{
				*inventory++ = *source++;
				remaining--;
			} while (remaining >= 0);
			Owner_RefreshDerivedData(owner);
			UnitCalc(owner);
			owner++;
		} while (owner <= 3);

		gGameState.firstShortcut = *source++;
		gGameState.secondShortcut = *source++;
		*(u16 *)gGameState.activeOwners = *source++;
		*(u16 *)((u8 *)gGameState.activeOwners + 2) = *source;
		source = gInventorySnapshot;
		*source = 0;
		GameFlag_ClearBit(0x952);
	}
}
