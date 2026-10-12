#include "vram_tab.h"
#include "window.h"
#include "unit/battle/battle.h"
#include "unit/battle/battle4.h"
#include "unit/battle/battle5.h"
#include "unit/battle/battle6.h"
#include "unit/battle/battle7.h"
#include "unit/battle/battle8.h"
#include "unit/battle/battle9.h"
#include "unit/battle/battle10.h"
#include "unit/battle/battle11.h"
#include "unit/battle/battle12.h"
#include "unit/battle/effect/object.h"
#include "unit/system/scheduler/runtime.h"
#include "unit/system/scheduler/runtime3.h"
#include "unit/system/scheduler/runtime4.h"
#include "unit/system/scheduler/runtime5.h"
#include "unit/system/scheduler/runtime6.h"
#include "unit/system/scheduler/runtime7.h"
#include "unit/system/scheduler/runtime8.h"
#include "unit/system/scheduler/runtime9.h"
#include "unit/system/scheduler/runtim10.h"
#include "unit/system/scheduler/runtim11.h"
#include "unit/system/scheduler/runtim13.h"
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
#include "owner_state.h"
#include "edition.h"
#include "io_reg.h"
#include "battle_unit.h"
#include "game_state.h"
#include "btlsys.h"
#include "text/msg_ids.h"
#include "mem.h"
#include "resource_ids.h"
#include "callback_scheduler.h"
#include "fixed_point_position.h"
#include "iwram_call.h"
#include "global_cells.h"
#include "btldisp.h"
#include "btlmes.h"
#include "btlpty.h"
#include "battle_intro.h"
#include "inventory.h"
#include "dma.h"
#include "scene.h"
#include "btlwork.h"
#include "btltype.h"
#include "btlsmn.h"
#include "tbs_edition.h"
#include "serial_runtime.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "battle_command.h"
#include "resource.h"
#include "heap_state.h"
#include "flag_ids.h"
#include "menu_list.h"
#include "audio_engine_symbols.h"
#include "ui.h"
#include "btlcalc.h"
#include "objdisp.h"
#include "motobj.h"
#include "battle_status_icon.h"
#include "animspr.h"
#include "field_event.h"
#include "metadata_lookup.h"
#include "fixed_math.h"
#include "battle_motion.h"
#include "effect_step.h"
#include "battle_escape.h"
#include "battle_target.h"
#include "btlobj.h"
#include "btlev.h"
#include "project.h"
#include "battle_random.h"
#include "irqlock.h"
#include "transform.h"
#include "battle_efx.h"
#include "character.h"
#include "io_write_queue.h"
#include "item.h"
#include "irq.h"
#include "vram_block.h"
#include "ownerval.h"
#include "sound_ids.h"
#include "ramcode.h"
#include "map_scroll.h"
#include "iowrite.h"
#include "battle_formation.h"
#include "b5_context.h"
#include "frame.h"

void BattleMotion_RunValueSequence(s32 id, s32 unused0, s32 unused1)
{
	u16	 selection[2];
	u16 *sel;
	s32	 target;
	s32	 remaining;

	UnitGetFar(id);
	ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(id)->object, 5);

	sel = selection;
	remaining = 1;
	do
	{
		target = 0xff;
		sel[1] = target;
		sel[0] = id;
		BattleLayout_HighlightPartyPanelsFar(sel);
		BattleMotion_SetRecordChildValues(BtlObjGet(id)->object, 7);
		WaitFrames(2);

		sel[0] = id;
		BattleLayout_HighlightPartyPanelsFar(sel);
		BattleMotion_SetRecordChildValues(BtlObjGet(id)->object, BattleMotion_GetSlotPalette(id));
		WaitFrames(2);
		remaining--;
	} while (remaining >= 0);

	PtyStDrawFar(BtlWk->pty_st);
}

//--------------------------------------------------------------------------
// actor.c
// バトル：倒れたユニットの絵と並びの後しまつ
//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
// ユニットを味方か敵の並びから外し、行動の予定からも消す
// id : ユニット番号
//--------------------------------------------------------------------------
void BattleActor_RemoveFromLists(s32 id)
{
	struct BtlWork *work;
	s32				i;
	u32				j;
	s32				unit;

	// 味方の並び
	work = BtlWk;
	UnitGetFar(id)->side = BATTLE_UNIT_ABSENT;
	for (i = 0;; i++)
	{
		if (work->party_units[i] == id)
		{
			work->party_units[i] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		if (work->party_units[i] == BATTLE_UNIT_LIST_END)
			break;
	}

	// 敵の並び
	/* FAKEMATCH: needed to match: the goto loop avoids the pointer walk and saved 0xfe constant produced by a for loop */
	do
	{
		j = 0;
	again:
		unit = work->enm_no[j];
		if (unit == id)
		{
			work->enm_no[j] = BATTLE_UNIT_REMOVED;
			goto removed;
		}
		j++;
		if (unit == BATTLE_UNIT_LIST_END)
			return;
		goto again;
	} while (0);

removed:
	EnmCntRel(id);
	for (j = 0; j < 20; j++)
	{
		if (work->actions[j].unit_id == id)
			work->actions[j].unit_id = BATTLE_UNIT_LIST_END;
	}
}

//--------------------------------------------------------------------------
// 倒れたユニットの絵：味方はアニメ 4、敵はアニメ 5 にする
// 敵はさらに 4 フレーム待って並びから外し、そのあと絵を 1 ドットずつ消す
// id : ユニット番号
//--------------------------------------------------------------------------
void BattleMotion_InitializeActorRecords(s32 id)
{
	void				   *recs[4];
	struct BtlUnit		   *unit;
	struct AnimationObject *rec;
	struct AnimationEntry  *ent;
	s32						n;

	unit = UnitGetFar(id);
	n = 0;
	while ((rec = GetMotionRecord(BtlObjGet(id)->object, n)) != 0)
	{
		if (unit->side != BATTLE_UNIT_ENEMY)
			AnimationObjects_SelectAnimationFar(rec, 4);
		else
			AnimationObjects_SelectAnimationFar(rec, 5);
		n++;
	}

	if (unit->side == BATTLE_UNIT_ENEMY)
	{
		n = 0;
		while ((rec = GetMotionRecord(BtlObjGet(id)->object, n)) != 0)
		{
			ent = rec->entries[0];
			recs[n] = rec;
			ent->param = 6;
			ent->frame = 0xff;
			n++;
		}
		WaitFrames(4);
		BattleActor_RemoveFromLists(id);
		Map_RenderAllAnimatedTileFramesFar(recs, n); // 1 ドットずつ消す
		ActivateBattleObjectSlot(id);
	}
}

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
		entry = (order->items + 1);
		if (*(s16 *)((char *)entry + offset) != 255)
		{
			cursor = entry;
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
		&& ((struct EnemyDefinition *)Owner_GetRecordFar(UnitGetFar(actor_id)->class_id))
				   ->target_strategy
			   != 2
		&& (u32)((action->tgt_flg & 0x0f) - 3) <= 2)
	{
		selected = -1;
		for (target_index = 0; target_index < target_count; target_index++)
		{
			for (inner_index = target_index; inner_index < target_count - 1; inner_index++)
			{
				this_unit = UnitGetFar(unit_ids[inner_index]);
				next_unit = UnitGetFar(unit_ids[inner_index + 1]);
				if (((struct EnemyDefinition *)Owner_GetRecordFar(UnitGetFar(actor_id)->class_id))
						->target_strategy
					== 0)
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
// A second view of struct BattlePromptSprite, kept: plain halfwords, where its bit-fields
// change BattlePresentation_WaitForAdvance (differ 50).
struct AdvanceSprite
{
	struct AdvanceSprite *next;
	u16					  attr0;
	u16					  attr1;
	u16					  attr2;
	u16					  attr3;
};

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
	Io_Write16(16, &REG_BLDALPHA);
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
		Io_Write16(16, &REG_BLDALPHA);
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

void BattleEv_MarkDisplay(void)
{
	gBattleDisplayWork->marked = 1;
}

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

//--------------------------------------------------------------------------
// tgtexec.c
// バトル：対象ごとの効果処理
//--------------------------------------------------------------------------
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

//--------------------------------------------------------------------------
// check_status_or_special_id.c
// バトル：EP を使うわざの判定
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// EP を使うわざなら 1 を返す。エナジードレインは EP に関係なく 1
// act_no : わざ番号
//--------------------------------------------------------------------------
u32 Ability_CheckStatusOrSpecialId(s32 act_no)
{
	u8 ep;

	if (act_no == 126) // エナジードレイン
		return 1;
	ep = BtlActGet(act_no)->pp_cost;
	return (u32)((0 - ep) | ep) >> 31; // ep が 0 でなければ 1
}

//--------------------------------------------------------------------------
// cmdplay.c
// バトル：敵の行動の選択と、バトルイベントの再生
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 行動の種類
//--------------------------------------------------------------------------
#define CMD_KOUGEKI 0 // こうげき
#define CMD_ENA 1	  // エナジー
#define CMD_ITEM 2	  // アイテム
#define CMD_BOUGYO 3  // ぼうぎょ
#define CMD_WAZA 4	  // 敵のわざ
#define CMD_BOUGYO2 7 // ぼうぎょ (効果 47)
#define CMD_NIGERU 99 // にげる

#define WAZA_KOUGEKI 1 // わざ番号：こうげき
#define MIKATA_MAX 7   // 仲間の番号の上限

// 敵の行動の記録 (BtlUnit の rewards を使う)
struct BattleAutomaticState
{
	unsigned initialized : 1;
	unsigned selection : 3; // 行動表の番号
	unsigned remaining : 28;
};

//--------------------------------------------------------------------------
// 敵の行動を選ぶ
// retry : 0 ならぼうぎょ・にげるだけをここで決め、1 なら残りの行動を決める
//--------------------------------------------------------------------------
s32 BattleCommand_SelectAutomatic(struct BattleActionRecord *req, s32 retry)
{
	u32							 sel;
	struct BattleAutomaticState *memo;
	struct EnemyDefinition		*enm;
	s32							 item_ok;
	s32							 tries;
	s32							 keep_going;
	s8							*strategy;
	u8							*item_mask;
	s16							 cmd;
	s32							 tgt;
	s32							 tgt2;
	s32							 side;
	s32							 side2;
	struct ItemDefinition		*item;
	s32							 use_item;
	s8							 mode;
	struct BtlAct				*item_act;
	struct BtlAct				*act;
	struct BtlUnit				*unit;
	s32							 act_no;
	s32							 tgt_flg;
	u8							 efx;
	u8							 tgt_mode;

	// 0-255 の乱数で、重みの表から行動表の番号を引く
	s32 WeightRnd(u8 * weights)
	{
		s32 rnd;
		s32 total;
		s32 no;
		s32 n;

		rnd = BtlRndFar() & 0xff;
		total = weights[0];
		no = 0;
		n = 0;
		if (rnd >= total)
		{
		loop:
			n++;
			if (n <= 7)
			{
				total += weights[n];
				if (rnd < total)
					no = n;
				else
					goto loop;
			}
		}
		return no;
	}

	unit = UnitGetFar(req->unit_id);
	keep_going = item_ok = 1;
	tries = 0;
	sel = -1U;
	if (unit->class_index != 0) // 仲間は選ばない
		return;
	if (retry != 0 && req->command != CMD_WAZA)
		return;

	enm = Owner_GetRecordFar(unit->class_id);
	memo = (struct BattleAutomaticState *)&unit->rewards;
	strategy = &enm->action_strategy;
	item_mask = &enm->item_actions;

	// 行動表の番号を選ぶ
select_again:
	mode = *strategy;
	switch (mode)
	{
	case 0: // 重みの表から
		sel = WeightRnd(BattleCommand_SelectionWeights[0]);
		break;
	case 1:
		sel = WeightRnd(BattleCommand_SelectionWeights[1]);
		break;
	case 2:
		sel = WeightRnd(BattleCommand_SelectionWeights[2]);
		break;
	case 3: // 乱数で始め、順に進む
		if (memo->initialized == 0)
		{
			memo->selection = BtlRndFar();
			memo->initialized = 1;
		}
	read_saved_selection:
		sel = memo->selection;
		if (retry != 0)
		{
			memo->selection = sel + 1;
		}
		break;
	case 4: // 記録した番号から順に進む
		goto read_saved_selection;
	case 5: // 選び直すたびに次へ
		sel++;
		break;
	case 6: // 選び直さない
		break;
	}

	// アイテムを使う番号なら、持っている道具を確かめる
	use_item = (*item_mask >> sel) & 1;
	use_item &= item_ok;
	act_no = enm->actions[sel];
	req->command = CMD_WAZA;
	if (use_item != 0 && retry != 0)
	{
		if (!(0x1ff & unit->inventory[0]))
		{
			use_item = 0;
			if (enm->target_strategy == 0)
			{
				req->command = CMD_ITEM;
				req->parameter = 0x1fd;
				return;
			}
		}
		if (use_item != 0)
		{
			item = Item_GetDirectFar(unit->inventory[0]);
			if (item->use_type == 1)
			{
				item_act = BtlActGet(item->action_id);
				act_no = item->action_id;
				req->command = CMD_ITEM;
				tgt_flg = item_act->tgt_flg;
				req->parameter = 0;
				switch (tgt_flg)
				{
				case 1:
				case 2:
					goto item_checked;
				}
			}
			use_item = 0;
		item_checked:
			if (use_item == 0)
				goto disable_item;
		}
		else
		{
		disable_item:
			item_ok = 0;
		}
	}

	if (keep_going == 0)
		goto finish_target;

	// ぼうぎょ・にげるのわざは、自分を対象にする
	act = BtlActGet(act_no);
	efx = act->efx;
	switch (efx)
	{
	case 46:
		req->command = CMD_BOUGYO;
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	case 47:
		req->command = CMD_BOUGYO2;
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	case 49:
		req->command = CMD_NIGERU;
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	}

	if (retry == 0)
	{
		switch (req->command)
		{
		case CMD_BOUGYO:
		case CMD_BOUGYO2:
			break;
		default:
			goto done;
		}
	}

	// エナジーならEPとふうじを確かめる
	// target_strategy が 0 でなければEP不足を、2 ならふうじも避ける
	if (req->command != CMD_ITEM && Ability_CheckStatusOrSpecialId(act_no) != 0)
	{
		req->command = CMD_ENA;
		req->parameter = act_no;
		if (act->pp_cost > unit->ep && enm->target_strategy != 0)
			goto next_candidate;
		if (unit->fuuji != 0 && enm->target_strategy == 2)
			goto next_candidate;
	}
	cmd = req->command;
	if (cmd == CMD_NIGERU && (u8)unit->chouhatsu != 0)
		goto next_candidate;
	if (retry != 0 && (cmd == CMD_BOUGYO || cmd == CMD_BOUGYO2))
		goto next_candidate;

	if (cmd == CMD_WAZA)
	{
		req->parameter = act_no;
		if (act_no == WAZA_KOUGEKI)
			req->command = CMD_KOUGEKI;
	}

	// 対象を選ぶ
	req->rng = act->rng;
	tgt_mode = act->target_mode;
	switch (tgt_mode)
	{
	case 2:
	case 4:
		tgt = BattleTarget_SelectForAction(req->unit_id, act);
		if (tgt == -2)
		{
			side = 0;
			if (req->unit_id <= (u32)MIKATA_MAX)
				side = 1;
			tgt = BattleTarget_SelectRandomPosition(side);
		}
		if (tgt != -1)
		{
			req->target = tgt;
			keep_going = 0;
		}
		break;
	case 1:
		tgt2 = BattleTarget_SelectForAction(req->unit_id, act);
		if (tgt2 == -2)
		{
			side2 = 0;
			if (req->unit_id <= (u32)MIKATA_MAX)
				side2 = 1;
			tgt2 = BattleTarget_SelectRandomPosition(side2);
		}
		if (tgt2 != -1)
		{
			req->target = tgt2;
			keep_going = 0;
		}
		break;
	case 3:
		req->target = Battle_FindTaggedSlotByValue(req->unit_id);
		break;
	default:
		tgt2 = Battle_FindTaggedSlotByValue(req->unit_id);
		req->target = tgt2;
		keep_going = 0;
		break;
	}

finish_target:
	if (retry == 0)
		keep_going = 0;
next_candidate:
	// 18 回選べなければ、ぼうぎょ
	if (keep_going != 0 && tries > 16)
	{
		req->command = CMD_BOUGYO;
		keep_going = 0;
	}
	tries++;
	if (keep_going != 0)
		goto select_again;
done:
	return;
}

//--------------------------------------------------------------------------
// DMA3 を三回設定し、IRQ 復帰命令のワードを呼び出し先として読む
//--------------------------------------------------------------------------
void Dma_StopAllThenRunHook(void)
{
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	Dma_Set(0, 0, DMA_ENABLE32 | DMA_32BIT, REG_DMA3);
	_call_via_r0(IwramIrqModeWord);
}

//--------------------------------------------------------------------------
// 止まっている再生を、対象の処理から始める
// cue : 当たったときの効果音 (0 なら変えない)
//--------------------------------------------------------------------------
void BattleEventRuntime_BeginPhase(s32 cue)
{
	struct BattleEventState *ev;

	ev = &BtlWk->events;
	if (ev->phase == BATTLE_PLAYBACK_IDLE)
	{
		ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		if (cue != 0)
		{
			ev->pending_cue = cue;
		}
	}
}

//--------------------------------------------------------------------------
// イベントの列を空にして、再生を登録する
// frames : 最初のイベントまでの待ち
//--------------------------------------------------------------------------
s32 BattleEventRuntime_SchedulePhase(s32 frames)
{
	struct BattleEventState *ev;

	ev = &BtlWk->events;
	ev->queue.count = 0;
	ev->event_index = 0;
	ev->timer = frames;
	ev->phase = BATTLE_PLAYBACK_DISPATCH;
	BtlWk->plan.target_count = 0;
	return Scheduler_AddOrUpdateCallback(BattleEvent_Playback, 0xc80);
}

//--------------------------------------------------------------------------
// バトルイベントの再生
//--------------------------------------------------------------------------
#define PLAY_MES 3		// メッセージの表示を待つ
#define PLAY_OKURI 5	// 送りのキーを待つ
#define PLAY_HIT 10		// 当たった者を点滅させる
#define PLAY_TAORE 11	// 倒れた敵を消す
#define PLAY_MES_ADD 13 // 続きのメッセージ (キーを待たない)

#define POSE_DAMAGE 5 // ダメージを受けたポーズ
#define PAL_HIT 7	  // 点滅の色

#define TAORE_FLASH 0x400 // 色を変えている間のタイマーの基準

#define OBJ_VRAM 0x06010000

//--------------------------------------------------------------------------
// スケジューラから呼ばれ、BtlTgtExec が積んだイベントを順に実行する
//--------------------------------------------------------------------------
void BattleEvent_Playback(void)
{
	u16						 sel[8];
	struct AnimationObject	*recs[4];
	struct BtlWork			*work = BtlWk;
	struct BattleEventState *ev = &work->events;

	// キャラのタイルを消す
	void ClrRecTiles(struct AnimationObject * rec, s32 value)
	{
		u8 *tiles = (u8 *)OBJ_VRAM + gVramBlockCache[rec->slot].offset;

		Iwram_ClearWords(tiles, rec->width * rec->height);
	}

	if (work->events.phase == BATTLE_PLAYBACK_IDLE)
		return;
	for (;;)
	{
		if (ev->phase == BATTLE_PLAYBACK_READY)
			return;
		if (ev->phase == BATTLE_PLAYBACK_RESOLVE_TARGET)
		{
			// 次の対象の効果を計算し、イベントを積む
			if (ev->queue.target_index < work->plan.target_count)
			{
				ev->queue.count = 0;
				ev->event_index = 0;
				ev->timer = 0;
				BtlTgtExec(&work->plan, ev->queue.target_index);
				ev->queue.target_index++;
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
			}
			else
			{
				ev->phase = BATTLE_PLAYBACK_READY;
			}
		}
		else if (ev->phase == BATTLE_PLAYBACK_DISPATCH)
		{
			// 積んだイベントを順に実行する
			while (ev->event_index < ev->queue.count)
			{
				{
					s32 i = ev->event_index;

					if (ev->phase != BATTLE_PLAYBACK_DISPATCH)
						break;
					if (ev->timer != 0)
					{
						ev->timer--;
						return;
					}
					switch (ev->queue.opcodes[i])
					{
					case BEV_SE: // 効果音
						AudioCommand_PlayFar(ev->queue.operands[i]);
						break;
					case BEV_SCR:
						Battle_SetRuntimeFlagBit0(ev, ev->queue.operands[i]);
						break;
					case BEV_UNIT: // メッセージに入れる名前
						UiWork_PushValueSlotFar(ev->queue.operands[i], 1);
						break;
					case BEV_NUM: // メッセージに入れる数
						UiWork_PushValueSlotFar(ev->queue.operands[i], 5);
						break;
					case BEV_ITEM: // メッセージに入れるアイテム名
						UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x1ff, 2);
						break;
					case BEV_ACT: // メッセージに入れるわざ名
						UiWork_PushValueSlotFar(ev->queue.operands[i] & 0x3fff, 4);
						break;
					case BEV_MARK:
						gBattleDisplayWork->marked = 1;
						break;
					case BEV_MES: // メッセージを出し、送りを待つ
						if ((s32)ev->queue.operands[i] >= 0)
							UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
						ev->phase = PLAY_MES;
						gKeysPressedLatch = 0;
						break;
					case BEV_MES_ADD: // メッセージを出し、送りを待たない
						if ((s32)ev->queue.operands[i] >= 0)
							UiText_PrepareMessageWorkFar(ev->queue.operands[i]);
						ev->phase = PLAY_MES_ADD;
						break;
					case BEV_CLR: // メッセージに入れたものを消す
						UiWork_ClearValueNameTablesFar();
						break;
					case BEV_OBJ_EFX:
						BattleActor_DestroyTemporaryObject(ev->queue.operands[i]);
						break;
					case BEV_OBJ_BGN: // 当たった
						if (ev->pending_cue > 0)
							AudioCommand_PlayFar(ev->pending_cue);
						ev->actor_id = ev->queue.operands[i];
						ObjectDispatch_ApplyArgumentToChildrenFar(
							BtlObjGet(ev->actor_id)->object, POSE_DAMAGE);
						ev->phase = PLAY_HIT;
						ev->timer = 0;
						break;
					case BEV_OBJ_RES: // 倒れた
					{
						struct BtlUnit		   *unit;
						struct AnimationObject *rec;
						s32						n;

						ev->actor_id = ev->queue.operands[i];
						BattleEnemy_RecordDefeat(ev->actor_id, ev->flags);
						BattleActor_ResetRuntimeFields(ev->actor_id);
						unit = UnitGetFar(ev->actor_id);
						for (n = 0;
							(rec = GetMotionRecord(BtlObjGet(ev->actor_id)->object, n)) != 0; n++)
						{
							if (unit->side != BATTLE_UNIT_ENEMY)
								AnimationObjects_SelectAnimationFar(rec, 4);
							else
								AnimationObjects_SelectAnimationFar(rec, 5);
						}
						if (unit->side == BATTLE_UNIT_ENEMY)
						{
							ev->phase = PLAY_TAORE;
							ev->timer = 0;
						}
						break;
					}
					case BEV_REDRAW: // 仲間の状態を描き直す
						PtyStDrawFar(BtlWk->pty_st);
						break;
					case BEV_OBJ_END: // 状態アイコン・色・ポーズを今の状態に合わせる
					{
						struct BtlObj *slot;

						BattleUnit_BuildStatusFlags(
							ev->queue.operands[i], BtlObjGet(ev->queue.operands[i]));
						slot = BtlObjGet(ev->queue.operands[i]);
						BattleMotion_SetRecordChildValues(
							slot->object, BattleMotion_GetSlotPalette(ev->queue.operands[i]));
						BattlePres_SetActorModeAndAction(ev->queue.operands[i]);
						break;
					}
					}
					ev->event_index++;
				}
			}
			if (ev->phase == BATTLE_PLAYBACK_DISPATCH)
				ev->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		}
		else if (ev->phase == PLAY_MES || ev->phase == PLAY_MES_ADD)
		{
			// メッセージが出きるまで待つ
			if (UiWork_IsCompleteFar() == 0)
				return;
			if (ev->phase == PLAY_MES_ADD)
			{
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
				ev->timer = 0;
			}
			else
			{
				ev->phase = PLAY_OKURI;
				ev->display_source = -1;
				ev->timer = gFrameTick;
			}
		}
		else if (ev->phase == PLAY_OKURI)
		{
			// 矢印を出して送りのキーを待つ
			// B を押し続けるか送りのキーを押すと進み、10 ティックを過ぎれば押し続けていても進む
			struct BattlePromptSprite  *spr = &ev->display_entry;
			u8						   *arrow = BattlePres_AdvanceArrowTiles;
			s32							pat = (s32)arrow + ((gFrameCount >> 2) & 7) * 128;
			struct BattleDisplayWork   *disp = gBattleDisplayWork;
			struct UiWindow			   *window = disp->window;
			struct BattleDisplayOffset *offset = disp->offset;
			s32							x = 0;

			if (ev->display_source == -1)
				ev->display_source = work->prompt_slot;
			UiWork_ClearValueNameTablesFar();
			QueueIoWriteDelay10((u32)&REG_WINOUT, 4);
			QueueIoWriteDelay6((u32)&REG_WINOUT, 16);
			((u32 *)spr)[1] = 0xa000;
			((u32 *)spr)[2] = x;
			spr->tile = Resource_GetBuffer(ev->display_source, pat);
			x = window->x * 8 + (offset->x >> 8) + 4;
			spr->x = x;
			spr->y = Trig_Sin(gFrameCount << 12) / 32768 + window->y * 8 + (offset->y >> 8) + 6;
			if ((BATTLE_OPTIONS & KEY_B) || (gKeysPressedLatch & KEY_OKURI)
				|| ((u32)(gFrameTick - ev->timer) > 10 && (BATTLE_OPTIONS & KEY_OKURI)))
			{
				AudioCommand_PlayFar(111);
				ev->phase = BATTLE_PLAYBACK_DISPATCH;
				ev->timer = 0;
			}
			else
			{
				Runtime_PushSlotEntry(spr, 240);
				return;
			}
		}
		else if (ev->phase == PLAY_HIT)
		{
			// 2 フレームごとに色を変え、9 フレームで戻る
			if (ev->timer & 1)
			{
				if (ev->timer & 2)
				{
					struct BtlObj *slot;

					sel[0] = 0xff;
					slot = BtlObjGet(ev->actor_id);
					BattleMotion_SetRecordChildValues(
						slot->object, BattleMotion_GetSlotPalette(ev->actor_id));
				}
				else
				{
					sel[0] = ev->actor_id;
					sel[1] = 0xff;
					BattleMotion_SetRecordChildValues(BtlObjGet(ev->actor_id)->object, PAL_HIT);
				}
				BattleLayout_HighlightPartyPanelsFar(sel);
			}
			ev->timer++;
			if (ev->timer <= 8)
				return;
			ev->phase = BATTLE_PLAYBACK_DISPATCH;
			ev->timer = 0;
		}
		else if (ev->phase == PLAY_TAORE)
		{
			// flags があれば、まず 30 フレームのあいだ色を変える
			if (ev->timer == 0 || ev->timer >= TAORE_FLASH)
			{
				s32 frame = 6;

				if (ev->timer == 0 && ev->flags != 0)
				{
					s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actor_id)->class_id);

					if (kind >= 0)
					{
						kind--;
						if (kind < 0)
							kind = 0;
						AudioCommand_PlayFar(kind + 146);
					}
					ev->timer = TAORE_FLASH;
				}
				if (ev->timer > TAORE_FLASH + 29)
					ev->timer = 0;
				if (ev->timer == 0)
				{
					s32 kind = Summon_GetLayoutKind(UnitGetFar(ev->actor_id)->class_id);

					if (kind >= 0)
						AudioCommand_PlayFar(kind + 146);
				}
				if (ev->timer >= TAORE_FLASH)
					frame = (ev->timer - TAORE_FLASH) / 8 % 5 + 1;
				if (frame == 6 || (ev->timer & 7) == 0)
				{
					struct AnimationObject	*rec;
					s32						 n;
					struct AnimationObject **slot;
					s32						 none;

					for (n = 0, slot = recs, none = 0xff;
						(rec = GetMotionRecord(BtlObjGet(ev->actor_id)->object, n)) != 0; n++)
					{
						struct AnimationEntry *child;
						u8					   old;

						*slot++ = rec;
						child = rec->entries[0];
						old = child->frame;
						child->param = frame;
						child->frame = old | none;
					}
				}
				ev->timer++;
				return;
			}
			if (ev->timer == 4)
			{
				BattleActor_RemoveFromLists(ev->actor_id);
				ev->timer++;
				return;
			}
			if (ev->timer > 4)
			{
				// タイルを順に描き換えて消す
				struct BtlObj		   *slot;
				struct AnimationObject *rec;
				s32						count;
				s32						frame;
				s32						base;

				slot = BtlObjGet(ev->actor_id);
				slot->fading = 1;
				for (count = 0; (rec = GetMotionRecord(slot->object, count)) != 0; count++)
					recs[count] = rec;
				base = ev->timer * 4;
				frame = base - 20;
				if (frame > 127)
				{
					s32 k;

					for (k = 0; k < count; k++)
						ClrRecTiles(recs[k], 0);
					ActivateBattleObjectSlot(ev->actor_id);
					ev->phase = BATTLE_PLAYBACK_DISPATCH;
					ev->timer = 0;
					return;
				}
				{
					s32 k;

					for (k = 0; k < count; k++)
					{
						render_animated_tile_frameFar((u8 *)recs[k], frame);
						render_animated_tile_frameFar((u8 *)recs[k], base - 19);
						render_animated_tile_frameFar((u8 *)recs[k], base - 18);
						render_animated_tile_frameFar((u8 *)recs[k], base - 17);
					}
				}
				ev->timer++;
				return;
			}
			ev->timer++;
			return;
		}
	}
}

u32 BattleEventRuntime_Reset(void)
{
	struct BattleEventState *events;

	events = &BtlWk->events;
	events->phase = BATTLE_PLAYBACK_IDLE;
	events->queue.count = 0;
	events->event_index = 0;
	events->timer = 0;
	events->queue.target_index = 0;
	events->pending_cue = 0x86;
	events->flags = 0;
}

s32 BattleEventRuntime_WaitForReady(void)
{
	s32						 state;
	struct BattleEventState *events;

	events = &BtlWk->events;
	state = events->phase;
	if (state == BATTLE_PLAYBACK_IDLE)
	{
		events->phase = BATTLE_PLAYBACK_RESOLVE_TARGET;
		state = BATTLE_PLAYBACK_RESOLVE_TARGET;
	}
	if (state != BATTLE_PLAYBACK_READY)
	{
		do
		{
			WaitFrames(1U);
		} while (events->phase != BATTLE_PLAYBACK_READY);
	}
	Scheduler_RemoveCallback(BattleEvent_Playback);
	return BattleEventRuntime_Reset();
}

//--------------------------------------------------------------------------
// is_unit_listed.c
// バトル：ユニットが戦闘中の並びにいるかの判定
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ユニットが自分の側の並びにいれば 1 を返す
// id : ユニット番号（0-7 味方、それ以外は敵）
//--------------------------------------------------------------------------
u32 BattleParty_IsUnitListed(u32 id)
{
	u16 list[8];
	s32 side;
	s32 cnt;
	s32 i;

	side = BATTLE_SIDE_PARTY;
	if (id > 7)
		side = BATTLE_SIDE_ENEMIES;
	cnt = BattleParty_ListActorIds(side, list);

	for (i = 0; i < cnt; i++)
	{
		if (list[i] == id)
			break;
	}

	return i != cnt;
}

s32 BattlePlacement_CountValidEntries(u32 unit_id, u8 *counts)
{
	u16						  unit_ids[8];
	struct DjinnRecoveryList *list;
	s32						  total;
	s32						  count;
	s32						  i;
	s32						  j;
	s32						  side_mask;
	s32						  side;

	count = 0;
	side_mask = BATTLE_SIDE_PARTY;
	if (unit_id > 7)
		side_mask = BATTLE_SIDE_ENEMIES;
	total = BattleParty_ListActorIds(side_mask, unit_ids);
	side = 0;
	if (unit_id > 7)
		side = 1;
	list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(side))->list;
	if (counts != 0)
		for (j = 3; j >= 0; j--)
			counts[j] = 0;
	i = 0;
	if (list->count != 0)
	{
		do
		{
			if (list->entries[i].turns == -1)
			{
				for (j = 0; j < total; j++)
					if (unit_ids[j] == list->entries[i].unit_id)
						break;
				if (j != total)
				{
					if (counts != 0)
						counts[list->entries[i].element]++;
					count++;
				}
			}
			i++;
		} while (i != list->count);
	}
	return count;
}

//--------------------------------------------------------------------------
// command.c
// バトル：行動の宣言と、対象・効果の計画
//--------------------------------------------------------------------------
// ジンの指定：属性 (bit 8-11) と番号 (bit 0-7)
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)
#define CMD_DJINN 5	  // ジン
#define CMD_SHOUKAN 6 // しょうかん
#define CMD_NONE 8	  // 何もしない
#define TGT_MAX 14

//--------------------------------------------------------------------------
// 行動を宣言し、対象と効果の計画を作る
// 返り値 : 0 = 実行する、-1 = 失敗 (メッセージ済み)、-2 = 何もしない
//--------------------------------------------------------------------------
s32 BattleCommand_BuildPlan(struct BattleActionRecord *req, struct BtlPlan *plan)
{
	struct BtlUnit *unit = UnitGetFar(req->unit_id);
	struct BtlWork *work = BtlWk;
	s32				tid;
	s32				act_no;

	//----------------------------------------------------------------------
	// わざの範囲にいる者を対象に並べる
	// 誰もいなければ、身を守って -1
	//----------------------------------------------------------------------
	s32 TgtSet(s32 no)
	{
		struct BtlAct *act = BtlActGet(no);
		s32			   mode = act->target_mode;
		s32			   dead_ok = 0;
		s32			   cnt;
		s32			   pty_cnt;
		s32			   enm_cnt;
		s32			   center;
		s32			   rng;
		s32			   pos;
		s32			   id;

		switch (act->efx)
		{
		case 5: // ふっき
		case 56:
		case 57:
			dead_ok = 1;
			break;
		}
		switch (mode)
		{
		case 0: // ひとり
			plan->tgt_dist[0] = mode;
			plan->target_count = 1;
			plan->tgt_no[0] = tid;
			plan->tgt_adj[0] = 1;
			break;
		case 4:
			plan->tgt_dist[0] = 0;
			plan->target_count = 1;
			plan->tgt_no[0] = tid;
			plan->tgt_adj[0] = 1;
			break;
		default:
			// 狙った位置から rng - 1 までの並び
			cnt = 0;
			for (pos = 0; work->party_units[pos] != BATTLE_UNIT_LIST_END; pos++)
				;
			pty_cnt = pos;
			for (pos = 0; work->enm_no[pos] != BATTLE_UNIT_LIST_END; pos++)
				;
			enm_cnt = pos;
			center = req->target & 15;
			rng = req->rng;
			{
				s32 first = center - rng + 1;
				s32 last = center + rng - 1;

				for (pos = first; pos <= last; pos++)
				{
					if (pos < 0)
						continue;
					if (req->target & 128) // 敵の側
					{
						if (pos >= enm_cnt)
							continue;
						id = work->enm_no[pos];
						if (id == BATTLE_UNIT_REMOVED)
							continue;
						if (!dead_ok && UnitGetFar(id)->hp == 0)
							continue;
						plan->tgt_adj[cnt] = 1;
						plan->tgt_dist[cnt] = pos - center;
						plan->tgt_no[cnt] = id;
						cnt++;
					}
					else
					{
						if (pos >= pty_cnt)
							continue;
						id = work->party_units[pos];
						if (id == BATTLE_UNIT_REMOVED)
							continue;
						if (!dead_ok && UnitGetFar(id)->hp == 0)
							continue;
						plan->tgt_adj[cnt] = 1;
						plan->tgt_dist[cnt] = pos - center;
						plan->tgt_no[cnt] = id;
						cnt++;
					}
				}
			}
			plan->target_count = cnt;
			if (cnt <= 0)
			{
				UiWork_PushValueSlotFar(req->unit_id, 1);
				UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
				if (unit->guard_lv == 0)
					unit->guard_lv = 1;
				return -1;
			}
		}
	}

	tid = Battle_GetTaggedSlotValue(req->target);
	BattleEventRuntime_Reset();
	plan->akuryo_dmg = 0;
	plan->aid = req->unit_id;
	plan->target_count = 0;
	plan->presentation_flags = 0;
	plan->failure = 0;
	plan->elm = 4;
	UiWork_ClearValueNameTablesFar();
	if (unit->hp == 0)
		return -2;

	// デバッグ：R を押していれば敵全員 (SELECT も押せば味方全員) を倒す
	if (BATTLE_AUTO_MODE && GameFlag_TestFar(0x16d) && (BATTLE_OPTIONS & KEY_R))
	{
		s32 teki = 1;
		s32 i;
		s32 id;

		if (BATTLE_OPTIONS & KEY_SELECT)
			teki = 0;
		for (i = 0;; i++)
		{
			if (teki)
				id = work->enm_no[i];
			else
				id = work->party_units[i];
			if (id == BATTLE_UNIT_LIST_END)
				break;
			if (id == BATTLE_UNIT_REMOVED)
				continue;
			if (Owner_AdjustFirstValueFar(id, 0xc0000000) == 0)
			{
				BtlEvPush(BEV_OBJ_BGN, id);
				BtlEvPush(BEV_OBJ_RES, id);
			}
		}
		BattleEv_DispatchQueued();
		return -2;
	}

	// 動けない状態
	UiWork_ClearValueNameTablesFar();
	if (unit->miugoki)
	{
		unit->miugoki = 0;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgUnableToMove);
		return -1;
	}
	if (unit->nemuri)
	{
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgIsAsleep);
		return -1;
	}
	if (unit->shibire)
	{
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgIsParalyzed);
		return -1;
	}
	// かなしばりは 1/4 で動けない
	if ((unit->restraint & 1) && req->command != CMD_BOUGYO && (BtlRndFar() & 3) == 0)
	{
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgIsBound);
		return -1;
	}
	if (req->command == CMD_NONE)
		return -2;
	act_no = WAZA_KOUGEKI;
	{
		s32 i;

		for (i = 0; i < TGT_MAX; i++)
			plan->tgt_crit[i] = 0;
		for (i = 0; i < TGT_MAX; i++)
			plan->tgt_hit[i] = -1;
	}

	//----------------------------------------------------------------------
	// 行動ごとの宣言
	//----------------------------------------------------------------------
	switch (req->command)
	{
	case CMD_NIGERU:
		if ((u16)req->unit_id <= MIKATA_MAX)
		{
			UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyFlees);
		}
		else
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorRuns);
		}
		BattlePresentation_WaitForAdvance();
		plan->outcome = 7;
		return 0;

	case CMD_KOUGEKI:
		// 武器の力が発動すれば、そのわざになる
		act_no = RollWeaponUnleashFar(unit);
		if (TgtSet(act_no) == -1)
			return -1;
		if (act_no != WAZA_KOUGEKI)
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(Inventory_GetEquippedItemFar(unit, 1), 2);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgWeaponHowls);
			BattleEv_MarkDisplay();
			UiWork_PushValueSlotFar(act_no, 4);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgWeaponHowls + 1);
		}
		break;

	case CMD_ENA:
	{
		struct BtlAct *act;
		s32			   ok = 1;

		act_no = req->parameter;
		act = BtlActGet(act_no);
		if (TgtSet(act_no) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(act_no, 4);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorCasts);
		// EPが足りないか、ふうじなら失敗
		if (unit->ep < act->pp_cost)
		{
			plan->failure = 2;
			ok = 0;
		}
		if (unit->fuuji)
		{
			plan->failure = 1;
			ok = 0;
		}
		if (ok)
		{
			plan->failure = 0;
			unit->ep -= act->pp_cost;
			UnitRatioFar(req->unit_id);
			if (unit->ep < 0)
				unit->ep = 0;
			if (unit->ep > unit->max_ep)
				unit->ep = unit->max_ep;
		}
	}
	break;

	case CMD_ITEM:
	{
		struct ItemDefinition *item;

		if (req->parameter < 0)
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgItemAlreadyUsed);
			return -1;
		}
		item = Item_GetDirectFar(unit->inventory[req->parameter]);
		act_no = item->action_id;
		if (act_no == 0 || (unit->inventory[req->parameter] & 0x400))
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
			if (unit->guard_lv == 0)
				unit->guard_lv = 1;
			return -1;
		}
		if (TgtSet(act_no) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(unit->inventory[req->parameter], 2);
		if (item->use_type == 2 || item->use_type == 0)
		{
			switch (item->type)
			{
			case 1:
			case 3:
			case 6:
			case 7:
			case 8:
				UiText_ShowMessageAndWaitCoreFar((s32)MsgActorRaisesItem); // かざした
				break;
			default:
				UiText_ShowMessageAndWaitCoreFar((s32)MsgActorUsesBattleItem); // つかった
				break;
			}
		}
		else
		{
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorUsesBattleItem);
		}
	}
	break;

	case CMD_BOUGYO:
	case CMD_BOUGYO2:
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorDefends);
		return -1;

	case CMD_NONE:
		return -2;

	case CMD_WAZA:
	{
		s32 mes;

		act_no = req->parameter;
		if (TgtSet(act_no) == -1)
			return -1;
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(act_no, 4);
		if ((BtlActGet(act_no)->tgt_flg & 15) == 6)
			mes = (s32)MsgActorUnleashesAbility;
		else
			mes = (s32)MsgActorUsesAbility;
		// わざごとの言い方
		switch (act_no)
		{
		case 435: // ちょうおんぱ
		case 436:
			mes = (s32)MsgEmitsUltrasonicWaves;
			break;
		case 224: // ヒートミラージュ
			mes = (s32)MsgActorCasts;
			break;
		case 500: // おどし
			mes = (s32)MsgGlowersFerociously;
			break;
		case 501: // てがふるえている
			mes = (s32)MsgIsIntimidated;
			break;
		case 499: // けんせい
			mes = (s32)MsgBalefulGaze;
			break;
		case 494: // ミミズをたべる
			mes = (s32)MsgEatsWorms;
			break;
		case 437: // スクウィールロアー ～ じごくのとおぼえ
		case 438:
		case 439:
		case 440:
		case 441:
			mes = (s32)MsgLetsOutAbility;
			break;
		case 442: // ウイングビート ～ ウイングストローク
		case 443:
		case 444:
			mes = (s32)MsgActorUsesAbility;
			break;
		case 472: // くさったたいえき
			mes = (s32)MsgEnemyUnleashesAbility;
			break;
		case 488: // せつないひとみ
			mes = (s32)MsgGlowersMiserably;
			break;
		case 492: // ふらんしゅう
			mes = (s32)MsgSmellOfDecay;
			break;
		case 495: // きょうぼうか
			mes = (s32)MsgFuriousRage;
			break;
		case 503: // ぞうしょくうんどう
			mes = (s32)MsgAttemptsToDivide;
			break;
		case 504: // なかまをさがす
			mes = (s32)MsgLooksForAllies;
			break;
		case 508: // たすけをさがす
			mes = (s32)MsgLooksForHelp;
			break;
		}
		UiText_ShowMessageAndWaitCoreFar(mes);
	}
	break;

	case CMD_DJINN:
		// セットされていなければ、スタンバイのジンをセットし直す
		act_no = Djinn_GetDefinitionHeaderFar(
			DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
		if (!Djinn_IsActiveFar(
				req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter)))
		{
			if (Trade_CanOfferDjinnFar(
					req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter)))
			{
				BtlActGet(act_no);
				BattlePres_SetActorModes(0, 0);
				Djinn_ActivateFar(
					req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
				Trade_RemoveOfferFar(
					req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
				UnitCalcFar(req->unit_id);
				BattleEventRuntime_Reset();
				BattleEventRuntime_SchedulePhase(30);
				BtlEvPush(BEV_UNIT, req->unit_id);
				BtlEvPush(BEV_ACT,
					DJINN_ELEMENT(req->parameter) * 20 + DJINN_INDEX(req->parameter) + 300);
				BtlEvPush(BEV_SE, 175);
				BtlEvPush(BEV_REDRAW, 0);
				BtlEvPush(BEV_MES, (s32)MsgDjinnSet);
				BtlEvPush(BEV_OBJ_END, req->unit_id);
				AudioCommand_PlayFar(212);
				ObjectDispatch_ApplyArgumentToChildrenFar(BtlObjGet(req->unit_id)->object, 3);
				ObjectDispatch_ApplyValueToChildrenFar(
					(struct DispatchObject *)BtlObjGet(req->unit_id)->object, 32);
				BattleFx_PlayUnitElementEffect(req->unit_id, DJINN_ELEMENT(req->parameter), 3, 0);
				BattleEventRuntime_WaitForReady();
				return -2;
			}
			// リカバリー中
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(act_no, 4);
			AudioCommand_PlayFar(114);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgDjinnInRecovery);
			WaitFrames(60);
			return -1;
		}
		// セットされたジンを解き放つ
		{
			struct BtlAct *act;

			if (TgtSet(act_no) == -1)
				return -1;
			Trade_AddOfferFar(
				req->unit_id, DJINN_ELEMENT(req->parameter), DJINN_INDEX(req->parameter));
			act = BtlActGet(act_no);
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(act_no, 4);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgActorUnleashesDjinn);
			plan->elm = act->damage_class;
		}
		break;

	case CMD_SHOUKAN:
	{
		const struct SummonDefinition *summon;
		struct DjinnRecoveryList	  *list;
		s32							   side;
		u8							   counts[4];
		/* FAKEMATCH: needed to match: unread: the reference frame keeps 8 bytes below counts */
		u8	unused[8];
		s32 i;

		// 属性ごとのスタンバイの数が足りるか
		summon = SummonDefinition_Get(req->parameter);
		BattlePlacement_CountValidEntries(req->unit_id, counts);
		side = 0;
		if ((u16)req->unit_id > MIKATA_MAX)
			side = 1;
		list = &((struct DjinnRecoveryTable *)Trade_GetOfferStateFar(side))->list;
		for (i = 0; i < 4 && counts[i] >= summon->djinn_required[i]; i++)
			counts[i] = summon->djinn_required[i];
		act_no = summon->name_message_id;
		if (TgtSet(act_no) == -1)
			return -1;
		if (i != 4)
		{
			UiWork_PushValueSlotFar(req->unit_id, 1);
			UiWork_PushValueSlotFar(act_no, 4);
			UiText_ShowMessageAndWaitCoreFar((s32)MsgSummonLacksDjinn);
			return -1;
		}
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiWork_PushValueSlotFar(act_no, 4);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorSummons);
		// 使うスタンバイのジンに印 (-2) をつける
		for (i = 0; i != list->count; i++)
		{
			struct DjinnRecoveryEntry *entry = &list->entries[i];

			if (entry->turns == -1 && BattleParty_IsUnitListed(entry->unit_id)
				&& counts[entry->element] != 0)
			{
				entry->turns = -2;
				counts[entry->element]--;
			}
		}
	}
	break;
	}

	//----------------------------------------------------------------------
	// こうげきの計画
	//----------------------------------------------------------------------
	if (act_no == WAZA_KOUGEKI)
	{
		struct BtlUnit *tgt = UnitGetFar(plan->tgt_no[0]);

		plan->act_no = WAZA_KOUGEKI;
		plan->elm = Item_GetEquippedElementFar(req->unit_id);
		plan->outcome = 2;
		if (unit->class_index == 0) // 敵
		{
			plan->presentation_flags = Battle_GetSpriteFlagNibble(unit->class_id) | 0x4000;
		}
		else
		{
			plan->presentation_flags = 0;
			switch (unit->class_id)
			{
			case ROBIN:
				plan->presentation_flags = 0x4001;
				break;
			case GERALD:
				plan->presentation_flags = 0x4001;
				break;
			case IWAN:
				plan->presentation_flags = 0x4004;
				break;
			case MEARI:
				plan->presentation_flags = 0x4004;
				break;
			case JASMINE:
				plan->presentation_flags = 0x4001;
				break;
			}
		}
		UiWork_PushValueSlotFar(req->unit_id, 1);
		UiText_ShowMessageAndWaitCoreFar((s32)MsgActorAttacks);
		// 外れ：まぼろしなら 153/256、ほかに 1/32
		// 相手が動けなければ外れない
		/* FAKEMATCH: needed to match: the one-pass block keeps the native test order */
		do
		{
			if (tgt->hp == 0 || tgt->nemuri != 0 || tgt->shibire != 0 || tgt->miugoki != 0
				|| tgt->charm != 0)
				break;
			if (unit->maboroshi != 0 && (BtlRndFar() & 255) <= 152)
				plan->tgt_adj[0] = 0;
			if ((BtlRndFar() & 31) == 0)
				plan->tgt_adj[0] = 0;
		} while (0);
		if (GameFlag_TestFar(366))
			plan->tgt_adj[0] = 0;
		// 会心：1/32、または装備による確率
		if (tgt->hp != 0)
		{
			if ((BtlRndFar() & 31) == 0)
				plan->tgt_crit[0] = 1;
			else if ((Equipment_GetUnleashRateBonusFar(unit) << 16) / 200 > (BtlRndFar() & 0xffff))
				plan->tgt_crit[0] = 1;
		}
	}

	//----------------------------------------------------------------------
	// わざの計画
	//----------------------------------------------------------------------
	else
	{
		struct BtlAct *act = BtlActGet(act_no);

		plan->elm = act->damage_class;
		plan->presentation_flags = 0;
		plan->act_no = act_no;
		if (act->efx == 65 || act->efx == 41 || act->efx == 42 || act->efx == 43 || act->efx == 44
			|| act->efx == 68)
		{
			// ある確率で、ダメージの倍率を上げる
			s32 chance;
			s32 amount;

			if (act->efx == 65 || act->efx == 68)
				chance = 153;
			else if (act->efx == 41 || act->efx == 43)
				chance = 32;
			else
				chance = 64;
			if (act->efx == 65 || act->efx == 41 || act->efx == 42)
				amount = 1;
			else
				amount = 2;
			if ((BtlRndFar() & 255) < chance)
			{
				s32 i;

				for (i = 0; i < plan->target_count; i++)
					plan->tgt_adj[i] += amount;
			}
		}
		else if (act->efx >= 36 && act->efx <= 40)
		{
			// 会心の確率 1/64 ～ 1/4
			s32 mask;

			switch (act->efx)
			{
			case 36:
				mask = 63;
				break;
			case 37:
				mask = 31;
				break;
			case 38:
				mask = 15;
				break;
			case 39:
				mask = 7;
				break;
			case 40:
			default:
				mask = 3;
				break;
			}
			if ((BtlRndFar() & mask) == 0)
			{
				s32 i;

				for (i = 0; i < plan->target_count; i++)
					plan->tgt_crit[i] = 2;
			}
		}
		else if (act_no == 178) // めっさつのじゅつ：命中を先に決める
		{
			s32 i;

			for (i = 0; i < plan->target_count; i++)
				plan->tgt_hit[i] =
					BtlHitChk(req->unit_id, plan->tgt_no[i], act->damage_class, act->efx, 100);
		}
		if (act_no >= 0 && act_no <= 518)
		{
			plan->presentation_flags = Battle_ActionFlags[act_no];
			if (plan->tgt_adj[0] > 1)
				plan->presentation_flags =
					Battle_ActionFlags[act_no] + (plan->tgt_adj[0] << 12) - 0x1000;
		}
		if ((act_no >= 0 && act_no <= 517) && Battle_ActionStatus[act_no] != 0)
			plan->outcome = Battle_ActionStatus[act_no];
		else if (Ability_CheckStatusOrSpecialId(act_no))
			plan->outcome = 3;
		else if (plan->presentation_flags != 0)
		{
			if (unit->class_index == 0)
				plan->outcome = 8;
			else
				plan->outcome = 3;
		}
		else
			plan->outcome = 1;
		if (BtlEfxRevChkFar(act->efx))
			plan->presentation_flags |= 0x10000;
		if (act_no == 178 && plan->tgt_hit[0] != 0)
			plan->presentation_flags |= 0x1000;
	}
	if (req->command == CMD_ITEM && plan->outcome != 5 && plan->outcome != 9)
		plan->outcome = 4;
	plan->cmd = req->command;
	return 0;
}
