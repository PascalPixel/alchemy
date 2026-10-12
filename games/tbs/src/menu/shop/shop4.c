#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/shop2.h"
#include "unit/menu/shop/shop3.h"
#include "unit/menu/shop/shop4.h"
#include "unit/menu/shop/shop5.h"
#include "unit/menu/shop/shop6.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "party_state.h"
#include "btlsys.h"
#include "object_runtime.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "shop.h"
#include "ui.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "keystate.h"
#include "node_chain.h"
#include "game_state.h"
#include "sound_ids.h"
#include "item.h"
#include "fixed_math.h"
#include "far_runtime.h"
#include "lib.h"
#include "fixed_point_position.h"
#include "inn_runtime.h"
#include "event_runtime.h"
#include "btlcalc.h"
#include "fieldobj.h"

s32 Shop_ServicePrice(s32 entry_no, s32 kind)
{
	u8	value = UnitGetFar(entry_no)->level;
	s32 result = 0;

	if (kind == 0)
	{
		result = value * 20;
	}
	else if (kind == 1)
	{
		result = 10;
	}
	else if (kind == 2)
	{
		result = 50;
	}
	else if (kind == 3)
	{
		result = value * 10;
	}
	return result;
}

s32 Shop_CanServe(s32 entry_no, s32 kind)
{
	struct BtlUnit *entry = UnitGetFar(entry_no);
	s32				result = 0;

	if ((kind == 0 && entry->hp <= 0) || (kind == 1 && entry->doku != 0)
		|| (kind == 2 && entry->akuryo != 0) || (kind == 3 && entry->restraint != 0))
	{
		result = 1;
	}
	return result;
}

s32 Shop_CountUnits(void)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					active = 0;
	s32					variant = shop->party_action;
	s32					index;

	for (index = 0; index < shop->party_member_count; index++)
	{
		if (Shop_CanServe(shop->party_member_ids[index], variant))
			active++;
	}
	return active;
}

s32 Shop_MsgByMode(s32 value)
{
	s8 mode = ((struct ShopRuntime *)gMenuWork)->party_action;

	if (mode == 1)
	{
		value += (u32)MsgCurePoisonService - (u32)MsgReviveService;
	}
	if (mode == 2)
	{
		value += (u32)MsgExorcismService - (u32)MsgReviveService;
	}
	if (mode == 3)
	{
		value += (u32)MsgRemoveCurseService - (u32)MsgReviveService;
	}
	return value;
}

void UiMessage_ShowResolvedAndWait(s32 value)
{
	s32 no;

	no = BattleFx_GetResourceIdFar(((struct ShopRuntime *)gMenuWork)->keeper_resource);
	UiWork_FinalizePendingCoreFar();
	value = Shop_MsgByMode(value);
	UiText_OpenMessageWindowFar(value, 5, 0, (no << 0x10) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
	{
		WaitFrames(1U);
	}
	WaitFrames(1U);
}

void UiMessage_ShowResolvedAndRestoreState(s32 arg0)
{
	struct ShopRuntime	 *state;
	struct RenderOutput **slot;
	s32					  value;
	u8					  saved;

	state = gMenuWork;
	slot = &state->cursor.anchor;
	saved = (*slot)->active;
	value = BattleFx_GetResourceIdFar(state->keeper_resource);
	arg0 = Shop_MsgByMode(arg0);
	(*slot)->active = 13;
	UiWork_FinalizePendingCoreFar();
	UiText_OpenMessageWindowFar(arg0, 5, 0, (value << 16) | 0x22);
	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);
	WaitFrames(1);
	state->cursor.anchor->active = saved;
}

s32 Shop_ConfirmAct(s32 unit_id)
{
	s32					 party_action = 0;
	s32					 list_window = 0;
	struct ShopRuntime	*shop;
	struct RenderOutput *cursor_anchor;

	Shop_InitializeCursorWork();
	shop = gMenuWork;
	shop->party_action = list_window;

	{
		s32 shown =
			(u16)((struct AnimationObject *)((struct ObjectRuntime *)Object_GetByIdFar(unit_id))
					  ->animation)
				->entries[0]
				->anim_id;
		shop->keeper_resource = shown;
	}

	list_window = UiWindow_CreateWithSideObjectFar(shop->keeper_resource, 0, 0, 0);
	if (list_window == 0)
	{
		list_window = UiWindow_CreateFar(-5, 0, 5, 5, 2);
	}
	if (list_window == 0)
	{
		list_window = UiWindow_CreateFar(0, 0, 5, 5, 2);
		SideObject_CreateFar(2, 0, 0, list_window, -4, -4);
	}

	cursor_anchor = RenderOutput_CreateFar(
		shop->cursor_icon, 0x40000000, (struct RenderInput *)list_window, 0, 0);
	cursor_anchor->active = 1;
	cursor_anchor->kind = 0;
	ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
	shop->cursor.anchor = cursor_anchor;
	UiMessage_ShowResolvedAndWait((s32)MsgSanctumWelcome);

#if EDITION_INTERNATIONAL
	shop->money_window = UiWindow_CreateFar(16, 11, 12, 4, 2);
#else
	shop->money_window = UiWindow_CreateFar(16, 11, 11, 4, 2);
#endif
	Shop_DrawMoney();

	for (;;)
	{
		party_action = Menu_SelectEntry19To1cFar(party_action);
		shop->party_action = party_action;
		if (party_action == -1)
			break;

		{
			s32 base = (s32)MsgReviveService;
			s32 message = base;

			base = 0;
			UiMessage_ShowResolvedAndWait(message);
			if (Shop_CountUnits() == 0)
			{
				UiMessage_ShowResolvedAndWait(message + 1);
			}
			else
			{
				Sanctum_RunPartyService();
			}
		}
		shop->party_action = 0;
		ShopCursor_SetPositionImmediate(&shop->cursor, -32, 112);
		UiMessage_ShowResolvedAndWait((s32)MsgSanctumMoreAid);
	}

	UiMessage_ShowResolvedAndWait((s32)MsgSanctumFarewell);
	UiWork_FinalizeFar(shop->money_window, 2);
	UiWork_FinalizeFar(list_window, 2);
	Inn_Cleanup();
	return 0;
}

s32 Sanctum_RunPartyService(void)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					price_window;
	s32					redraw;
	s32					kind;
	s32					list_window;
	s32					selection;
	s32					unit_id;
	s32					retry;
	s32					price;
	s32					message;

	price_window = 0;
	redraw = 1;
	kind = shop->party_action;
	UiMessage_ShowResolvedAndWait((s32)MsgWhoToRevive);

	list_window = UiWindow_CreateFar(1, 12, 13, 3, 2);
	shop->cursor.anchor->active = 4;
	shop->mode = redraw;
	PsynergyMenu_InitializeEntryObjectsFar(list_window, 2, 0, 8, price_window);
#if defined(TBS_EDITION_DE)
	price_window = UiWindow_CreateFar(0, 16, 30, 3, 2);
#elif defined(TBS_EDITION_FR)
	price_window = UiWindow_CreateFar(1, 16, 25, 3, 2);
#else
	price_window = UiWindow_CreateFar(1, 16, 23, 3, 2);
#endif

	selection = 0;
	unit_id = 0;
	retry = 0;
	while (selection < shop->party_member_count)
	{
		unit_id = shop->party_member_ids[selection];
		if (Shop_CanServe(unit_id, kind) != 0)
			break;
		selection++;
	}

	redraw = 1;
	for (;;)
	{
		if (retry != 0)
		{
			retry = 0;
			UiMessage_ShowResolvedAndWait((s32)MsgWhoToRevive);
			redraw = 1;
			selection = 0;
			while (selection < shop->party_member_count)
			{
				unit_id = shop->party_member_ids[selection];
				if (Shop_CanServe(unit_id, kind) != 0)
					break;
				selection++;
			}
		}

		if (redraw != 0)
		{
			redraw = 0;
			selection = (selection + shop->party_member_count) % shop->party_member_count;
			unit_id = shop->party_member_ids[selection];
			Shop_PlaceCursor((void *)list_window, selection * 24 - 12, 0);
			shop->mode = 3;
			Shop_HiliteUnit(list_window, selection);
			Shop_DrawSelMsg(price_window, unit_id);
		}

		if ((gKeyState & KEY_A) != 0)
		{
			WaitFrames(1);
			price = Shop_ServicePrice(unit_id, kind);
			if (Shop_CanServe(unit_id, kind) == 0)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				continue;
			}
			UiWork_PushValueSlotFar(unit_id, 1);
			UiWork_PushValueSlotFar(price, 5);
			message = (s32)MsgReviveDonation;
			UiMessage_ShowResolvedAndWait(message);
			if (UiMessage_ShowChoiceVariant(0) != 0)
			{
				UiMessage_ShowResolvedAndRestoreState(message + 2);
				retry = 1;
				continue;
			}
			if (price > (u32)gGameState.coins)
			{
				AudioCommand_PlayFar(SOUND_MENU_CANCEL);
				UiMessage_ShowResolvedAndRestoreState(message + 1);
				retry = 1;
				continue;
			}
			UiWork_PushValueSlotFar(unit_id, 1);
			UiMessage_ShowResolvedAndWait(message + 3);
			UiWork_FinalizePendingCoreFar();
			BattleUnit_ResetStateByMode(unit_id, kind);
			Shop_RunPartyMemberIconBurst(selection);
			Party_AdjustCoinsFar(-price);
			Shop_DrawMoney();
			UiWork_PushValueSlotFar(unit_id, 1);
			UiMessage_ShowResolvedAndWait(message + 4);
			if (Shop_CountUnits() != 0)
			{
				retry = 1;
				continue;
			}
			break;
		}
		if ((gKeyState & KEY_B) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CANCEL);
			break;
		}
		if ((gKeysRepeat & KEY_LEFT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			redraw = 1;
			selection -= 1;
		}
		if ((gKeysRepeat & KEY_RIGHT) != 0)
		{
			AudioCommand_PlayFar(SOUND_MENU_CURSOR_MOVE);
			redraw = 1;
			selection += 1;
		}
		WaitFrames(1);
		continue;
	}

	Menu_ReleaseEntryObjectsFar();
	UiWork_FinalizeFar(price_window, 2);
	UiWork_FinalizeFar(list_window, 2);
	WaitFrames(1);
	return 0;
}

void BattleUnit_ResetStateByMode(s32 id, s32 mode)
{
	struct BtlUnit *unit;

	unit = UnitGetFar(id);
	if (mode == 0)
	{
		unit->hp = unit->max_hp;
		UnitRatioFar(id);
	}
	else if (mode == 1)
	{
		unit->doku = 0;
	}
	else if (mode == 2)
	{
		unit->akuryo = 0;
	}
	else if (mode == 3)
	{
		s32 i;

		for (i = 0; i < 15; i++)
		{
			if (unit->inventory[i] & 0x200)
			{
				if (Item_GetDirectFar(unit->inventory[i])->flags & 1)
				{
					unit->inventory[i] ^= 0x200;
					UnitCalcFar(id);
				}
			}
		}
	}
}

void Shop_HiliteUnit(s32 enabled, s32 selected)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					index;
	s32					variant = shop->party_action;

	if (enabled != 0)
	{
		for (index = 0; index < shop->party_member_count; index++)
		{
			if (index == selected)
				AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 30);
			else
				AnimationObjects_SelectAnimationFar(shop->party_member_icons[index], 1);
			shop->party_member_scale[index] = 0x10000;
			if (!Shop_CanServe(shop->party_member_ids[index], variant))
				shop->party_member_scale[index] = 0xb333;
		}
	}
}

void Shop_DrawSelMsg(s32 target, s32 selection)
{
	s32 variant;
	s32 message;

	variant = ((struct ShopRuntime *)gMenuWork)->party_action;
	message = Shop_ServicePrice(selection, variant);
	if (target != 0)
	{
		RenderOutput_PrepareForRedrawFar(target);
		if (Shop_CanServe(selection, variant) != 0)
		{
			variant = (s32)MsgReviveCost;
		}
		else
		{
			variant = (s32)MsgNoHealingNeeded;
		}
		variant = Shop_MsgByMode(variant);
		UiWork_PushValueSlotFar(message, 5);
		UiText_DrawResourceFar(variant, target, 0, 0);
	}
}

void BattleFx_UpdateRadialMotion(struct EffectSlot *effect)
{
	struct FixedPointPosition position;
	s8						 *state_pointer;
	s32						  state;
	s32						  result;

	state_pointer = &effect->state;
	state = *state_pointer;
	if (state == 0)
	{
		position.x = effect->origin_x;
		position.z = effect->origin_z;
		Vector_AddPolarOffset(0x280000, Random16(), (s32 *)&position);
		EffectSlot_SetPositionFar(effect, position.x, position.z);
		position.x = effect->origin_x;
		position.z = effect->origin_z;
		Vector_AddPolarOffset(0x40000, Random16(), (s32 *)&position);
		effect->target_x = position.x;
		effect->target_z = position.z;
		effect->max_speed = 0x20000;
		effect->acceleration = 0x6666;
		effect->flag42 = state;
		*state_pointer = *state_pointer + 1;
	}
	else if (state == 1)
	{
		result = BattleFx_HasReachedTargetFar(effect);
		if (result == 0)
			*state_pointer = result;
	}
	else if (state == 2)
	{
		if (BattleFx_HasReachedTargetFar(effect) == 0)
			BattleFx_ClearOwnedSlotFar(effect);
	}
}

void Shop_ResetEffects(void)
{
	struct ShopRuntime *shop = gMenuWork;
	struct EffectSlot  *effect = shop->effects;
	s32					count = 23;
	s8					member;

	do
	{
		count--;
		EffectSlot_UpdateFar((s32)effect);
		effect++;
	} while (count >= 0);
	member = shop->burst_member;
	if (member != -1)
		ObjectGroup_SetChildValueUnlessFifteenFar(
			shop->party_member_icons[member], (Random16() * 7) >> 16);
}

void Shop_RunPartyMemberIconBurst(s32 member)
{

	struct ShopRuntime		 *shop;
	s8						  saved_kind;
	struct FixedPointPosition position;
	struct EffectSlot		 *effect;
	s32						  callback_flags;
	s32						  i;

	shop = gMenuWork;
	saved_kind = shop->cursor.anchor->active;
	shop->burst_member = 0xff;
	shop->cursor.anchor->active = 13;
	AudioCommand_PlayFar(Shop_PartyActionCues[shop->party_action]);
	Shop_RestoreSceneTiles(0x00202108);
	AnimationObjects_SetActiveTimerStepsFar(shop->party_member_icons[member], 0);
	WaitFrames(20);
	callback_flags = 0xc80;
	Scheduler_AddOrUpdateCallback(Shop_ResetEffects, callback_flags);

	position.x = shop->party_member_x[member] << 16;
	position.z = (shop->party_member_y[member] << 16) + 0xfff40000;

	i = 0;
	effect = shop->effects;
	do
	{
		EffectSlot_InitializeFar(effect, 0x11c, position.x, position.z);
		EffectSlot_SetCallbackFar(effect, BattleFx_UpdateRadialMotion);
		EffectSlot_SetObjectModeFar(effect, 7);
		ObjectGroup_SetChildValueUnlessFifteenFar(effect->object, (Random16() * 7) >> 16);
		effect->scale_y = 0xb333;
		effect->scale_x = 0xb333;
		WaitFrames(3);
		if (i == 5)
		{
			shop->burst_member = member;
		}
		i++;
		effect++;
	} while (i <= 17);

	AudioCommand_WaitForStateByteClear();
	{
		u8	active_mode = 2;
		u8 *entry = (u8 *)&shop->effects[0].state;
		for (i = 23; i >= 0; i--)
		{
			if (*(s8 *)(entry + 5) != 0)
			{
				*entry = active_mode;
			}
			entry += sizeof(struct EffectSlot);
		}
	}

	WaitFrames(20);
	AudioCommand_PlayFar(126);
	shop->burst_member = -1;
	ObjectGroup_SetChildValueUnlessFifteenFar(shop->party_member_icons[member], 0);
	WaitFrames(20);

	{
		u8				  *flag_entry = (u8 *)&shop->effects[0].active;
		struct EffectSlot *entry2 = shop->effects;
		for (i = 23; i >= 0; i--)
		{
			s32 flag = *flag_entry;

			flag <<= 24;

			flag_entry += 0x48;
			if (flag != 0)
			{
				BattleFx_ClearOwnedSlotFar(entry2);
			}
			entry2++;
		}
	}

	Scheduler_RemoveCallback(Shop_ResetEffects);
	AnimationObjects_SetActiveTimerStepsFar(shop->party_member_icons[member], 16);
	Shop_InitEffect();
	WaitFrames(30);
	shop->cursor.anchor->active = saved_kind;
}
