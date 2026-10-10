#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "btlpty.h"
#include "battle_command.h"
#include "btlsys.h"
#include "fixed_math.h"
#include "battle_efx.h"
#include "btltype.h"
#include "btlsmn.h"
#include "btlwork.h"
#include "motobj.h"
#include "character.h"
#include "animspr.h"
#include "callback_scheduler.h"
#include "battle_status_icon.h"
#include "scene.h"
#include "io_reg.h"
#include "io_write_queue.h"
#include "resource.h"
#include "ui.h"
#include "menu_list.h"
#include "objdisp.h"
#include "item.h"
#include "global_cells.h"
#include "btlev.h"

struct SlotArray
{
	s16 items[64];
};

s32 BattleTarget_SelectRandomPosition(s32 require_living_unit)
{

	u16				  positions[6];
	struct SlotArray *order;
	s16				 *entry;
	u16				 *cursor;
	s32				  value;
	s32				  count;
	s32				  index;
	s32				  slot;
	s32				  tail;
	s32				  offset;

	count = 0;
	order = (struct SlotArray *)BtlWk;

	if (require_living_unit != 0)
	{
		for (;;)
		{
			index = 0;
			slot = (u32) & ((struct BtlWork *)0)->party_units / sizeof(s16);
			if (order->items[slot] != 255)
			{
				entry = order->items;
				do
				{
					value = entry[slot];
					if (value != 254)
					{
						if (UnitGetFar(value)->hp != 0)
						{
							positions[count] = index | 0x100;
							count++;
						}
					}
					slot++;
					index++;
				} while (entry[slot] != 255);
			}
			goto pick;
		}
	}
	else
	{
		index = 0;
		slot = (u32) & ((struct BtlWork *)0)->enm_no / sizeof(s16) - 1;
		tail = (u32) & ((struct BtlWork *)0)->enm_no / sizeof(s16) - 1;
		offset = tail * 2;
		entry = (s16 *)(order->items + 1);
		if (*(s16 *)((char *)entry + offset) != 255)
		{
			cursor = (u16 *)entry;
			do
			{
				if ((s16)cursor[slot] != 254)
				{
					positions[count] = index | 0x180;
					count++;
				}
				slot++;
				tail++;
				index++;
			} while (entry[tail] != 255);
		}
	}

pick:
	if (count == 0)
		return 0;
	return positions[(Random16() * count) >> 16];
}

#define COUNT_PARTIAL_CURES(unit, count) \
	{                                    \
		if ((unit)->maboroshi != 0)      \
			(count)++;                   \
		if ((u8)(unit)->konran != 0)     \
			(count)++;                   \
		if ((unit)->charm != 0)          \
			(count)++;                   \
		if ((unit)->nemuri != 0)         \
			(count)++;                   \
		if ((unit)->fuuji != 0)          \
			(count)++;                   \
		if ((unit)->shiryo != 0)         \
			(count)++;                   \
	}

#define COUNT_ALL_CURES(unit, count)          \
	{                                         \
		COUNT_PARTIAL_CURES((unit), (count)); \
		if ((unit)->akuryo != 0)              \
			(count)++;                        \
		if ((unit)->doku != 0)                \
			(count)++;                        \
	}

#define COUNT_POSITIVE_MODIFIERS(unit, count) \
	{                                         \
		if ((unit)->atk_lv > 0)               \
			(count)++;                        \
		if ((unit)->def_lv > 0)               \
			(count)++;                        \
		if ((unit)->res_lv > 0)               \
			(count)++;                        \
		if ((unit)->elm_lv[0] > 0)            \
			(count)++;                        \
		if ((unit)->elm_lv[1] > 0)            \
			(count)++;                        \
		if ((unit)->elm_lv[2] > 0)            \
			(count)++;                        \
		if ((unit)->elm_lv[3] > 0)            \
			(count)++;                        \
	}

#define CHECK_EFFECT_TARGET(unit, action, applies, damage_class)    \
	{                                                               \
		(applies) = 0;                                              \
		switch ((action)->efx)                                      \
		{                                                           \
		case EFX_ATK_UP2:                                           \
		case EFX_ATK_UP1:                                           \
			if ((unit)->atk_lv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->atk_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_ATK_DOWN2:                                         \
		case EFX_ATK_DOWN1:                                         \
			if ((unit)->atk_lv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->atk_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_DEF_UP2:                                           \
		case EFX_DEF_UP1:                                           \
			if ((unit)->def_lv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->def_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_DEF_DOWN2:                                         \
		case EFX_DEF_DOWN1:                                         \
			if ((unit)->def_lv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->def_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_RES_UP2:                                           \
		case EFX_RES_UP1:                                           \
			if ((unit)->res_lv + 1 <= 4)                            \
				(applies) = 1;                                      \
			if ((unit)->res_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_RES_DOWN2:                                         \
		case EFX_RES_DOWN1:                                         \
			if ((unit)->res_lv - 1 >= -4)                           \
				(applies) = 1;                                      \
			if ((unit)->res_cnt == 1)                               \
				(applies)++;                                        \
			break;                                                  \
		case EFX_CURE_POISON:                                       \
			if ((unit)->doku != 0)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_CURE_PART:                                         \
			COUNT_PARTIAL_CURES((unit), (applies));                 \
			break;                                                  \
		case EFX_HEAL_60:                                           \
		case EFX_HEAL_30:                                           \
			if ((unit)->hp < (unit)->max_hp)                        \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_BUFF_CLEAR:                                        \
			COUNT_POSITIVE_MODIFIERS((unit), (applies));            \
			break;                                                  \
		case EFX_CURE_ALL:                                          \
			COUNT_ALL_CURES((unit), (applies));                     \
			break;                                                  \
		case EFX_DEATH_CURSE:                                       \
			if ((unit)->shiryo == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_POISON:                                            \
			if ((unit)->doku == 0)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_VENOM:                                             \
			if ((unit)->doku <= 1)                                  \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_STUN:                                              \
			if ((unit)->shibire == 0)                               \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_SLEEP:                                             \
			if ((unit)->nemuri == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_EVIL_SPIRIT:                                       \
			if ((unit)->akuryo == 0)                                \
				(applies) = 1;                                      \
			break;                                                  \
		case EFX_REVIVE_FULL:                                       \
		case EFX_REVIVE_HALF:                                       \
		case EFX_REVIVE_80:                                         \
			if ((unit)->hp == 0)                                    \
				(applies) = 100;                                    \
			break;                                                  \
		case 0:                                                     \
		case 1:                                                     \
		case 2:                                                     \
			break;                                                  \
		default:                                                    \
			(applies) = 1;                                          \
			break;                                                  \
		}                                                           \
		if ((unit)->hp == 0 && BtlEfxRevChkFar((action)->efx) == 0) \
			(applies) = 0;                                          \
		if ((applies) == 0)                                         \
		{                                                           \
			(damage_class) = ((action)->tgt_flg & 0x0f) - 1;        \
			switch (damage_class)                                   \
			{                                                       \
			case 2:                                                 \
			case 3:                                                 \
				if ((unit)->hp != 0)                                \
					(applies)++;                                    \
				break;                                              \
			case 7:                                                 \
			case 8:                                                 \
				if ((action)->pow != 0 && (unit)->hp != 0)          \
					(applies)++;                                    \
				break;                                              \
			case 9:                                                 \
				if ((unit)->ep != 0)                                \
					(applies)++;                                    \
				break;                                              \
			case 0:                                                 \
				if ((unit)->hp != 0 && (unit)->hp < (unit)->max_hp) \
					(applies)++;                                    \
				break;                                              \
			case 1:                                                 \
			case 4:                                                 \
			case 5:                                                 \
				if ((action)->pow != 0 && (unit)->hp != 0)          \
					(applies)++;                                    \
				break;                                              \
			}                                                       \
		}                                                           \
	}

s32 BattleTarget_SelectForAction(s32 actor_id, struct BtlAct *action)
{
	u16				order_positions[6];
	s32				unit_ids[6];
	s32				target_positions[6];
	struct BtlWork *turn_order;
	struct BtlUnit *unit;
	struct BtlUnit *next_unit;
	struct BtlUnit *this_unit;
	s32				candidate_count;
	s32				target_index;
	s32				target_count;
	s32				inner_index;
	s32				selected;
	s32				unit_id;
	s32				value;
	s32				next_value;
	s32				temp;
	s32				opposing;
	s32				damage_class;
	s32				applies;
	u32				roll;
	s16			   *slot;

	turn_order = BtlWk;
	target_count = 0;
	candidate_count = 0;

	if (action->target_mode != 0)
	{
		opposing = 0;
		if (action->target_mode == 2 || action->target_mode == 4)
			opposing = 1;

		if (actor_id < 0 || actor_id > 7)
		{
			if (opposing != 0)
				goto scan_mirrored_order;
		}
		else if (opposing == 0)
		{
			goto scan_mirrored_order;
		}

		target_index = 0;
		while (turn_order->party_units[target_index] != 255)
		{
			slot = &turn_order->party_units[target_index];
			unit_id = *slot;
			if (unit_id != 254)
			{
				if (action->target_mode != 4 || unit_id == actor_id)
				{
					unit_ids[candidate_count] = unit_id;
					order_positions[candidate_count] = 0x100;
					order_positions[candidate_count] |= target_index;
					candidate_count++;
				}
			}
			target_index++;
		}
		goto scan_complete;

	scan_mirrored_order:
		target_index = 0;
		while (turn_order->enm_no[target_index] != 255)
		{
			unit_id = turn_order->enm_no[target_index];
			if (unit_id != 254)
			{
				if (action->target_mode != 4 || unit_id == actor_id)
				{
					unit_ids[candidate_count] = unit_id;
					order_positions[candidate_count] = 0x180;
					order_positions[candidate_count] |= target_index;
					candidate_count++;
				}
			}
			target_index++;
		}
	}

scan_complete:

	if (candidate_count == 0)
		return -2;

	for (target_index = 0; target_index < candidate_count; target_index++)
	{
		unit = UnitGetFar(unit_ids[target_index]);
		CHECK_EFFECT_TARGET(unit, action, applies, damage_class);
		if (applies != 0)
		{
			unit_ids[target_count] = unit_ids[target_index];
			target_positions[target_count] = order_positions[target_index];
			target_count++;
		}
	}

	if (target_count == 0)
		return -1;

	if (action->target_mode == 1 && action->rng == 1
		&& Owner_GetRecordFar(UnitGetFar(actor_id)->class_id)->target_strategy != 2
		&& (u32)((action->tgt_flg & 0x0f) - 3) <= 2)
	{
		selected = -1;
		for (target_index = 0; target_index < target_count; target_index++)
		{
			for (inner_index = target_index; inner_index < target_count - 1; inner_index++)
			{
				this_unit = UnitGetFar(unit_ids[inner_index]);
				next_unit = UnitGetFar(unit_ids[inner_index + 1]);
				if (Owner_GetRecordFar(UnitGetFar(actor_id)->class_id)->target_strategy == 0)
				{
					value = this_unit->hp;
					next_value = next_unit->hp;
				}
				else
				{
					value = this_unit->max_hp;
					next_value = next_unit->max_hp;
				}
				if (value < next_value)
				{
					temp = unit_ids[inner_index];
					unit_ids[inner_index] = unit_ids[inner_index + 1];
					unit_ids[inner_index + 1] = temp;
					temp = target_positions[inner_index];
					target_positions[inner_index] = target_positions[inner_index + 1];
					target_positions[inner_index + 1] = temp;
				}
			}
		}

		switch (target_count)
		{
		case 1:
			selected = 0;
			break;
		case 2:
			roll = (11 * Random16()) >> 16;
			selected = 0;
			if (roll > 5)
				selected = 1;
			break;
		case 3:
			selected = (15 * Random16()) >> 16;
			if (selected <= 5)
				selected = 0;
			else if (selected <= 10)
				selected = 1;
			else
				selected = 2;
			break;
		case 4:
			selected = (18 * Random16()) >> 16;
			if (selected <= 5)
				selected = 0;
			else if (selected <= 10)
				selected = 1;
			else if (selected <= 14)
				selected = 2;
			else
				selected = 3;
			break;
		}

		if (selected >= 0)
			return target_positions[selected];
	}
	return target_positions[(Random16() * target_count) >> 16];
}

//--------------------------------------------------------------------------

// actor_reset_runtime_fields.c

// バトル：状態変化の消去と、メッセージ送りの待ち

//--------------------------------------------------------------------------

#define BtlObjGet ((int (*)())BtlObjGet)

//--------------------------------------------------------------------------
// 状態異常と能力の変化をすべて消す
// 戦闘不能になったときに呼ばれる
//--------------------------------------------------------------------------
s32 BattleActor_ResetRuntimeFields(s32 id)
{
	struct BtlUnit *unit;
	s8			   *lv;
	s32				n;
	u8				zero;

	unit = UnitGetFar(id);
	n = 3;
	zero = 0;
	lv = &unit->elm_lv[3];
	do
	{
		n--;
		*lv-- = zero;
	} while (n >= 0);

	unit->doku = 0;
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
	unit->akuryo = 0;
	unit->shiryo = 0;
	unit->unknown_142[0] = 0;
	unit->unknown_142[1] = 0;
	unit->sonae = 0;
	unit->miugoki = 0;
	unit->agi_cnt = 0;
	unit->agi_lv = 0;
	unit->chouhatsu = 0;

	UnitCalcFar(id);
	return BattleUnit_BuildStatusFlags(id, BtlObjGet(id));
}

//--------------------------------------------------------------------------
// メッセージ送りの矢印
//--------------------------------------------------------------------------
#define KEY_OKURI (KEY_A | KEY_B | KEYS_SHOULDERS) // 送りのキー

// OAM の一枚
struct AdvanceSprite
{
	struct AdvanceSprite *next;
	u16					  attr0;
	u16					  attr1;
	u16					  attr2;
	u16					  attr3;
};

extern volatile u32 gFrameCount;
void AudioCommand_PlayFar(s32 cue);

//--------------------------------------------------------------------------
// ウィンドウの位置に矢印を出し、送りのキーを待つ
// B を押し続けるか、送りのキーを押すと進む
// 16 フレームを過ぎれば、押し続けていても進む
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForAdvance(void)
{
	struct AdvanceSprite	   *spr;
	struct AdvanceSprite		sprite;
	s32							frame;
	s32							slot;
	s32							pat;
	struct UiWindow			   *window;
	struct BattleDisplayOffset *offset;
	s32							attr2;
	s32							attr1;
	s32							x;
	s16							tile;

	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	frame = 0;
loop:
	// 4 フレームごとに 8 枚のパターンを回す
	pat = ((gFrameCount >> 2) & 7) * 128 + (s32)BattlePres_AdvanceArrowTiles;
	window = gBattleDisplayWork->window;
	offset = gBattleDisplayWork->offset;
	QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
	QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
	/* FAKEMATCH: needed to match: the one-pass loop keeps the BLDALPHA address load after the value */
	do
	{
		u16 blend = 16;
		REG_BLDALPHA = blend;
	} while (0);
	*(u32 *)&spr->attr0 = 0xa400;
	*(u32 *)&spr->attr2 = 0;
	tile = Resource_GetBuffer(slot, pat) & 0x3ff;
	attr2 = spr->attr2;
	spr->attr2 = (attr2 & ~0x3ff) | tile;
	// ウィンドウの位置はキャラ単位、オフセットは 1/256 ドット
	x = (u16)((window->x * 8 + (offset->x >> 8) + 4) & 0x1ff);
	attr1 = spr->attr1;
	spr->attr1 = (attr1 & ~0x1ff) | x;
	// 上下に 2 ドットまでゆれる
	*(u8 *)&spr->attr0 = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
	Runtime_PushSlotEntry(spr, 240);
	if (!(gKeysHeld & KEY_B) && !(gKeyState & KEY_OKURI)
		&& (frame <= 15 || !(gKeysHeld & KEY_OKURI)))
	{
		WaitFrames(1);
		frame++;
		goto loop;
	}
	AudioCommand_PlayFar(111);
	Resource_ResetEntry(slot);
	WaitFrames(1);
}

//--------------------------------------------------------------------------
// 指定の位置でカーソルを動かし、送りのキーを待つ
// x, y : ドット
//--------------------------------------------------------------------------
s32 BattlePresentation_WaitForPromptAt(s32 x, s32 y)
{
	struct MenuPoint	 pos;
	s32					 pat;
	struct CursorSprite	 sprite;
	struct CursorSprite *spr;
	s32					 slot;

	pat = Ui_GetTableWordZeroFar(0);
	pos.x = x;
	pos.y = y;
	while (!UiWork_IsCompleteFar())
		WaitFrames(1);
	spr = &sprite;
	slot = Resource_LoadIntoFreeSlot(128);
	while (1)
	{
		QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
		QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
		/* FAKEMATCH: needed to match: the one-pass loop keeps the BLDALPHA address load after the value */
		do
		{
			u16 blend = 16;
			REG_BLDALPHA = blend;
		} while (0);
		((s32 *)spr)[1] = MENU_SPRITE_SIZE_16;
		((s32 *)spr)[2] = 0;
		spr->tile = Resource_GetBuffer(slot, pat);
		// 4 フレームごとに右上へゆらす
		spr->x = pos.x + ((gFrameCount & 4) >> 1) - 4;
		spr->y = pos.y - ((gFrameCount & 4) >> 2) - 8;
		Runtime_PushSlotEntry(spr, 240);
		if (gKeyState & KEY_OKURI)
			break;
		WaitFrames(1);
	}
	Resource_ResetEntry(slot);
	WaitFrames(1);
}

#undef BtlObjGet

#define BattlePresentation_WaitForAdvance ((int (*)())BattlePresentation_WaitForAdvance)

void BattleEv_MarkDisplay(void)
{
	gBattleDisplayWork->marked = 1;
}

void		   ObjectDispatch_ReleaseFar(struct DispatchObject *object);
struct BtlObj *BtlObjGet(s32 arg0);
s32			   ActivateBattleObjectSlot(s32 arg0);

s32 BattleActor_DestroyTemporaryObject(s32 arg0)
{
	/* FAKEMATCH: needed to match: the shared discard-only result type preserves TBS epilogue register choice */
	struct BtlUnit *creature;
	struct BtlObj  *runtime;

	creature = UnitGetFar(arg0);
	if (creature->side == 1)
	{
		Owner_UpdateRatioPairFar(creature, 0);
		BattleActor_RemoveFromLists(arg0);
		ActivateBattleObjectSlot(arg0);
		runtime = BtlObjGet(arg0);
		ObjectDispatch_ReleaseFar((struct DispatchObject *)runtime->object);
		runtime->object = 0;
		runtime->active = 0;
	}
}

void Battle_SetRuntimeFlagBit0(struct BattleEventState *runtime, u32 operand)
{
	runtime->flags |= 1;
}

void UiWork_PushValueSlotFar(u32, u32);
void BattleMotion_RunValueSequence();

u32 BattleEv_DispatchQueued(void)
{
	struct BattleEventState *runtime = &BtlWk->events;
	struct BattleEventQueue *queue = &runtime->queue;
	s32						 i;

	for (i = 0; i < queue->count; i++)
	{
		u8 opcode = queue->opcodes[i];
		if (opcode <= BEV_SCR)
			switch (opcode)
			{
			case BEV_SCR:
				Battle_SetRuntimeFlagBit0(runtime, queue->operands[i]);
				break;
			case BEV_OBJ_EFX:
				BattleActor_DestroyTemporaryObject(queue->operands[i]);
				break;
			case BEV_UNIT:
				UiWork_PushValueSlotFar(queue->operands[i], 1);
				break;
			case BEV_NUM:
				UiWork_PushValueSlotFar(queue->operands[i], 5);
				break;
			case BEV_ITEM:
				UiWork_PushValueSlotFar(queue->operands[i] & ITEM_ID_MASK, 2);
				break;
			case BEV_ACT:
				UiWork_PushValueSlotFar(queue->operands[i] & OWNER_ACTION_ID_MASK, 4);
				break;
			case BEV_MARK:
				gBattleDisplayWork->marked = 1;
				break;
			case BEV_CLR:
				UiWork_ClearValueNameTablesFar();
				break;
			case BEV_MES:
				if ((s32)queue->operands[i] >= 0)
					UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
				BattlePresentation_WaitForAdvance();
				UiWork_ClearValueNameTablesFar();
				break;
			case BEV_MES_ADD:
				if ((s32)queue->operands[i] >= 0)
					UiText_ShowMessageAndWaitCoreFar(queue->operands[i]);
				UiWork_ClearValueNameTablesFar();
				break;
			case BEV_OBJ_BGN:
				if (runtime->pending_cue > 0)
					AudioCommand_PlayFar(runtime->pending_cue);
				BattleMotion_RunValueSequence(queue->operands[i], 0, 0);
				break;
			case BEV_OBJ_RES:
				BattleEnemy_RecordDefeat(queue->operands[i], runtime->flags);
				BattleActor_ResetRuntimeFields(queue->operands[i]);
				BattleMotion_InitializeActorRecords(queue->operands[i]);
				break;
			case BEV_REDRAW:
				PtyStDrawFar(BtlWk->pty_st);
				break;
			case BEV_OBJ_END:
				BattleUnit_BuildStatusFlags(queue->operands[i], BtlObjGet(queue->operands[i]));
				BattlePres_SetActorModeAndAction(queue->operands[i]);
				break;
			}
	}
	return BattleEventRuntime_Reset();
}

u32 BtlEvPush(u32 opcode, u32 operand)
{
	struct BattleEventQueue *queue = &BtlWk->events.queue;
	u32						 index = queue->count;

	queue->opcodes[index] = opcode;
	queue->operands[index] = operand;
	queue->count = index + 1;
	return opcode;
}

#undef BattlePresentation_WaitForAdvance
