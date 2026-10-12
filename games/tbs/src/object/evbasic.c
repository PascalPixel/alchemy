#include "vram_tab.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evtable.h"
#include "unit/object/marker.h"
#include "unit/object/sparkle.h"
#include "unit/object/disptran.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/object/evmes.h"
#include "unit/object/evcamera.h"
#include "unit/object/djinnev.h"
#include "unit/object/psydisp.h"
#include "unit/object/psyfx.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/sched.h"
#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

// 手に入れたアイテムのメッセージ (ドイツ語版はブラックオーブだけ別の文)
static inline void ItemGift_ShowGotItem(s32 item, s32 mode)
{
#if defined(TBS_EDITION_DE)
	if (item == ITEM_BLACK_ORB)
	{
		UiText_ShowPositionedMessageAndWaitFar(MsgGotBlackOrb, mode);
		return;
	}
#endif
	UiWork_PushValueSlotFar(item, 2);
	UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem, mode);
}

// イベントのワーク (gEventWork) のこのファイルでの見え方。共通の EventWork に欄のない 0xcb6 以降も持つ
struct FieldWork
{
	u8				   unknown_000[0x10];
	s32				   view;
	struct FieldActor *actors[86];
	s16				   action;
	s16				   input;
	s16				   exit;
	s16				   menu;
	s16				   examine;
	s16				   start;
	u16				   descriptor;
	s16				   tile;
	s16				   scene;
	s16				   command;
	s16				   item;
	s16				   event;
	s16				   count;
	s16				   survived;
	s16				   quantities[9];
	s16				   item_argument;
	u8				   unknown_19c[2];
	s16				   mode;
	u8				   unknown_1a0[8];
	s32				   unknown_1a8;
	u8				   unknown_1ac[4];
	s32				   unknown_1b0;
	s32				   unknown_1b4;
	u8				   unknown_1b8[4];
	u8				  *cells;
	s32				   transition;
	u16				   unknown_1c4;
	u16				   transition_active;
	s32				   transition_frames;
	u8				   unknown_1cc[0xaea];
	u16				   busy;
	s16				   effect_active;	// 効果が動いていれば 0 以外
	s16				   effect_frames;	// 残りのフレーム
	s16				   effect_center_x; // 効果の中心 (ドット)
	s16				   effect_center_y;
	s16				   unknown_cc0;
	u8				   unknown_cc2[4];
	s8				   resolving_action; // エナジーの演出中
	s8				   unknown_cc7;
	s16				   queued_sound;
};

void Battle_UpdateModeFromShoulderButtons(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	if (gDebugMode != 0)
	{
		if (gKeyState & KEY_L)
		{
			runtime->mode_1cc = 0;
		}
		if (gKeyState & KEY_R)
		{
			runtime->mode_1cc = -1;
		}
	}
}

void Battle_WaitMode0(s32 frames)
{
	if (((struct BattleRuntime *)gEventWork)->mode_1cc == 0 && frames != 0)
	{
		WaitFrames(frames);
	}
}

void Battle_InitializeRenderObject(void)
{
	struct BattleRenderObject *object;

	object = ObjectTable_Get(gGameState.selected_actor);
	object->unknown_30 = 0x10000;
	object->unknown_34 = 0x8000;
	object->unknown_38 = 0x80000000;
	object->unknown_40 = 0x80000000;
	object->unknown_24 = 0;
	object->unknown_2c = 0;
	if (gGameState.movement_mode == 1)
	{
		ObjectDispatch_ApplyArgumentToChildrenFar(object, 0xC);
		return;
	}
	ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
}

void Battle_Reset(void)
{
	struct BattleRuntime *runtime = (struct BattleRuntime *)gEventWork;

	UiTimedNotice_CloseIfActiveFar();
	Battle_InitializeRenderObject();
	if (runtime->unknown_cb6 != 0)
	{
		Battle_ResetEffectCounter();
	}
	{
		s32 zero = 0;
		runtime->unknown_cc2 = zero;
		runtime->unknown_cc4 = zero;
		runtime->unknown_1c8 = 0x10;
		runtime->mode_1cc = zero;
		runtime->unknown_1da = 0xFFFF;
		runtime->unknown_1dc = -1;
		runtime->unknown_1de = -1;
		Scheduler_AddOrUpdateCallback((const void *)Battle_UpdateModeFromShoulderButtons, 0xC80);
		GameFlag_ClearBitFar(0x132);
		runtime->object_id = gGameState.selected_actor;
		runtime->unknown_1f8 = zero;
	}
}

void BattleFx_FinishAction(void)
{
	Scheduler_RemoveCallback(Battle_UpdateModeFromShoulderButtons);
	Object_AttachWorkTargetToObject(gGameState.selected_actor, 1);
	GameFlag_RefreshLureCapFar();
}

void Event_NoOpHookA(void)
{
}

void Event_NoOpHook(void)
{
}

void Event_RunObjectHookAndWait(struct ScenePlacement *table)
{
	Event_NoOpHook();
	ObjectTable_ResetForObject(table);
	WaitFrames(1);
	ObjectTable_Get(gGameState.selected_actor);
}

void Event_CallWithLastActiveObjectId(struct ScenePlacement *table)
{
	Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

void Event_SetSceneEvents(s32 value)
{
	struct FieldStepWork *work = (struct FieldStepWork *)gEventWork;

	work->events = (const struct SceneEvent *)value;
}

void Event_PrepareObjectAndApplyValue(s32 event_id, s32 value)
{
	UnitGetFar(event_id);
	Party_AddActiveOwnerFar(event_id);
	if (value != 0)
	{
		/* FAKEMATCH: Party_ShowJoinedMessageFar is called as s32 (*)(s32, s32); as void (*)(s32), its prototype, Event_PrepareObjectAndApplyValue does not compile */
		((s32 (*)(s32, s32))Party_ShowJoinedMessageFar)(event_id, value);
	}
}

void Event_PrepareTwoObjectsAndApply(s32 first, s32 second)
{
	Party_AddActiveOwnerFar(first);
	Party_AddActiveOwnerFar(second);
	Party_ShowPairJoinedMessageFar(first, second);
}

s32 Event_ValidatePackedId(u32 packed_id)
{
	s32 group = (packed_id >> 10) & 0xf;
	s32 entry = packed_id & 0x3ff;

	if (group > 7)
		return -1;
	if (GameFlag_TestFar(group) == 0)
		return -2;
	if (Equipment_HasValueFar(group, entry) == 0)
		return -3;
	return 0;
}

void Event_ClearInvalidPackedValues(void)
{
	if (Event_ValidatePackedId(gGameState.first_shortcut) != 0)
		gGameState.first_shortcut = 0;
	if (Event_ValidatePackedId(gGameState.second_shortcut) != 0)
		gGameState.second_shortcut = 0;
}

void Party_RemoveOwnerRestored(s32 owner)
{
	struct BtlUnit *vitals;
	s32				i;
	s32				count;
	s32				standing;

	Party_RemoveActiveOwnerFar(owner);
	Event_ClearInvalidPackedValues();
	vitals = UnitGetFar(owner);
	Unit_Recover(vitals);
	vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
	if (vitals->hp_gauge == 0 && vitals->hp != 0)
		vitals->hp_gauge = 1;
	vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
	if (vitals->ep_gauge == 0 && vitals->ep != 0)
		vitals->ep_gauge = 1;
	vitals->doku = 0;
	standing = 0;
	count = Party_CountActiveOwnersFar();
	for (i = 0; i < count; i++)
	{
		vitals = UnitGetFar(gGameState.active_owners[i]);
		if (vitals->hp != 0)
			standing++;
	}
	if (standing == 0)
	{
		vitals = UnitGetFar(gGameState.selected_actor);
		vitals->hp = 1;
		vitals->hp_gauge = Vitals_Ratio(vitals->hp, vitals->max_hp);
		if (vitals->hp_gauge == 0 && vitals->hp != 0)
			vitals->hp_gauge = 1;
		vitals->ep_gauge = Vitals_Ratio(vitals->ep, vitals->max_ep);
		if (vitals->ep_gauge == 0 && vitals->ep != 0)
			vitals->ep_gauge = 1;
	}
}

s32 Party_CheckMemberValueTotal(s32 id)
{
	s32 count;
	s32 sum = 0;
	s32 i;

	count = Party_CountActiveOwnersFar();
	for (i = 0; i < count; i++)
		sum += Inventory_CountItemFar(gGameState.active_owners[i], id);

	if (sum >= count * 30)
	{
		UiWork_PushValueSlotFar(id, 2);
		UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore, 1);
		UiWork_PushValueSlotFar(id, 2);
		UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryMore + 1, 1);
		return -1;
	}
	return 0;
}

s32 PartyInventory_GiveItem(s32 item, s32 unused)
{
	struct EventRuntime *work;
	s32					 saved;
	s32					 owner;
	s32					 result;
	s32					 count;
	s32					 i;
	s32					 member;
	s32					 slot;

	work = (struct EventRuntime *)gEventWork;
	saved = work->message;
	owner = PartyInventory_AddFar(item);
	if (owner == -1)
	{
		ItemGift_ShowGotItem(item, 1);
		UiText_ShowPositionedMessageAndWaitFar(MsgCannotCarryAnyMore, 1);
	retry:
		UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop, 1);
		result = Shop_PickUnitItemFar(&member, &slot);
		if (result == -1)
		{
			if (Item_GetDirectFar(item)->flags & ITEM_INDISPENSABLE)
			{
				UiWork_PushValueSlotFar(item, 2);
				UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 4, 1);
				goto retry;
			}
			UiWork_PushValueSlotFar(item, 2);
			UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 1, 5);
			result = Object_CallSpawnRoutineAtOrigin(1);
			UiWork_FinalizePendingCoreFar();
			if (result != 0)
				goto retry;
			Item_AdjustCounterFar(item, 1);
			UiWork_PushValueSlotFar(item, 2);
			UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 2, 1);
			work->message = saved;
			return owner;
		}
		else
		{
			UnitGetFar(member);
			{
				/* FAKEMATCH: the ROM loads the slot argument before the member. Only the English build, without the thirty-item check below, needs the copy; the direct call matches the rest. */
				s32 s = slot;

				count = Shop_GetSelectionState(member, s);
			}
#if !defined(TBS_EDITION_EN)
			if (Inventory_CountItemFar(member, item) > 29)
			{
				UiWork_PushValueSlotFar(member, 1);
				UiWork_PushValueSlotFar(item, 2);
				UiText_ShowPositionedMessageAndWaitFar(MsgWhatWillYouDrop + 7, 1);
				goto retry;
			}
#endif
			for (i = 0; i < count; i++)
				Inventory_DiscardFar(member, slot);
			owner = PartyInventory_AddFar(item);
			AudioCommand_PlayFar(83);
			if (owner == gGameState.selected_actor)
			{
				ItemGift_ShowGotItem(item, 3);
			}
			else
			{
				UiWork_PushValueSlotFar(item, 2);
				UiWork_PushValueSlotFar(owner, 1);
				UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 3);
			}
			work->message = saved;
			return owner;
		}
	}
	else
	{
		AudioCommand_PlayFar(83);
		ItemGift_ShowGotItem(item, 3);
		if (owner != gGameState.selected_actor)
		{
			UiWork_PushValueSlotFar(item, 2);
			UiWork_PushValueSlotFar(owner, 1);
#if defined(TBS_EDITION_DE)
			UiText_ShowPositionedMessageAndWaitFar(MsgGaveItemToMember, 1);
#elif defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
			UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 1);
#else
			UiText_ShowPositionedMessageAndWaitFar(MsgKorosseoRobinGotItem + 1, 3);
#endif
		}
		work->message = saved;
	}
	return owner;
}

//--------------------------------------------------------------------------
// try_add_and_return_owner.c
// 持ち物：アイテムを持たせて、持ち主を返す
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// アイテムを持たせる
// 持てればキャラクター番号を、持ち物がいっぱいなら -1 を返す
// item   : アイテム番号
// unused : 使わない
// owner  : 持たせるキャラクター番号
//--------------------------------------------------------------------------
s32 Inventory_TryAddAndReturnOwner(s32 item, s32 unused, s32 owner)
{
	if (Inventory_AddItemFar(owner, item) >= 0)
		return owner;
	return -1;
}

//--------------------------------------------------------------------------
// 何もしない
//--------------------------------------------------------------------------
void Inventory_NoOpCallback(void)
{
}

struct EventPromptWork
{
	struct EventRuntime event;
	u8					unknown_200[0xac2];
	s16					resource_base;
	s16					width;
};

void Inventory_ReservedNoOp(void)
{
}

void Object_WaitUntilChildValueDiffers(s32 object_id, s32 value)
{
	struct ObjectRuntime *obj = ObjectTable_Get(object_id);

	if (obj != 0 && obj->animation_kind == 1)
	{
		s32 cnt = 0;
		u8 *p = &((struct AnimationObject *)obj->animation)->last_no;

		while (cnt <= 89)
		{
			WaitFrames(1);
			if (value != *p)
			{
				break;
			}
			cnt++;
		}
	}
}

s32 Inventory_PromptAndSetObjectMode(s32 actor, s32 force_bottom)
{
	struct EventPromptWork *work = (struct EventPromptWork *)gEventWork;
	s32						sprite =
		((struct ScenePlacement *)BattleAction_FindDescriptor(work->event.speaker))->sprite;
	struct UiWindow *window = (struct UiWindow *)work->event.message_window;
	struct UiWindow *side = (struct UiWindow *)work->event.side_window;
	s32				 use_bottom = 1;
	s32				 answer;

	while (gKeyState != 0)
		WaitFrames(1);

	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);

	WaitFrames(3);

	if (force_bottom == 0)
	{
		s32 bottom = window->y + window->height;

		if (side != 0)
		{
			s32 side_bottom = side->y + side->height;
			if (bottom < side_bottom)
				bottom = side_bottom;
		}

		if (bottom > 15)
			use_bottom = 0;
	}

	answer = Menu_RunConfirmSelectionFar(use_bottom, work->resource_base, work->width, 0);
	if (answer != 0)
	{
		Object_SetModeById(actor, 4);
		UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
		UiWork_FinalizePendingCoreFar();
		Object_WaitUntilChildValueDiffers(actor, 4);
	}
	else
	{
		Object_SetModeById(actor, 3);
		UiWork_FinalizeEntityMatchingLocalizedIdFar(sprite);
		UiWork_FinalizePendingCoreFar();
		Object_WaitUntilChildValueDiffers(actor, 3);
	}

	return answer;
}

s32 Object_CallSpawnRoutineAtOrigin(s32 value)
{
	return Menu_RunConfirmSelectionFar(value, 0, 0, 0);
}

//--------------------------------------------------------------------------
// draw_draw_quantity_pair_with_cue.c
// メッセージ：わざを覚えさせて「覚えた」と出す
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// わざを覚えさせ、効果音とメッセージを出して、送られるまで待つ
// unit : キャラクター番号（メッセージの名前 1）
// abil : わざの番号（メッセージのわざ 4）
//--------------------------------------------------------------------------
void UiText_DrawQuantityPairWithCue(s32 unit, s32 abil)
{
	// 引数なしの宣言だが、r0・r1 の unit・abil がそのまま渡る
	/* FAKEMATCH: OwnerAction_AddFar is called as void (*)(void); as s32 (*)(s32, s32), its prototype, UiText_DrawQuantityPairWithCue does not compile */
	((void (*)(void))OwnerAction_AddFar)();
	AudioCommand_PlayFar(83);
	UiWork_PushValueSlotFar(unit, 1);
	UiWork_PushValueSlotFar(abil, 4);
	UiText_ShowPositionedMessageAndWaitFar(MsgAbilityLearned, 3);
}

void Event_StartScreenTransition(void)
{
	struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

	DisplayTransition_Start(runtime->value_1c0, runtime->value_1c8);
	runtime->status_1c6 = 1;
}

void Event_FinishScreenTransition(void)
{
	struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;

	DisplayTransition_Finish(runtime->value_1c0, runtime->value_1c8);
	runtime->status_1c6 = 0;
}

void Event_WaitTransitionFrames(void)
{
	WaitFrames(((struct EventRuntime *)gEventWork)->value_1c8);
}

void Event_SetDestination(s32 first, s32 second)
{
	((struct EventRuntime *)gEventWork)->value_170 = 999;
	gGameState.scene = first;
	gGameState.entrance = second;
}

void Event_SetSavedDestination(u16 first, u16 second)
{
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;
	state->value_170 = 999;
	gGameState.saved_scene = first;
	gGameState.saved_entrance = second;
}

void Event_SetExitRequest(u16 value)
{
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;
	state->value_170 = value;
}

//--------------------------------------------------------------------------
// result_request.c
// バトル効果：イベントから決まった敵との戦闘を用意する
//--------------------------------------------------------------------------

#define CHOJO 98	  // 頂上の嵐の背景で戦うエンカウント表
#define TOBANAI 0x800 // ジンを跳ねさせない
#define JIN_0 300	  // ジン 0 番の pending_djinn_event

//--------------------------------------------------------------------------
// エンカウント表 zone の slot 番目の敵グループとの戦闘を用意する
// ワールドマップ（mode 3）では、選択中の者の足もとの地形も引く
//--------------------------------------------------------------------------
void BattleFx_SetWeightedResult(s32 zone_no, s32 slot_no)
{
	register s32				  zone = zone_no;
	register s32				  slot = slot_no;
	register struct EventRuntime *ev = (struct EventRuntime *)gEventWork;

	ev->value_17c = BattleFx_GetWeightedResult(zone, slot); // 敵グループ
	if (zone == CHOJO && slot == 0)
		gGameState.special = (s32)&ResourceId_VinasuChojoStormBackdrop;
	if (ev->mode_19e == 3)
		BattleFx_LookupResult((u8 *)ObjectTable_Get(gGameState.selected_actor) + 8); // 位置
	BattleFx_SelectBattleCue(zone, slot);
}

//--------------------------------------------------------------------------
// ジン no 番との戦闘を用意する
// flag の下 8 ビットはジンのオブジェクトで、TOBANAI がなければ跳ねさせる
// no は属性ごとに 20 ずつ（Djinn_ResolvePendingEvent）
//--------------------------------------------------------------------------
void BattleFx_SetPhaseRequest(s32 flag, s32 no)
{
	struct EventRuntime *ev;
	struct GameState	*gs;
	s32					 skip;

	ev = (struct EventRuntime *)gEventWork;
	skip = flag & TOBANAI;
	flag &= 0xff;

	if (skip == 0)
		EventActor_RunHopAndLeapSequence(flag);

	gs = &gGameState;
	gs->pending_djinn_event = (no + JIN_0) | skip;
	ev->value_17c = BattleFx_GetPhaseResult(no); // ジンの敵グループ
	if (ev->mode_19e == 3)
	{
		void *obj = ObjectTable_Get(gs->selected_actor);

		BattleFx_LookupResult((u8 *)obj + 8); // 位置
	}
	BattleFx_SelectBattleCue(0, 0);
}

void Party_SetFields1ceAnd1d0(s32 first, s32 second)
{
	gGameState.next_scene = first;
	gGameState.next_entrance = second;
}

void Event_SetDefeatDestination(s32 first, s32 second)
{
	gGameState.defeat_scene = first;
	gGameState.defeat_entrance = second;
}

void Event_RequestDestination(u16 first, u16 second)
{
	struct EventRuntime *work = (struct EventRuntime *)gEventWork;
	gGameState.scene = first;
	gGameState.entrance = second;
	work->value_170 = 999;
}

void BattleFx_SetQueuedSoundAndPlay(s32 sound_id)
{
	((struct FieldWork *)gEventWork)->queued_sound = sound_id;
	if ((s16)sound_id == -1)
	{
		sound_id = 0x121;
	}
	AudioCommand_PlayFar(0x12a);
	AudioCommand_PlayFar(sound_id);
}

void BattleFx_PlayQueuedSound(void)
{
	s16 sound_id = ((struct FieldWork *)gEventWork)->queued_sound;

	if (sound_id != -1)
		AudioCommand_PlayFar(sound_id);
}
