#include "unit/battle/effect/fxscales.h"
#include "unit/battle/effect/koukabit.h"
#include "unit/battle/effect/koukatbl.h"
#include "unit/battle/effect/effect.h"
#include "unit/battle/effect/effect2.h"
#include "unit/battle/effect/effect3.h"
#include "unit/battle/effect/effect4.h"
#include "unit/battle/effect/effect5.h"
#include "unit/battle/effect/effect6.h"
#include "unit/battle/effect/effect7.h"
#include "unit/battle/effect/effect8.h"
#include "unit/battle/effect/effect9.h"
#include "unit/battle/effect/effect10.h"
#include "unit/battle/effect/effect11.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object4.h"
#include "unit/battle/effect/object5.h"
#include "unit/battle/effect/object6.h"
#include "unit/battle/effect/object7.h"
#include "unit/battle/effect/object8.h"
#include "unit/battle/effect/object9.h"
#include "unit/battle/effect/object10.h"
#include "unit/battle/effect/object11.h"
#include "unit/battle/effect/object12.h"
#include "unit/battle/effect/object13.h"
#include "unit/battle/effect/object14.h"
#include "unit/battle/effect/object15.h"
#include "unit/battle/effect/object16.h"
#include "unit/battle/effect/object17.h"
#include "unit/battle/effect/object18.h"
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
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "canvas.h"
#include "map_scroll.h"
#include "heap_state.h"
#include "menu_list.h"
#include "types.h"
#include "io_write_queue.h"
#include "io_reg.h"
#include "scene.h"
#include "global_cells.h"
#include "dma.h"
#include "resource_ids.h"
#include "battle_efx.h"
#include "fixed_math.h"
#include "callback_scheduler.h"
#include "effect_step.h"
#include "battle_effect_work.h"
#include "btldisp.h"
#include "ram_buffer.h"
#include "resource.h"
#include "b5_context.h"
#include "motobj.h"
#include "iwram_call.h"
#include "transform.h"
#include "btlwork.h"
#include "project.h"
#include "field_event.h"
#include "keystate.h"
#include "btltype.h"
#include "sound_ids.h"
#include "call.h"
#include "btlsys.h"
#include "battle_session.h"
#include "animspr.h"
#include "battle_unit.h"
#include "fixed_point_position.h"
#include "irq.h"

void Camera_ApplyPhasedDelta(void)
{
	struct BattleEffectWork *work = *(struct BattleEffectWork **)gBattleFxWork;
	struct BattleCamera		*camera = *(struct BattleCamera **)((u8 *)gBattleFxWork - 108);
	s32						*phase = &work->camera_phase;

	if (*phase == 1)
	{
		camera->yaw += work->camera_delta;
		*phase = 0;
	}
	else
	{
		camera->yaw += work->camera_delta / 2;
		if (*phase == 2)
			*phase = 0;
		else
			*phase = 2;
	}
}

//--------------------------------------------------------------------------
// run_no_effect_frames.c
// バトル演出：空の演出の繰り返し
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// 何もしない BattleFx_RunNoEffect を cnt 回呼ぶ（フレーム待ちはしない）
// cnt : 呼ぶ回数
//--------------------------------------------------------------------------
void BattleFx_RunNoEffectFrames(s32 cnt)
{
	s32 n;

	n = 0;
	if (cnt != 0)
	{
		do
		{
			n += 1;
			BattleFx_RunNoEffect();
		} while (n != cnt);
	}
}

//--------------------------------------------------------------------------
// dispatch.c
// バトル演出：演出の振り分け・走査線ごとのウインドウ端・生きている対象
//--------------------------------------------------------------------------

typedef void (*Callback)(s32 *);
extern Callback BattleFx_ModeHandlers[];

//--------------------------------------------------------------------------
// 演出の作業領域を取り、種類（arg[0]）ごとの演出を流して返す
// arg : struct BattleEffectArgument
//--------------------------------------------------------------------------
void BattleFx_InitializeMode(s32 *arg)
{
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

	if (*arg == 0)
	{
		BattleFx_InitializeDefaultMode((struct BattleEffectArgument *)arg);
	}
	else
	{
		switch (*arg)
		{
		case 1:
			BattleEffect_RunCirclingFallingScene(arg);
			break;
		case 2:
			BattleEffect_RunEmberColumns(arg);
			break;
		case 3:
			BattleFx_RunObjectRow(arg);
			break;
		case 4:
			BattleEffect_RunStagedParticles(arg);
			break;
		case 5:
			BattleEffect_RunDitherDissolveScene((struct BattleEffectArgument *)arg);
			break;
		case 6:
			BattleFx_InitializeMode6(arg);
			break;
		case 7:
			BattleFx_RenderAnimationMode0(arg);
			break;
		case 8:
			BattleFx_RunWhirlwindComets((struct BattleEffectArgument *)arg);
			break;
		case 9:
			BattleEffect_RunShockwaveScene((struct BattleEffectArgument *)arg);
			break;
		case 10:
			BattleFx_InitializeMode10(arg);
			break;
		case 11:
			BattleFx_InitializeDefaultMode((struct BattleEffectArgument *)arg);
			break;
		case 12:
			BattleFx_InitializeMode12(arg);
			break;
		}
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

//--------------------------------------------------------------------------
// 演出の作業領域を取り、BattleFx_ModeHandlers の表から演出を流す
// 種類 0 は演出なしで、variant（arg[6]）を 0 にする
// arg : struct BattleEffectArgument
//--------------------------------------------------------------------------
void BattleFx_DispatchMode(s32 *arg)
{
	s32						 kind;
	struct BattleEffectWork *work;

	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_SHEET, 0x302);
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_EFFECT, sizeof(struct BattleEffectWork));
	Runtime_AllocateHeapBlock(HEAP_SLOT_BATTLE_CANVAS, 0x4000);

	work = *(struct BattleEffectWork **)gBattleFxWork;
	work->effect = (struct BattleEffectArgument *)arg;
	kind = arg[0];
	if (kind == 0)
		arg[6] = 0;
	else
		BattleFx_ModeHandlers[kind - 1](arg);

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_CANVAS);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_EFFECT);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BATTLE_SHEET);
}

//--------------------------------------------------------------------------
// ウインドウ 0 の右端を走査線ごとに決め、HBlank DMA で毎ライン書き込む
// （スケジューラのコールバック）
// gMapCellBuffer +0   : 基準の右端（u16）
// gMapCellBuffer +2   : 8〜135 ライン目のずれ（u8 × 128）
// gMapCellBuffer +0x82: 作った WIN0H の表（u16 × 160）
// 表の外のラインは 0xfff1（左端 > 右端）で、ウインドウに入らない
//--------------------------------------------------------------------------
void BattleFx_BuildWindowEdgeTable(void)
{
	u16 *base;
	u16 *tbl;
	u8	*ofs;
	s32	 i;
	s32	 x;

	base = (u16 *)gMapCellBuffer;
	tbl = (u16 *)(Ram_MapCellBuffer + 0x82);
	ofs = Ram_MapCellBuffer + 2;
	for (i = 0; i != 160; i++)
	{
		if (i >= 8 && i <= 135)
		{
			x = *base - ofs[i - 8];
			if (x < 0)
				x = 0;
			if (x > 240)
				x = 240;
			tbl[i] = x;
		}
		else
		{
			tbl[i] = 0xfff1;
		}
	}
	{
		volatile u16 *channel = REG_DMA0;

		// DMA0 のリピートと開始条件を消してから止める
		channel[5] &= 0xc5ff;
		channel[5] &= 0x7fff;
		(void)channel[5];
		// HBlank ごと・リピート・転送先リロードで 1 ハーフワード
		Dma_Set(
			(u8 *)gMapCellBuffer + 0x82, (void *)&REG_WIN0H, 0xa2600001, (volatile u32 *)channel);
	}
}

//--------------------------------------------------------------------------
// 対象の側（敵か味方か）で HP の残っている者を集め、オブジェクトを出す
// 敵の番号は 128 から
//--------------------------------------------------------------------------
void BattleFx_SelectLivingTargets(struct BattleEffectArgument *arg)
{
	s16 list[14];
	s32 cnt;
	s32 i;

	cnt = 0;
	if (arg->actors[0] > 127)
	{
		for (i = 0; i != 6; i++)
		{
			s32 id = i + 128;

			if (UnitGetFar(id)->hp > 0)
				list[cnt++] = id;
		}
	}
	else
	{
		for (i = 0; i != 8; i++)
		{
			if (UnitGetFar(i)->hp > 0)
				list[cnt++] = i;
		}
	}
	list[cnt] = 0xff; // 終わり
	BattleActor_SpawnObjectsForListFar(list, 0);
}

void BattleEffect_SetupBlendedDisplay(void)
{
	void		  **slots = &gWorkSlot[HEAP_SLOT_BATTLE];
	u8			   *buffer = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
	struct BtlWork *battle = slots[0];
	volatile u32	zero;

	REG_DISPCNT = 1;
	gBgScroll[1].y = 32;
	BattleBackground_LoadFar(1, battle->background, 24);
	zero = 0;
	Dma_Set((const void *)&zero, buffer, 0x85001000, REG_DMA3);
	zero = 0;
	Dma_Set((const void *)&zero, (void *)0x06004000, 0x85001000, REG_DMA3);
	WaitFrames(1);
	REG_BLDALPHA = 0x100e;
	REG_BLDCNT = 0x3f46;
	REG_DISPCNT = 0x7741;
	gProjection.center_y = 120;
}

void ObjectGroup_UpdateMembers(
	s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value)
{
	struct BtlObj			*set;
	struct AnimationObject	*group;
	struct BattleEffectWork *state;
	s32						 group_index;

	set = GetBattleObjectSlotFar(set_id);
	state = *(struct BattleEffectWork **)gBattleFxWork;
	group_index = 0;

	while ((group = GetMotionRecordFar(set->object, group_index)) != NULL)
	{
		if (state_slot != -1)
		{
			state->actor_timers[state_slot] = ((u8 *)&state_value)[0];
		}
		if (set->fading == 0)
		{
			if (object_value != -1)
			{
				s32 object_index;

				object_index = 0;
				if (group->count != 0)
				{
					struct AnimationEntry **objects;

					objects = group->entries;
					do
					{
						struct AnimationEntry *object;

						object = *objects++;
						if (object != NULL && object != (struct AnimationEntry *)set->effect_entry
							&& object != (struct AnimationEntry *)set->animation_entry)
						{
							if (object_value == 0)
								object->param = BattleMotion_GetSlotPaletteFar(set_id);
							else
								object->param = object_value;
							object->frame = 0xff;
						}
						object_index++;
					} while (object_index != group->count);
				}
			}
			if (group_value != -1)
				AnimationObjects_SelectAnimationFar(group, group_value);
		}
		group_index++;
	}
}

void BattleFx_InitTilemapAndFlushQueue(void)
{
	BattleFx_BeginTiledCanvas(1);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// initialize_mode10.c
// 戦闘エフェクト：いばらのつる、岩と氷塊、氷の破片
//--------------------------------------------------------------------------
