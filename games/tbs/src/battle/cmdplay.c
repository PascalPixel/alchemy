//--------------------------------------------------------------------------
// cmdplay.c
// バトル：敵の行動の選択と、バトルイベントの再生
//--------------------------------------------------------------------------

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
#include "dma.h"
#include "btlwork.h"
#include "types.h"
#include "battle_command.h"
#include "btlsys.h"
#include "btltype.h"
#include "character.h"
#include "io_reg.h"
#include "callback_scheduler.h"
#include "battle_status_icon.h"
#include "iwram_call.h"
#include "irq.h"
#include "vram_block.h"
#include "btlev.h"
#include "animspr.h"
#include "motobj.h"
#include "fixed_math.h"
#include "io_write_queue.h"
#include "resource.h"
#include "ui.h"
#include "menu_list.h"
#include "btlpty.h"

void				   UiWork_PushValueSlotFar(s32, s32);
struct BtlAct		  *BtlActGet(s32 action_id);
struct ItemDefinition *Item_GetDirectFar(s32 item);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

#define Battle_FindTaggedSlotByValue ((s32 (*)(s32))Battle_FindTaggedSlotByValue)

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
	register u32				 sel;
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

#define KEY_OKURI (KEY_A | KEY_B | KEYS_SHOULDERS) // 送りのキー

#define POSE_DAMAGE 5 // ダメージを受けたポーズ
#define PAL_HIT 7	  // 点滅の色

#define TAORE_FLASH 0x400 // 色を変えている間のタイマーの基準

#define OBJ_VRAM 0x06010000

extern volatile u32 gFrameCount;
extern volatile s32 gFrameTick;

void ObjectDispatch_ApplyArgumentToChildrenFar(void *object, s32 mode);

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
		Iwram_ClearWords(
			(void *)(OBJ_VRAM + gVramBlockCache[rec->slot].offset), rec->width * rec->height);
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
