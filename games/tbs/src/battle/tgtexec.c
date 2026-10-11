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
#include "btlcalc.h"
#include "btlmes.h"
#include "btlobj.h"
#include "btldisp.h"
#include "iwram_call.h"
#include "mem.h"

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
	// 戻り値の型は s32 だが return 文は無い（pop {r1}; bx r1）
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

//--------------------------------------------------------------------------

// tgtexec.c

// バトル：対象ごとの効果処理

//--------------------------------------------------------------------------

#undef BtlObjGet

//--------------------------------------------------------------------------
// 対象が倒れていても追加効果を出す効果なら 1 を返す
// うばう効果は、与えたダメージを行動側に戻すため
// efx : 効果番号
//--------------------------------------------------------------------------
s32 BtlEfxDeadChk(s32 efx)
{
	switch (efx)
	{
	case 31: // HPをうばう
	case 32: // EPをうばう
	case 60: // HPをうばう（半分）
	case 69: // EPをうばう（1/10）
		return 1;
	default:
		return 0;
	}
}

//--------------------------------------------------------------------------
// 対象の判定・効果計算・メッセージ登録
//--------------------------------------------------------------------------
#define MIKATA 2 // 味方

#define MIKATA_CHK(id) ((id) >= 0 && (id) < 8)

#define STEP_LIM(v)   \
	{                 \
		if ((v) < -4) \
			(v) = -4; \
		if ((v) > 4)  \
			(v) = 4;  \
	}

#define ELM_BONUS()                             \
	{                                           \
		if (elm != 4)                           \
			bonus = pow - target->elm[elm].res; \
	}

#define MES_PUSH(m) BtlEvPush(BEV_MES, (u32)(m))

#define MES_SIDE(m, e)       \
	{                        \
		if (MIKATA_CHK(tid)) \
			MES_PUSH(m);     \
		else                 \
			MES_PUSH(e);     \
	}

#define GUARD_CUT()               \
	{                             \
		guard = target->guard_lv; \
		if (guard != 0)           \
		{                         \
			if (guard == 1)       \
				dmg /= 2;         \
			else                  \
				dmg /= 10;        \
		}                         \
	}

#define RES_STEP(d, v, m)         \
	{                             \
		target->res_lv += (d);    \
		STEP_LIM(target->res_lv); \
		BtlEvPush(BEV_NUM, (v));  \
		MES_PUSH(m);              \
		target->res_cnt = 7;      \
	}

#define STAT_SET(f, m) \
	{                  \
		MES_PUSH(m);   \
		(f) = 7;       \
	}

//--------------------------------------------------------------------------
// わざの効果を対象ひとりにかける
// plan : 行動と対象の情報
// slot : 対象配列の位置
//--------------------------------------------------------------------------
s32 BtlTgtExec(struct BtlPlan *plan, s32 slot)
{
	s32				dist;
	struct BtlAct  *action;
	struct BtlUnit *actor;
	s32				aid;
	s32				act_no;
	s32				bonus;
	struct BtlWork *work;
	s32				half;
	s32				adj;
	s32				dealt;
	s32				crush;
	s32				hit;
	s32				crit;
	s32				skip;
	s32				type;
	s32				aff;
	struct BtlUnit *bak;
	s32				pow;
	s32				hp;
	s32				rate;
	s16			   *cmd;
	s32				tid;
	s32				elm;
	struct BtlUnit *target;
	u32				n;
	s32				value;
	s32				dmg;
	s32				scale;
	s32				pass;
	s32				guard;
	s16				list[8];
	s32				cnt;
	s32				tmp;
	s32				size;

	// 変化量の計算用に、対象の状態を保存する
	bonus = 0;
	work = BtlWk;
	half = 0;
	dealt = 0;
	crush = 0;
	skip = 0;
	aff = 0;
	size = sizeof(struct BtlUnit);
	bak = MemAlloc(size);

	aid = plan->aid;
	tid = plan->tgt_no[slot];
	act_no = plan->act_no;
	elm = plan->elm;
	adj = plan->tgt_adj[slot];
	crit = plan->tgt_crit[slot];

	action = BtlActGet(act_no);
	actor = UnitGetFar(aid);
	target = UnitGetFar(tid);
	Iwram_CopyWords(bak, target, size);

	// 範囲内での距離と属性の相性
	if (action->rng != 255)
	{
		dist = plan->tgt_dist[slot];
		if (dist < 0)
			dist = -dist;
	}
	else
	{
		dist = 0;
	}

	if (elm != 4)
	{
		s32 i;

		value = target->base_elm[elm].res;
		for (i = 0; i < 4 && value >= target->base_elm[i].res; i++)
			;
		if (i == 4)
			aff = -1;

		for (i = 0; i < 4 && value <= target->base_elm[i].res; i++)
			;
		if (i == 4)
			aff = 1;
	}

	if (plan->elm >= 0 && plan->elm <= 3)
	{
		cmd = &plan->cmd;
		if (*cmd != 2)
		{
			pow = actor->elm[plan->elm].pow;
			goto pow_end;
		}
	}
	else
		cmd = &plan->cmd;
	pow = 100;
pow_end:

	if (plan->cmd == 5 && (plan->elm >= 0 && plan->elm <= 3) && aff > 0)
	{
		s32 chance;

		chance = pow - target->elm[plan->elm].res;
		chance += 30;
		chance *= 65536 / 100;
		if (chance > (BtlRndFar() & 0xffff))
			BtlEvPush(BEV_SCR, 5);
	}

	// 命中判定と、効果に応じた事前処理
	type = action->tgt_flg & 15;
	{
		s32 res;

		res = plan->tgt_hit[slot];
		if (res == -1)
			hit = BtlHitChk(aid, tid, elm, action->efx, HitTbl[dist]);
		else
			hit = res;
	}
	if (action->efx == 50 || action->efx == 51)
	{ /* なかまをよぶ */
		s32 cls;
		s32 ent;

		cls = actor->class_id;
		ent = EnmSlotGet();
		if (action->efx == 51)
			cls = BtlGrpRnd(work->enm_grp);
		if (hit != 0 && EnmChk(cls) != 0 && ent >= 0)
		{
			s32 chg;

			chg = EnmCntDec(cls, 1);
			if (chg & 0x8000)
				EnmCntClr(cls);
			BtlUnitSetFar(ent, cls, chg & 0x7fff);
			{
				s32 i;

				for (i = 0; i < 6; i++)
				{
					if (work->enm_no[i] == 254)
					{
						work->enm_no[i] = ent;
						break;
					}
					if (work->enm_no[i] == 255)
					{
						work->enm_no[i] = ent;
						work->enm_no[i + 1] = 255;
						break;
					}
				}
			}
			EnmUpd();
			{
				struct BtlObj *obj;

				obj = BtlObjGet(ent);
				BtlObjMake(obj, ent, obj->anchor_x / 0x10000, obj->anchor_z / 0x10000);
			}
			BtlObjPut();
			{
				s32 num;

				num = BtlEnmList(list);
				for (cnt = 0; cnt < num; cnt++)
					ObjMotRst((u16)list[cnt]);
			}
			BtlEvPush(BEV_UNIT, ent);
			if (act_no != 503)
				MES_PUSH(MsgAppears);
			else
				MES_PUSH(MsgSplitOff);
		}
		else if (act_no == 503)
		{
			MES_PUSH(MsgSplitFail);
		}
		else
		{
			MES_PUSH(MsgNoOneCame);
		}
	}

	if (hit != 0)
	{
		if (action->efx == 53)
		{
			hit = 0;
			for (n = 0; n < 20; n++)
			{
				if (work->actions[n].unit_id == tid)
				{
					hit = 1;
					break;
				}
			}
		}
		else if (action->efx == 35)
		{
			half = 1;
		}
		else if (action->efx == 34)
		{
			crush = 1;
		}
		else if (action->efx == 27)
		{
			skip = 1;
		}
		else if (action->efx == 55)
		{
			if (actor->hp != 0)
				BtlEvPush(BEV_OBJ_EFX, aid);
		}
		else if (action->efx == 32)
		{
			if (target->ep != 0)
				type = 10;
			else
				hit = 0;
		}
	}

	//------------------------------------------------------------------
	// ダメージ・回復
	//------------------------------------------------------------------
	if (skip == 0 && (target->hp != 0 || BtlEfxRevChkFar(action->efx) != 0))
	{
		s32 ep;
		s32 cur;
		s32 ap;

		switch (type)
		{
		case 3: /* こうげき */
		case 4:
		{
			s32 def;

			hp = target->hp;
			def = target->def;
			scale = def;
			if (half != 0)
				scale = (u32)def >> 1;
			pass = 1;
			do
			{
				ELM_BONUS();
				if (pass == 0)
					bonus = 0;
				ap = action->pow;
				if (type == 4)
					dmg = BtlAtkCalc(actor->atk, scale, 0, bonus) * ap / 10;
				else
					dmg = BtlAtkCalc(actor->atk, scale, ap, bonus);
				dmg *= adj;
				if (crit != 0)
				{
					if (crit == 1)
						dmg = dmg * 5 / 4;
					else
						dmg = dmg * 3 / 2;
					dmg += (target->level / 5) + 6;
					if (pass == 0)
					{
						BtlEvPush(BEV_MARK, 0);
						{
							s32 mes;

							mes = (s32)MsgCritical;
							if (MIKATA_CHK(tid))
								mes += 1;
							BtlEvPush(BEV_MES_ADD, mes);
						}
					}
				}
				dmg += BtlRndFar() & 3;
				GUARD_CUT();
				if (dmg <= 0)
					dmg = 1;
				if (crush != 0)
				{
					if (dmg < hp - 1)
					{
						dmg = hp - 1;
						if (dmg <= 0)
							dmg = 1;
					}
				}
				if (GameFlag_TestFar(366) != 0 && *cmd == 5 && hp <= dmg)
				{
					dmg = hp - 1;
				}
				pass++;
			} while (pass <= 1);
			hp -= dmg;
			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_UNIT, tid);
			BtlEvPush(BEV_NUM, dmg);
			MES_PUSH((s32)(MIKATA_CHK(tid) ? MsgDmgEmphP : MsgDmgEmphE) + aff);

			if (hp <= 0)
			{
				BtlEvPush(BEV_OBJ_RES, tid);
				BtlEvPush(BEV_UNIT, tid);
				hp = 0;
				MES_SIDE(MsgGoesDown, MsgFelled);
			}
			else
				BtlEvPush(BEV_OBJ_END, tid);
			dealt = target->hp - hp;
			target->hp = hp;
			UnitRatioFar(tid);
			break;
		}

		case 10: /* EPダメージ */
		{
			if (action->pow == 0)
				break;
			ep = target->ep;
			ELM_BONUS();
			ap = action->pow;
			dmg = BtlPowCalc(ap, bonus, 256);
			dmg = dmg * EpLossTbl[dist] / 100;
			dmg *= adj;
			GUARD_CUT();
			if (action->efx == 32 && dmg > ep)
				dmg = ep;

			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_NUM, dmg);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MIKATA_CHK(tid) ? MsgEpLossP : MsgEpLossE);
			ep -= dmg;
			if (ep <= 0)
				ep = 0;
			BtlEvPush(BEV_OBJ_END, tid);
			dealt = target->ep - ep;
			target->ep = ep;
			UnitRatioFar(tid);
			break;
		}

		case 1: /* HPかいふく */
		{
			if (action->pow == 0)
				break;
			cur = target->hp;

			ap = action->pow;
			dmg = BtlRcvCalc(ap, elm == 4 ? 100 : pow, 256);
			dmg = dmg * HpRcvTbl[dist] / 100;
			dmg *= adj;
			dmg += BtlRndFar() & 3;
			cur += dmg;
			if (cur > target->max_hp)
			{
				cur = target->max_hp;
				dmg = cur - target->hp;
			}
			BtlEvPush(BEV_UNIT, tid);
			if (cur == target->max_hp)
				MES_PUSH(MsgHpFull);
			else
			{
				BtlEvPush(BEV_NUM, dmg);
				MES_PUSH(MsgHpRecover);
			}
			dealt = target->hp - cur;
			target->hp = cur;
			UnitRatioFar(tid);
			break;
		}

		case -1:
		{
			if (action->pow == 0)
				break;
			ep = target->ep;
			ELM_BONUS();
			ap = action->pow;
			dmg = BtlPowCalc(ap, bonus, 256);
			dmg = dmg * EpDmgTbl[dist] / 100;
			dmg *= adj;
			GUARD_CUT();
			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_NUM, dmg);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MIKATA_CHK(tid) ? MsgDmgP : MsgDmgE);
			ep -= dmg;
			if (ep <= 0)
				ep = 0;
			BtlEvPush(BEV_OBJ_END, tid);
			target->ep = ep;
			UnitRatioFar(tid);
			break;
		}

		case 5: /* HPダメージ */
		case 6:
		case 8:
		{
			s32 round;

			if (action->pow == 0)
				break;
			cur = target->hp;
			round = 1;
			do
			{
				ELM_BONUS();
				if (round == 0)
					bonus = 0;
				ap = action->pow;
				if (*cmd == 6)
				{
					s32 item;

					item = act_no - 380;
					if (item >= 0 && item <= 21)
					{
						switch (item)
						{
						case 0:
						case 6:
						case 12:
						case 18:
							rate = 3;
							break;
						case 1:
						case 7:
						case 13:
						case 19:
							rate = 6;
							break;
						case 2:
						case 8:
						case 14:
						case 20:
							rate = 9;
							break;
						case 3:
						case 9:
						case 15:
						case 21:
							rate = 12;
							break;
						}
					}
					ap += target->max_hp * rate / 100;
				}
				dmg = BtlPowCalc(ap, bonus, 256);
				dmg *= adj;
				switch (type & 15)
				{
				case 5:
					dmg = HpDmgTbl5[dist] * dmg / 100;
					break;
				case 8:
					dmg = dmg * HpDmgTbl8[dist] / 100;
					break;
				case 6:
					dmg = dmg * HpDmgTbl6[dist] / 100;
					break;
				}
				dmg += BtlRndFar() & 3;
				GUARD_CUT();
				if (GameFlag_TestFar(366) != 0 && *cmd == 6 && cur > dmg)
				{
					dmg = cur;
				}
				round++;
			} while (round <= 1);
			BtlEvPush(BEV_OBJ_BGN, tid);
			BtlEvPush(BEV_NUM, dmg);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH((s32)(MIKATA_CHK(tid) ? MsgDmgEmphP : MsgDmgEmphE) + aff);
			cur -= dmg;

			if (cur <= 0)
			{
				BtlEvPush(BEV_OBJ_RES, tid);
				BtlEvPush(BEV_UNIT, tid);
				cur = 0;
				MES_PUSH(MIKATA_CHK(tid) ? MsgGoesDown : MsgFelled);
			}
			else
				BtlEvPush(BEV_OBJ_END, tid);
			dealt = target->hp - cur;
			target->hp = cur;
			UnitRatioFar(tid);
			break;
		}

		case 11: /* EPかいふく */
		{
			if (action->pow == 0)
				break;
			ep = target->ep;
			ap = action->pow;
			dmg = BtlRcvCalc(ap, elm == 4 ? 100 : pow, 256);
			dmg = dmg * EpRcvTbl[dist] / 100;
			dmg *= adj;
			ep += dmg;
			if (ep > target->max_ep)
			{
				ep = target->max_ep;
				dmg = ep - target->ep;
			}
			BtlEvPush(BEV_UNIT, tid);
			if (ep == target->max_ep)
				MES_PUSH(MsgEpFull);
			else
			{
				BtlEvPush(BEV_NUM, dmg);
				MES_PUSH(MsgEpRecover);
			}
			target->ep = ep;
			UnitRatioFar(tid);
			break;
		}

		case 0:
		case 7:
		case 9:
			break;

		case 2:
			if (hit != 0)
			{
				if (action->pow == 0)
					break;
				cur = target->hp;
				ELM_BONUS();
				ap = action->pow;
				dmg = BtlPowCalc(ap, bonus, 256);
				dmg *= adj;
				dmg = dmg * HpDmgTbl[dist] / 100;
				GUARD_CUT();
				BtlEvPush(BEV_OBJ_BGN, tid);
				BtlEvPush(BEV_NUM, dmg);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MIKATA_CHK(tid) ? MsgDmgP : MsgDmgE);
				cur -= dmg;

				if (cur <= 0)
				{
					BtlEvPush(BEV_OBJ_RES, tid);
					BtlEvPush(BEV_UNIT, tid);
					cur = 0;
					MES_SIDE(MsgGoesDown, MsgFelled);
				}
				else
					BtlEvPush(BEV_OBJ_END, tid);
				dealt = target->hp - cur;
				target->hp = cur;
				UnitRatioFar(tid);
				break;
			}
			BtlEvPush(BEV_OBJ_END, tid);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MsgNoEffect);
			break;
		}
	}

	//------------------------------------------------------------------
	// 状態異常・能力変化などの追加効果
	//------------------------------------------------------------------
	BtlEvPush(BEV_UNIT, tid);
	if ((BtlEfxRevChkFar(action->efx) != 0 || target->hp != 0 || BtlEfxDeadChk(action->efx) != 0)
		&& hit != 0)
	{
		switch (action->efx)
		{

		case 64: /* 状態異常をすべてなおす */
			if (target->maboroshi != 0)
			{
				target->maboroshi = 0;
				MES_PUSH(MsgCureDelusion);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->shibire != 0)
			{
				target->shibire = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureStun);
			}
			target->nemuri = 0;
			if (target->fuuji != 0)
			{
				target->fuuji = 0;
				MES_PUSH(MsgCureSeal);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->shiryo != 0)
			{
				target->shiryo = 0;
				MES_PUSH(MsgCureCurse);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->akuryo != 0)
			{
				target->akuryo = 0;
				MES_PUSH(MsgCureSpirit);
				BtlEvPush(BEV_UNIT, tid);
			}
			if (target->doku != 0)
			{
				MES_PUSH(MsgCurePoison);
				target->doku = 0;
			}
			BtlEvPush(BEV_CLR, 0);
			break;

		case 4: /* 状態異常をなおす */
			if (target->maboroshi != 0)
			{
				target->maboroshi = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureDelusion);
			}
			if (target->shibire != 0)
			{
				target->shibire = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureStun);
			}
			target->nemuri = 0;
			if (target->fuuji != 0)
			{
				target->fuuji = 0;
				BtlEvPush(BEV_CLR, 0);
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgCureSeal);
			}
			if (target->shiryo == 0)
				break;
			target->shiryo = 0;
			BtlEvPush(BEV_CLR, 0);
			BtlEvPush(BEV_UNIT, tid);
			MES_PUSH(MsgCureCurse);
			break;

		case 61: /* HPかいふく */
		case 62: /* HPかいふく */
		{
			s32	 old;
			s32	 maxu;
			s32	 maxv;
			s32	 heal;
			u16 *pt;

			/* FAKEMATCH: walking a pointer down from hp to max_hp reads each stat twice, unsigned and signed, as the ROM does; plain f reads share one load. */
			pt = (u16 *)&target->hp;
			old = *pt--;
			heal = *(s16 *)(pt + 1);
			pt--;
			maxu = *pt--;
			maxv = *(s16 *)(pt + 1);
			if (action->efx == 61)
				heal += maxv * 60 / 100;
			else
				heal += maxv * 30 / 100;
			if (heal > (s16)maxu)
				heal = (s16)maxu;
			tmp = heal - (s16)old;
			if (tmp == 0 && type != 1)
				break;
			if (heal == (s16)maxu)
				MES_PUSH(MsgHpFull);
			else
			{
				BtlEvPush(BEV_NUM, tmp);
				MES_PUSH(MsgHpRecover);
			}
			target->hp = heal;
			UnitRatioFar(tid);
			break;
		}

		case 63: /* EPかいふく */
		{
			s32 old;
			s32 maxv;
			s32 heal;

			heal = target->ep;
			maxv = target->max_ep;
			old = heal;
			heal += maxv * 7 / 100;
			if (heal > maxv)
				heal = maxv;
			tmp = heal - old;
			if (tmp == 0 && type != 11)
				break;
			if (heal == maxv)
				MES_PUSH(MsgEpFull);
			else
			{
				BtlEvPush(BEV_NUM, tmp);
				MES_PUSH(MsgEpRecover);
			}
			target->ep = heal;
			UnitRatioFar(tid);
			break;
		}

		case 59: /* すばやさアップ */
			target->agi_lv = 8;
			target->agi_cnt = 5;
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->agi - bak->agi);
			MES_PUSH(MsgAgiUp);
			break;

		case 58: /* すばやさダウン */
			target->agi_lv = -4;
			target->agi_cnt = 5;
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->agi - target->agi);
			MES_PUSH(MsgAgiDown);
			break;

		case 9: /* こうげき下がる */
			target->atk_lv -= 1;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->atk - target->atk);
			MES_PUSH(MsgAtkDown);
			target->atk_cnt = 7;
			break;

		case 8: /* こうげき下がる */
			target->atk_lv -= 2;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->atk - target->atk);
			MES_PUSH(MsgAtkDown);
			target->atk_cnt = 7;
			break;

		case 7: /* こうげき上がる */
			target->atk_lv += 1;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->atk - bak->atk);
			MES_PUSH(MsgAtkUp);
			target->atk_cnt = 7;
			break;

		case 6: /* こうげき上がる */
			target->atk_lv += 2;
			STEP_LIM(target->atk_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->atk - bak->atk);
			MES_PUSH(MsgAtkUp);
			target->atk_cnt = 7;
			break;

		case 13: /* ぼうぎょ下がる */
			target->def_lv -= 1;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->def - target->def);
			MES_PUSH(MsgDefDown);
			target->def_cnt = 7;
			break;

		case 12: /* ぼうぎょ下がる */
			target->def_lv -= 2;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, bak->def - target->def);
			MES_PUSH(MsgDefDown);
			target->def_cnt = 7;
			break;

		case 11: /* ぼうぎょ上がる */
			target->def_lv += 1;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->def - bak->def);
			MES_PUSH(MsgDefUp);
			target->def_cnt = 7;
			break;

		case 10: /* ぼうぎょ上がる */
			target->def_lv += 2;
			STEP_LIM(target->def_lv);
			UnitCalcFar(tid);
			BtlEvPush(BEV_NUM, target->def - bak->def);
			MES_PUSH(MsgDefUp);
			target->def_cnt = 7;
			break;

		case 5: /* ふっき */
			if (target->hp != 0)
				break;
			MES_PUSH(MsgRevived);
			target->hp = target->max_hp;
			UnitRatioFar(tid);
			break;

		case 56: /* ふっき */
			if (target->hp != 0)
				break;
			MES_PUSH(MsgRevived);
			target->hp = target->max_hp / 2;
			UnitRatioFar(tid);
			break;

		case 57: /* ふっき */
			if (target->hp != 0)
				break;
			MES_PUSH(MsgRevived);
			target->hp = target->max_hp * 8 / 10;
			UnitRatioFar(tid);
			break;

		case 3: /* 毒がきえる */
			if (target->doku != 0)
				MES_PUSH(MsgCurePoison);
			target->doku = 0;
			break;

		case 17: /* たいせいダウン */
			RES_STEP(-1, (bak->res_lv - target->res_lv) * 20, MsgResDown);
			break;

		case 16: /* たいせいダウン */
			RES_STEP(-2, (bak->res_lv - target->res_lv) * 20, MsgResDown);
			break;

		case 15: /* たいせいアップ */
			RES_STEP(1, (target->res_lv - bak->res_lv) * 20, MsgResUp);
			break;

		case 14: /* たいせいアップ */
			RES_STEP(2, (target->res_lv - bak->res_lv) * 20, MsgResUp);
			break;

		case 18: /* 毒 */
			if (target->doku != 0)
				break;
			MES_PUSH(MsgPoisoned);
			target->doku = 1;
			break;

		case 19: /* もう毒 */
			if (target->doku > 1)
				break;
			MES_PUSH(MsgVenom);
			target->doku = 2;
			break;

		case 20: /* まぼろし */
			STAT_SET(target->maboroshi, MsgDelusion);
			break;

		case 21: /* こんらん */
			STAT_SET(target->konran, MsgConfused);
			break;

		case 22: /* こころをうばう */
			STAT_SET(target->charm, MsgCharmed);
			break;

		case 23: /* しびれ */
			STAT_SET(target->shibire, MsgStunned);
			break;

		case 24: /* ねむり */
			STAT_SET(target->nemuri, MsgAsleep);
			break;

		case 25: /* エナジーふうじ */
			MES_SIDE(MsgPsyBlock, MsgPsySeal);
			target->fuuji |= 7;
			break;

		case 67: /* エナジーふうじ */
			MES_SIDE(MsgPsyBlock, MsgPsySeal);
			target->fuuji |= 16;
			break;

		case 27: /* せんとうふのう */
			BtlEvPush(BEV_OBJ_RES, tid);
			if (target->side == MIKATA)
				MES_PUSH(MsgKoDown);
			else if (act_no == 219)
				MES_PUSH(MsgSuffocate);
			else
				MES_PUSH(MsgSpiritDrain);
			target->hp = 0;
			UnitRatioFar(tid);
			break;

		case 29: /* リフレイン */
			STAT_SET(target->refrain, MsgRefrain);
			break;

		case 30: /* リフレクト */
			STAT_SET(target->reflect, MsgReflect);
			break;

		case 31: /* HPをうばう */
		case 60:
		{
			s32 heal;

			heal = actor->hp;
			dmg = dealt;
			if (action->efx == 60)
				dmg /= 2;
			heal += dmg;
			if (heal > actor->max_hp)
			{
				heal = actor->max_hp;
				dmg = heal - actor->hp;
			}
			BtlEvPush(BEV_CLR, 0);
			BtlEvPush(BEV_UNIT, aid);
			if (heal == actor->max_hp)
				MES_PUSH(MsgHpFull);
			else
			{
				BtlEvPush(BEV_NUM, dmg);
				MES_PUSH(MsgHpRecover);
			}
			actor->hp = heal;
			UnitRatioFar(aid);
			break;
		}

		case 32: /* EPをうばう */
		{
			s32 heal;
			s32 amt;

			heal = actor->ep;
			amt = dealt;
			heal += amt;
			if (heal > actor->max_ep)
			{
				heal = actor->max_ep;
				amt = heal - actor->ep;
			}
			BtlEvPush(BEV_CLR, 0);
			BtlEvPush(BEV_UNIT, aid);
			if (heal == actor->max_ep)
				MES_PUSH(MsgEpFull);
			else
			{
				BtlEvPush(BEV_NUM, amt);
				MES_PUSH(MsgEpRecover);
			}
			actor->ep = heal;
			UnitRatioFar(aid);
			break;
		}

		case 69: /* EPをうばう */
			dmg = dealt / 10;
			if (target->ep < dmg)
				dmg = target->ep;
			if (actor->ep + dmg > actor->max_ep)
				dmg = actor->max_ep - actor->ep;
			if (dmg == 0)
				break;
			BtlEvPush(BEV_NUM, dmg);
			MES_SIDE(MsgLeechTake, MsgLeechGain);
			UnitEpAddFar(aid, dmg);
			break;

		case 33: /* つよさがもどる */
			if (target->atk_lv > 0)
			{
				target->atk_lv = 0;
				target->atk_cnt = 0;
			}
			if (target->def_lv > 0)
			{
				target->def_lv = 0;
				target->def_cnt = 0;
			}
			if (target->res_lv > 0)
			{
				target->res_lv = 0;
				target->res_cnt = 0;
			}
			if (target->agi_lv > 0)
				target->agi_lv = 0;
			target->elm_lv[0] = 0;
			target->elm_lv[1] = 0;
			target->elm_lv[2] = 0;
			target->elm_lv[3] = 0;
			MES_PUSH(MsgBuffsReset);
			break;

		case 26: /* あくりょう */
			MES_PUSH(MsgEvilSpirit);
			target->akuryo = 1;
			break;

		case 28: /* しりょう */
			if (target->shiryo == 0)
			{
				MES_PUSH(MsgDeathCurse);
				target->shiryo = 7;
				break;
			}
			if (target->shiryo <= 1)
				break;
			target->shiryo -= 1;
			BtlEvPush(BEV_NUM, target->shiryo);
			MES_PUSH(MsgDeathCount);
			break;

		case 66: /* そなえ */
			MES_PUSH(MsgReadies);
			target->sonae = 2;
			break;

		case 54: /* ちょうはつ */
			MES_PUSH(MsgChallenge);
			target->chouhatsu = 1;
			if (MIKATA_CHK(tid))
				work->chouhatsu_flg |= 2;
			break;

		case 53: /* みうごき */
			MES_PUSH(MsgImmobile);
			target->miugoki = 1;
			break;

		case 46: /* まもりのオーラ */
			MES_PUSH(MsgAura);
			if (target->guard_lv > 0)
				break;
			target->guard_lv = 1;
			break;

		case 47: /* まもりのオーラ */
			MES_PUSH(MsgAura2);
			if (target->guard_lv > 1)
				break;
			target->guard_lv = 2;
			break;

		case 45:
			BtlEvPush(BEV_MES, -1);
			break;

		default:
			break;
		}
	}

	//------------------------------------------------------------------
	// ダメージによる目覚めと、能力・表示の更新
	//------------------------------------------------------------------
	BtlEvPush(BEV_CLR, 0);

	if (target->hp != 0)
	{
		if (target->nemuri != 0)
			if (target->nemuri <= 6 && dealt > 0 && (3 & BtlRndFar()) == 0)
			{
				target->nemuri = 0;
				BtlEvPush(BEV_UNIT, tid);
				MES_PUSH(MsgWakes);
			}
	}
	MemFree(bak);
	UnitCalcFar(tid);
	PtyStDrawFar(BtlWk->pty_st);
	if (target->hp != 0)
		BtlEvPush(BEV_OBJ_END, tid);
	// あくりょうによる反動を行動側に加算する
	if (actor->akuryo != 0 && (BtlRndFar() & 3) == 0 && dealt > 0)
	{
		s32 share;

		share = dealt >> 2;
		if (share == 0)
			share = 1;
		plan->akuryo_dmg += share;
	}
}
