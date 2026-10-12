#include "unit/menu/item/menu.h"
#include "unit/menu/item/menu2.h"
#include "unit/menu/item/menu3.h"
#include "unit/menu/item/menu4.h"
#include "unit/menu/item/menu5.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime7.h"
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
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"
#include "uiwindow.h"
#include "mem.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "btlsys.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "fixed_math.h"
#include "lib.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "field_sprite.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"
#include "keystate.h"
#include "window.h"
#include "menu_result.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "sound_ids.h"
#include "iwram_call.h"
#include "heap_state.h"
#include "menu_list.h"
#include "irq.h"
#include "object_factory.h"
#include "far_runtime.h"
#include "equipment_menu.h"
#include "owner_state.h"
#include "inventory.h"
#include "layout_guard.h"
#include "btltype.h"
#include "btlcalc.h"
#include "eventwrk.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "battle_efx.h"
#include "four_object_motion.h"

struct InventoryMenuState;

struct BtlUnit;

void Item_PrepareUsePalette(void)
{
	Palette_CopyObjectBankToBackground14();
	Palette_LightenBankHighlight(13);
}

void Item_UseNoOpCallback(void)
{
}

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
	struct InventoryMenuState *work;
	s32						   result;
	s32						   item_id;
	struct BtlUnit			  *owner;
	struct ItemDefinition	  *item;

	owner = UnitGetFar(owner_id);
	work = gMenuWork;
	item_id = ITEM_ID_MASK & owner->inventory[slot];
	item = Item_GetDirectFar(item_id);
	result =
		BattleEffect_ApplyToTargets(OWNER_ACTION_ID_MASK & item->action_id, owner_id, target_id, 1);
	if (result != -1)
	{
		item = Item_GetDirectFar(owner->inventory[slot]);
		if (item->use_type == 1)
		{
			Inventory_RemoveFar(owner_id, slot);
			work->item_count = ItemMenu_Collect(owner, work->items, 0);
		}
		if (item->use_type == 4)
		{
			if (item_id == 0xb8)
				item_id = 0xb9;
			owner->inventory[slot] = item_id;
		}
		result = 0;
	}
	return result;
}

s32 Item_ReturnOne(void)
{
	return 1;
}

s32 BattleEffect_ApplyToTargets(s32 effect_id, s32 source_id, s32 target_id, s32 fixed_scale)
{
	struct BtlAct			  *effect;
	struct InventoryMenuState *runtime;
	struct BtlUnit			  *target;
	struct BtlUnit			  *source;
	s32						   later_target;
	s32						   changed;
	u8						   index;
	s16						   scale;
	s32						   random_adjust;
	s32						   random_nonone;
	s32						   amount;
	s32						   result_code;

	effect = BtlActGet(effect_id);
	runtime = gMenuWork;
	changed = 0;
	result_code = 0;
	later_target = 0;

	if (target_id != 9)
		target = UnitGetFar(target_id);
	else
		target = UnitGetFar(0);

	/* FAKEMATCH: the dead test keeps the range load before the count guard and preserves target-index and power-load registers. */
	if (effect->rng == 0xff && later_target != 0)
		result_code = 3;

	for (index = 0; index < runtime->party_count; index++)
	{
		if (effect->rng == 0xff)
		{
			target_id = runtime->owner_ids[index];
			target = UnitGetFar(target_id);
		}

		amount = effect->pow;
		switch (effect->tgt_flg & 0xf)
		{
		case BATTLE_DAMAGE_HP_HEAL:
			if (fixed_scale == 0)
			{
				if (effect->damage_class != 4)
				{
					s32 stat_offset;

					source = UnitGetFar(source_id);
					stat_offset = effect->damage_class;
					scale = source->elm[stat_offset].pow;
				}
				else
					scale = 100;
				amount = BtlRcvCalc(amount, scale, 0x100);
			}

			if (target->hp <= 0)
			{
				if (later_target == 0)
					result_code = 2;
			}
			else
			{
				if (target->hp == target->max_hp)
				{
					if (later_target == 0)
						result_code = 4;
				}
				else
				{
					target->hp += amount;
					if (target->hp > target->max_hp)
					{
						amount -= target->hp - target->max_hp;
						target->hp = target->max_hp;
						if (later_target == 0)
							result_code = 0;
					}
					else if (later_target == 0)
					{
						result_code = 1;
					}
					UnitRatioFar(target_id);
					changed = 1;
					if (effect->rng == 0xff)
					{
						later_target = 1;
						result_code = 3;
					}
				}
			}
			break;

		case 9:
			random_adjust = (Random16() * 4) >> 16;
			if (random_adjust == 0)
			{
				random_adjust = -1;
			}
			else
			{
				random_nonone = 1 ^ random_adjust;
				random_adjust = (u32)((-random_nonone) | random_nonone) >> 31;
				random_adjust = 1 - random_adjust;
			}

			switch (effect_id & OWNER_ACTION_ID_MASK)
			{
			case 0x104:
				target->base_hp += amount + random_adjust;
				result_code = 0x10;
				changed = 1;
				break;
			case 0x105:
				target->base_ep += amount + random_adjust;
				result_code = 0x11;
				changed = 1;
				break;
			case 0x108:
				target->base_agility += amount + random_adjust;
				result_code = 0x12;
				changed = 1;
				break;
			case 0x109:
				target->base_luck += amount;
				result_code = 0x13;
				changed = 1;
				break;
			case 0x106:
				target->base_attack += amount + random_adjust;
				UiWork_PushValueSlotFar(3, 5);
				result_code = 0x14;
				changed = 1;
				break;
			case 0x107:
				target->base_defense += amount + random_adjust;
				UiWork_PushValueSlotFar(4, 5);
				result_code = 0x15;
				changed = 1;
				break;
			}
			break;

		case BATTLE_DAMAGE_PP_HEAL:
			if (target->ep == target->max_ep)
			{
				if (later_target == 0)
					result_code = 7;
			}
			else
			{
				target->ep += amount;
				if (target->ep > target->max_ep)
				{
					amount -= target->ep - target->max_ep;
					target->ep = target->max_ep;
					if (later_target == 0)
						result_code = 5;
				}
				else if (later_target == 0)
				{
					result_code = 6;
				}
				UnitRatioFar(target_id);
				changed = 1;
				if (effect->rng == 0xff)
				{
					later_target = 1;
					result_code = 8;
				}
			}
			break;

		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 10:
			break;
		}

		switch (effect->efx)
		{
		case 1:
			if (target->hp <= 0 || target->hp == target->max_hp)
			{
				if (later_target == 0)
					result_code = 2;
			}
			else
			{
				target->hp += amount;
				if (target->hp > target->max_hp)
				{
					target->hp = target->max_hp;
					if (later_target == 0)
						result_code = 0;
				}
				else if (later_target == 0)
				{
					result_code = 1;
				}
				UnitRatioFar(target_id);
				changed = 1;
			}
			break;

		case 2:
			if (target->ep == target->max_ep)
			{
				if (later_target == 0)
					result_code = 7;
			}
			else
			{
				target->ep += amount;
				if (target->ep > target->max_ep)
				{
					target->ep = target->max_ep;
					if (later_target == 0)
						result_code = 5;
				}
				else if (later_target == 0)
				{
					result_code = 6;
				}
				UnitRatioFar(target_id);
				changed = 1;
			}
			break;

		case EFX_REVIVE_FULL:
			if (target->hp == 0)
			{
				target->hp = target->max_hp;
				UnitRatioFar(target_id);
				changed = 1;
				if (later_target == 0)
					result_code = 0xc;
			}
			else if (later_target == 0)
			{
				result_code = 0xd;
			}
			break;

		case EFX_REVIVE_HALF:
			if (target->hp == 0)
			{
				target->hp = target->max_hp / 2;
				UnitRatioFar(target_id);
				if (later_target == 0)
					result_code = 0xc;
			}
			else if (later_target == 0)
			{
				result_code = 0xd;
			}
			break;

		case EFX_REVIVE_80:
			if (target->hp == 0)
			{
				target->hp = target->max_hp * 7 / 10;
				UnitRatioFar(target_id);
				if (later_target == 0)
					result_code = 0xc;
			}
			else if (later_target == 0)
			{
				result_code = 0xd;
			}
			break;

		case EFX_CURE_POISON:
			if (target->doku != 0)
			{
				target->doku = 0;
				changed = 1;
				if (later_target == 0)
					result_code = 0xa;
			}
			else if (later_target == 0)
			{
				result_code = 0xb;
			}
			break;
		}

		if (effect->rng != 0xff)
			break;
	}

	if (changed == 0)
	{
		runtime->message_offset = result_code;
		return -1;
	}

	for (index = 0; index < runtime->party_count; index++)
		UnitCalcFar(runtime->owner_ids[index]);
	runtime->message_offset = result_code;
	return 0;
}

void Item_PlayUseAnimation(u32 item)
{
	Ability_PlayUseAnimation(Item_GetDirectFar(item)->action_id & OWNER_ACTION_ID_MASK);
}

void Ability_PlayUseAnimation(s32 action)
{
	s32			   animation_type;
	u32			   target_type;
	struct BtlAct *ability;

	ability = BtlActGet(action);
	animation_type = 0xf & ability->tgt_flg;
	switch (animation_type)
	{
	case 1:
		Audio_PlayCueReturnOne(0x7e);

	case 11:
		Audio_PlayCueReturnOne(0x7e);
		return;
	default:
		target_type = ability->efx - 1;
		switch (target_type)
		{
		case 4:
			Audio_PlayCueReturnOne(0x52);
			return;
		case 2:
			Audio_PlayCueReturnOne(0x54);
			return;
		default:
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
			Audio_PlayCueReturnOne(0x5b);

		case 0:
		case 1:
		case 30:
		case 31:
			return;
		}
		break;
	}
}
