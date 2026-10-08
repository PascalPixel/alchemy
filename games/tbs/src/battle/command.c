//--------------------------------------------------------------------------
// command.c
// バトル：行動の宣言と、対象・効果の計画
//--------------------------------------------------------------------------

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
#include "ownerval.h"
#include "btlev.h"
#include "system.h"
#include "battle_unit.h"
#include "battle_command.h"
#include "btlsys.h"
#include "btlsmn.h"
#include "btlpty.h"
#include "btlcalc.h"
#include "btlmes.h"
#include "motobj.h"
#include "item.h"
#include "io_reg.h"
s32 GameFlag_TestFar(s32 flag);
struct BtlAct * BtlActGet(s32 action_id);
struct ItemDefinition *Item_GetDirectFar(s32 item);

#define Battle_GetTaggedSlotValue ((s32 (*)(s32))Battle_GetTaggedSlotValue)
#define Trade_AddOfferFar ((void (*)(s32, s32, s32))Trade_AddOfferFar)
#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(struct MotionObject *, s32))ObjectDispatch_ApplyValueToChildrenFar)

// ジンの指定：属性 (bit 8-11) と番号 (bit 0-7)
#define DJINN_ELEMENT(v) (((v) >> 8) & 15)
#define DJINN_INDEX(v) ((v) & 255)

// 行動の種類
#define CMD_KOUGEKI 0 // こうげき
#define CMD_ENA 1	  // エナジー
#define CMD_ITEM 2	  // アイテム
#define CMD_BOUGYO 3  // ぼうぎょ
#define CMD_WAZA 4	  // 敵のわざ
#define CMD_DJINN 5	  // ジン
#define CMD_SHOUKAN 6 // しょうかん
#define CMD_BOUGYO2 7 // ぼうぎょ (効果 47)
#define CMD_NONE 8	  // 何もしない
#define CMD_NIGERU 99 // にげる

#define WAZA_KOUGEKI 1 // わざ番号：こうげき
#define MIKATA_MAX 7   // 仲間の番号の上限
#define TGT_MAX 14

void					   UiWork_PushValueSlotFar(s32 value, s32 slot);
void					   UiText_ShowMessageAndWaitCoreFar(s32 message);
void					   Djinn_ActivateFar(s32 unit_id, s32 element, s32 index);
void					   Trade_RemoveOfferFar(s32 unit_id, s32 element, s32 index);
s32						   BattleEventRuntime_SchedulePhase(s32 frames);
void					   AudioCommand_PlayFar(s32 cue);
void					   ObjectDispatch_ApplyArgumentToChildrenFar(struct MotionObject *object, s32 mode);
struct DjinnRecoveryTable *Trade_GetOfferStateFar(s32 side);
void					   BattlePresentation_WaitForAdvance(void);

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
				ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(req->unit_id)->object, 32);
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
		list = &Trade_GetOfferStateFar(side)->list;
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
