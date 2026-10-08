//--------------------------------------------------------------------------
// BATTLE.C
// バトル：キャラの表示の優先度、状態アイコン、ポーズ
//--------------------------------------------------------------------------

#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "BTLPTY.H"
#include "BATTLE_STATUS_ICON.H"
#include "BTLSYS.H"
#include "MOTOBJ.H"
#include "ANIMSPR.H"
#include "BTLDISP.H"
#include "FIELD_EVENT.H"

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(struct MotionObject *, s32))ObjectDispatch_ApplyValueToChildrenFar)

#define UNIT_MAX 14 // 敵味方を合わせた人数

//--------------------------------------------------------------------------
// 状態アイコンを進め、カメラの向きで味方と敵の表示の優先度を決める
//--------------------------------------------------------------------------
void BattleActor_UpdateSpritePriorities(void)
{
	u16						 ids[UNIT_MAX];
	s32						 pri[2]; // 味方, 敵
	s32						 i;
	s32						 j;
	s32						 count;
	struct MotionObject		*obj;
	struct AnimationObject	*rec;
	struct AnimationObject **list;

	// 全員の状態アイコン
	BattleParty_ListActorIds(BATTLE_SIDE_BOTH, ids);
	/* FAKEMATCH: needed to match: the scan is rotated by hand with a goto, which keeps the loop pass off it */
	i = 0;
	if (ids[i] != BATTLE_UNIT_LIST_END)
	{
	again:
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				BattleStatusIcon_Cycle(slot);
				if (slot->effect_entry != 0)
				{
					rec = GetMotionRecord(obj, 0);

					if (rec != 0)
					{
						struct SpriteEntry *efx;
						s32					p = 0;

						if (obj->y != 0) // 高さがあれば
							p = 9;
						efx = slot->effect_entry;
						if (efx->priority != p)
						{
							efx->priority = p;
							rec->dirty = 1;
						}
					}
				}
			}
		}
		i++;
		if (i <= UNIT_MAX - 1 && ids[i] != BATTLE_UNIT_LIST_END)
			goto again;
	}

	// カメラの向きで、味方と敵の優先度を入れ替える
	if ((s16)gCameraWork->yaw >= 0)
	{
		pri[0] = 1;
		pri[1] = 2;
	}
	else
	{
		pri[0] = 2;
		pri[1] = 1;
	}

	// 味方
	{
		count = BattleParty_ListActorIds(BATTLE_SIDE_PARTY, ids);
		for (i = 0; i < count; i++)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->record_storage_kind & 15)
				{
				case 1: // 一枚
				{
					rec = obj->records;
					rec->part[0].priority = pri[0];
					break;
				}
				case 2: // 四枚までの表
				{
					list = (struct AnimationObject **)obj->records;
					for (j = 0; j < 4; j++)
					{
						rec = *list++;
						if (rec != 0)
							rec->part[0].priority = pri[0];
					}
					break;
				}
				}
			}
		}
	}

	// 敵
	{
		count = BattleParty_ListActorIds(BATTLE_SIDE_ENEMIES, ids);
		for (i = 0; i < count; i++)
		{
			struct BtlObj *slot = BtlObjGet(ids[i]);

			if (slot != 0)
			{
				obj = slot->object;

				switch (obj->record_storage_kind & 15)
				{
				case 1:
				{
					rec = obj->records;
					rec->part[0].priority = pri[1];
					break;
				}
				case 2:
				{
					list = (struct AnimationObject **)obj->records;
					for (j = 0; j < 4; j++)
					{
						rec = *list++;
						if (rec != 0)
							rec->part[0].priority = pri[1];
					}
					break;
				}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------
// 状態アイコンのビット
//--------------------------------------------------------------------------
#define ICON_DOKU 0x01		// 毒
#define ICON_MOUDOKU 0x02	// もう毒
#define ICON_SHIBIRE 0x04	// しびれ
#define ICON_FUUJI 0x08		// エナジーふうじ
#define ICON_NEMURI 0x10	// ねむり
#define ICON_MABOROSHI 0x20 // まぼろし
#define ICON_AKURYO 0x40	// あくりょう
#define ICON_SHIRYO 6		// しりょう：残りの数 1-7 が bit 7-13

#define ICON_MAX 14 // アイコンの種類

//--------------------------------------------------------------------------
// 状態異常から状態アイコンのビットを作る
//--------------------------------------------------------------------------
s32 BattleUnit_BuildStatusFlags(s32 id, struct BtlObj *slot)
{
	struct BtlUnit *unit = UnitGetFar(id);
	s8				doku = unit->doku;
	u32				flags = 0;

	if (doku == 1)
		flags = ICON_DOKU;
	if (doku == 2)
		flags |= doku; // もう毒 (ICON_MOUDOKU)
	if (unit->maboroshi != 0)
		flags |= ICON_MABOROSHI;
	if (unit->shibire != 0)
	{
		s32 cls;
		flags |= ICON_SHIBIRE;
		cls = unit->class_id;
		if (cls == 121 || cls == 148) // この二体には出さない
			flags &= ~ICON_SHIBIRE;
	}
	if (unit->fuuji != 0)
		flags |= ICON_FUUJI;
	if (unit->akuryo != 0)
		flags |= ICON_AKURYO;
	if (unit->nemuri != 0)
		flags |= ICON_NEMURI;
	if (unit->shiryo != 0)
		flags |= 1 << (unit->shiryo + ICON_SHIRYO);
	slot->active_conditions = flags;
}

//--------------------------------------------------------------------------
// 状態アイコンを 80 フレームごとに次の状態へ切り替える
//--------------------------------------------------------------------------
s32 BattleStatusIcon_Cycle(struct BtlObj *slot)
{
	struct AnimationEntry  *old;
	struct AnimationEntry  *icon;
	struct AnimationObject *rec;
	struct MotionObject	   *owner;
	s32						anim;
	s32						handle;
	s32						prev;
	s32						changed = 0;

	if (slot->cycle_timer >= 0)
		slot->cycle_timer--;

	// 表示中の状態が消えたら、すぐに切り替える
	old = (struct AnimationEntry *)slot->animation_entry;
	if (old == 0)
	{
		if ((s16)slot->active_conditions == 0)
			goto wait;
		goto update;
	}
	if ((((s16)slot->active_conditions >> slot->selected_condition) & 1) == 0)
		goto update;
wait:
	if (slot->cycle_timer != 0)
		goto done;

update:
	anim = -1;
	owner = slot->object;
	if ((s16)slot->active_conditions != 0)
	{
		prev = slot->selected_condition;
		for (anim = prev + 1;; anim++)
		{
			if (anim > ICON_MAX - 1)
				anim = 0;
			if ((((s16)slot->active_conditions >> anim) & 1) != 0)
				break;
		}

		if (prev != anim || old == 0)
		{
			slot->selected_condition = anim;
			changed = 1;
		}
		slot->cycle_timer = 80;
	}
	else
	{
		changed = 1;
	}

	rec = GetMotionRecord(owner, 0);
	if (rec == 0)
		goto done;

	// アイコンのアニメ番号は、幅 32 のキャラと他とで別の並び
	if (anim >= 0)
	{
		if (rec->width == 32)
			anim += 340;
		else
			anim += 355;
	}

	if (slot->animation_entry != 0 && changed != 0)
	{
		ResourceMetadata_UnregisterFar(rec, (s32)slot->animation_entry);
		slot->animation_entry = 0;
	}

	if (anim >= 0 && changed != 0)
	{
		/* FAKEMATCH: needed to match: storing the handle before testing failure avoids an extra unconditional branch */
		handle = ResourceMetadata_RegisterFar(rec, anim);
		slot->animation_entry = (struct SpriteEntry *)handle;
		if (handle == -1)
			slot->animation_entry = 0;
		icon = (struct AnimationEntry *)slot->animation_entry;
		if (icon != 0)
		{
			icon->priority = 3;
			Animation_SetWorkEntryFar(icon, 0);
		}
	}

	rec->dirty = 1;
	if (anim >= 0)
		slot->animation = anim;
	else
		slot->animation = 0;

done:;
}

//--------------------------------------------------------------------------
// 状態に合わせてキャラのポーズを決める
// 動けなければ味方 4・敵 0、倒れていれば味方 4・敵 5、ほかは 1
//--------------------------------------------------------------------------
s32 BattlePres_SetActorModeAndAction(s32 id)
{
	struct BtlUnit *unit;
	s32				mode;

	unit = UnitGetFar(id);
	mode = 1;
	if (unit->hp != 0)
	{
		if (unit->nemuri != 0 || unit->shibire != 0 || unit->miugoki != 0)
			mode = (unit->side != BATTLE_UNIT_ENEMY) * 4;
	}
	else
	{
		s32 mikata = unit->side ^ mode;
		mode = (u32)(-mikata | mikata) >> 31;
		mode = 5 - mode;
	}

	Object_SetMode(BtlObjGet(id)->object, mode);
	ObjectDispatch_ApplyValueToChildrenFar(BtlObjGet(id)->object, (id & 3) + 14);
}
