//--------------------------------------------------------------------------
// dispatch.c
// バトル演出：演出の振り分け・走査線ごとのウインドウ端・生きている対象
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
#include "mem.h"
#include "heap_state.h"
#include "canvas.h"
#include "battle_effect_work.h"
#include "types.h"
#include "battle_efx.h"
#include "global_cells.h"
#include "dma.h"
#include "ram_buffer.h"
#include "btlsys.h"
#include "btltype.h"
#include "io_reg.h"
#include "project.h"
#include "btlwork.h"
#include "motobj.h"
#include "battle_session.h"
#include "animspr.h"
#include "btldisp.h"
#include "resource.h"
#include "iwram_call.h"
#include "callback_scheduler.h"
#include "fixed_math.h"
#include "resource_ids.h"
#include "effect_step.h"
#include "transform.h"

#define BattleEffect_RunCirclingFallingScene ((void (*)(s32 *))BattleEffect_RunCirclingFallingScene)
#define BattleEffect_RunEmberColumns ((void (*)(s32 *))BattleEffect_RunEmberColumns)
#define BattleFx_RunObjectRow ((void (*)(s32 *))BattleFx_RunObjectRow)
#define BattleEffect_RunStagedParticles ((void (*)(s32 *))BattleEffect_RunStagedParticles)
#define BattleFx_InitializeMode6 ((void (*)(s32 *))BattleFx_InitializeMode6)
#define BattleFx_InitializeMode10 ((void (*)(s32 *))BattleFx_InitializeMode10)
#define BattleFx_InitializeMode12 ((void (*)(s32 *))BattleFx_InitializeMode12)

typedef void (*Callback)(s32 *);
extern Callback BattleFx_ModeHandlers[];

extern u8 gMapCellBuffer[];

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
		Dma_Set(gMapCellBuffer + 0x82, (void *)&REG_WIN0H, 0xa2600001, (volatile u32 *)channel);
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

void WaitFrames(s32 frames);

void BattleEffect_SetupBlendedDisplay(void)
{
	extern u16		gBgScroll[];
	void		  **slots = &gWorkSlot[HEAP_SLOT_BATTLE];
	u8			   *buffer = slots[HEAP_SLOT_BATTLE_CANVAS - HEAP_SLOT_BATTLE];
	struct BtlWork *battle = slots[0];
	volatile u32	zero;

	REG_DISPCNT = 1;
	gBgScroll[3] = 32;
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

#define GetMotionRecordFar ((void *(*)(void *, s32))GetMotionRecordFar)

#define gBattleFxWork (*(struct BattleEffectWork **)&gBattleFxWork)

void ObjectGroup_UpdateMembers(
	s32 set_id, s32 object_value, s32 group_value, s32 state_slot, s32 state_value)
{
	struct BtlObj			*set;
	struct AnimationObject	*group;
	struct BattleEffectWork *state;
	s32						 group_index;

	set = GetBattleObjectSlotFar(set_id);
	state = gBattleFxWork;
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

#undef BattleFx_InitializeMode10
#undef GetMotionRecordFar
#undef gBattleFxWork

s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *state, s32 flags);

void AudioCommand_PlayFar(s32 cue);

#define HI(v) (((s16 *)&(v))[1]) // 16.16 固定小数の整数部

//--------------------------------------------------------------------------
// いばらのつる : とげを飛ばし、つるを伸ばして対象を持ち上げ、
// 絵を点ごとに消してから対象を落として爆発させる
// effect : 対象の一覧
//--------------------------------------------------------------------------
void BattleEffect_RunDitherDissolveScene(struct BattleEffectArgument *effect)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	u8						*canvas;
	s32						 frame;
	s32						 height;
	s32						 speed;
	s32						 phase;
	s32						 rise;
	s32						 i;
	s32						 j;
	u8						*order;
	s32						 point[3];
	struct EffectPosition	 screen;
	DrawRectangle			 draw[2];

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_SpawnObjects(8, 0x177, 1);

	// 消す順番 : 8 組の 0〜127 をでたらめに並べかえる
	order = Ram_MapCellBuffer;
	for (i = 0; i != 1024; i++)
		order[i] = i & 127;
	for (i = 0; i != 8; i++)
	{
		for (j = 0; j != 128; j++)
		{
			s32 a = Random16() & 127;
			s32 b = Random16() & 127;
			u8 *to = &order[i * 128 + b];
			u8 *from = &order[i * 128 + a];
			u8	t = *to;

			*to = *from;
			*from = t;
		}
	}

	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	REG_BLDCNT = 0;
	Resource_LoadAndDecompress((s32)&ResourceId_ThornVineSheet, work->sheet, 1, 1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 1);
	draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 1);
	draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
	work->transfer_mode = 1;
	work->transfer_value = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_BG2X = -0x2000; // 左へ 32 ドット (20.8 固定小数)

	// つる (0〜15) : x、根もとの y、伸びる速さ、伸びはじめるまでの待ち
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *vine = &work->particles[i];

		if (i <= 4)
		{
			vine->x = i * 20;
			vine->y = (Random16() & 7) + 104;
		}
		else
		{
			vine->x = i * 20 - 90;
			vine->y = (Random16() & 7) + 108;
		}
		vine->velocity_y = (Random16() & 7) + 4;
		vine->variant = (Random16() & 15) + 16;
	}
	// 光 (16〜31) : 負の variant は出るまでの待ち
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *spark = &work->particles[16 + i];

		spark->x = (Random16() & 63) + 32;
		spark->y = (Random16() & 31) + 64;
		spark->variant = -(Random16() & 7) - 8;
	}
	// とげ (32〜47) : 右上から左へ飛ぶ
	for (i = 0; i != 16; i++)
	{
		struct EffectStep *thorn = &work->particles[32 + i];

		thorn->x = 128 << 16;
		thorn->y = 64 << 16;
		thorn->velocity_x = -((Random16() & 255) + 200) << 9;
		thorn->velocity_y = 0;
		thorn->variant = 0;
	}
	BattleFx_SelectLivingTargets(work->effect);

	height = -(64 << 16);
	speed = 0;
	for (frame = 0; frame != 366; frame++)
	{
		struct BattleCamera *camera = gCameraWork;

		// A・B で、つるの場面を落とす場面まで飛ばす
		if ((gKeysRepeat & (KEY_A | KEY_B)) != 0 && frame > 190 && frame <= 285)
		{
			Iwram_ClearWords(canvas, 0x4000);
			frame = 286;
		}
		if (frame == 224)
			work->transfer_mode = 0;
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 31)
		{
			work->shake_frames = 8;
			AudioCommand_PlayFar(157);
			for (i = 0; i != work->effect->count; i++)
				BattleMotion_ApplyVariantMotionFar(work->effect->actors[i], 6);
		}
		if (frame == 72)
			AudioCommand_PlayFar(136);
		if (frame == 140)
			AudioCommand_PlayFar(156);

		// 陣形のオブジェクトを 64 ドットの高さまで落とす
		speed += 0x4000;
		height += speed;
		if (height > 64 << 16)
			height = 64 << 16;
		BattleFx_PlaceFormationObjects(2, 128 << 16, height);
		if (frame >= 48 && frame <= 96)
		{
			s32 pose = (frame - 48) / 24 % 3;

			AnimationObjects_SelectAnimationFar(work->objects[3], ThornVines_ObjectPoses[pose * 2]);
			AnimationObjects_SelectAnimationFar(
				work->objects[4], ThornVines_ObjectPoses[pose * 2 + 1]);
		}

		// とげを 1 フレームに 1 本ずつ飛ばす
		if (frame >= 72 && frame <= 127)
		{
			for (i = 0; i != 16; i++)
			{
				struct EffectStep *thorn = &work->particles[32 + i];

				if (frame >= i + 72 && thorn->y < (104 << 16))
				{
					s32 cel = (frame + i) / 4 % 5;
					s32 x = HI(thorn->x);
					s32 y = thorn->y >> 16;
					u8 *cell = work->sheet + ThornVines_ThornOffsets[cel];
					u32 width = ThornVines_ThornWidths[cel];
					u32 tall;

					x -= width >> 1;
					tall = ThornVines_ThornHeights[cel];
					y -= tall >> 1;
					draw[0](canvas, cell, x, y, width, tall);
					EffectStep_AdvanceWithGravity2D(thorn, 64, 0x1000);
				}
			}
		}
		// 破片 (16〜63) を地面から跳ねあげる
		if (frame == 128)
		{
			for (i = 0; i != 48; i++)
			{
				struct EffectStep *chip = &work->particles[16 + i];

				chip->x = (Random16() % 96) << 16;
				chip->y = ((Random16() & 7) + 88) << 16;
				chip->velocity_x = ((Random16() & 255) - 128) << 11;
				chip->velocity_y = -(Random16() & 255) << 11;
				chip->variant = -(Random16() & 15) - 16;
			}
			REG_BG2X = 0;
		}

		// つるを伸ばす (伸びているあいだは画面を揺らす)
		phase = frame - 128;
		if (phase >= 0 && phase <= 96)
		{
			rise = phase;
			if (rise > 80)
				rise = 80;
			else
				work->shake_frames = 2;
			for (i = 0; i != 10; i++)
			{
				struct EffectStep *vine = &work->particles[i];

				if (rise > vine->variant)
				{
					u8 *sheet = work->sheet;
					s32 top;
					s32 wrap;
					s32 k;

					if (i > 5)
						sheet = work->sheet + 0x6c0;
					top = (rise - vine->variant) * vine->velocity_y;
					for (wrap = top; wrap > 184; wrap -= 64)
						;
					// 先端 (24×8)
					if (wrap <= 119)
						draw[i & 1](canvas, sheet, vine->x, vine->y - wrap - 8, 24, 8);
					// 茎 (24×64 を 3 枚、画面の上と根もとで切る)
					for (k = 0; k != 3; k++)
					{
						s32 y = vine->y - wrap + k * 64;
						s32 skip = 0;
						s32 rows = 64;

						if (y >= -64)
						{
							if (y < 0)
							{
								skip = -y * 24;
								rows = y + 64;
								y = 0;
							}
							if (y + rows > vine->y)
								rows -= y + rows - vine->y;
							draw[i & 1](canvas, sheet + skip + 192, vine->x, y, 24, rows);
						}
					}
					// 葉 (奇数のつるだけ)
					if ((i & 1) != 0)
					{
						s32 y = ((vine->y - top) & 127) - 16;
						s32 kind = i % 3;
						s32 full = ThornVines_LeafSizes[kind];
						s32 rows = full;

						if (y + rows > vine->y)
							rows -= y + rows - vine->y;
						if (rows > 0)
							draw[i & 1](canvas, work->sheet + ThornVines_LeafOffsets[kind],
								vine->x + 8, y, full, rows);
					}
				}
			}
		}
		// 破片 : 落ちるか画面の外へ出たら、フレーム 159 まで跳ねあげなおす
		if (phase >= 0 && phase <= 95)
		{
			for (i = 0; i != 32; i++)
			{
				struct EffectStep *chip = &work->particles[32 + i];

				if (chip->variant >= 0)
				{
					s32 cel = i % 5;

					draw[0](canvas, work->sheet + ThornVines_ChipOffsets[cel], HI(chip->x),
						HI(chip->y), ThornVines_ChipWidths[cel], ThornVines_ChipHeights[cel]);
					chip->x += chip->velocity_x;
					chip->y += chip->velocity_y;
					chip->velocity_y += 0x4000;
					if ((chip->y < 0 || chip->y > 120 << 16) && frame <= 159)
					{
						chip->x = (Random16() % 96) << 16;
						chip->y = ((Random16() & 7) + 88) << 16;
						chip->velocity_x = ((Random16() & 255) - 128) << 11;
						chip->velocity_y = -(Random16() & 255) << 11;
					}
				}
				chip->variant++;
			}
		}

		// BG パレットの 64 色を 4 フレームごとに灰色へ 1 段ずつ近づける
		if (frame >= 224 && frame <= 247 && (frame & 3) == 0)
		{
			u16 *color = (u16 *)BG_PLTT;

			for (i = 0; i != 64; i++)
			{
				s32 red = *color & 31;
				s32 green = (*color >> 5) & 31;
				s32 blue = (*color >> 10) & 31;
				s32 grey = (red + green + blue) / 3;

				if (red > grey)
					red--;
				if (red < grey)
					red++;
				if (green > grey)
					green--;
				if (green < grey)
					green++;
				if (blue > grey)
					blue--;
				if (blue < grey)
					blue++;
				*color++ = (blue << 10) | (green << 5) | red;
			}
		}

		// 対象ごとに 6 つの光を、体のまわりでくり返し光らせる
		if (phase >= 0 && phase <= 172)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				struct MotionObject *object =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				point[0] = object->x;
				point[1] = object->y;
				point[2] = object->z;
				EffectPosition_ApplyBaseAndYOffset(point, &screen);
				for (j = 0; j != 6; j++)
				{
					struct EffectStep *spark = &work->particles[16 + i * 6 + j];

					if (spark->variant == 0)
					{
						spark->x = screen.x + (Random16() & 15) - 8;
						spark->y = screen.y + (Random16() & 15) - 40;
					}
					if (spark->variant >= 0 && spark->variant <= 4)
					{
						u8 *cell = work->sheet + ThornVines_SparkOffsets[spark->variant];
						u32 size = ThornVines_SparkSizes[spark->variant];
						u32 half = size >> 1;

						draw[0](canvas, cell, spark->x - half, spark->y - half, size, size);
					}
					spark->variant++;
					if (frame <= 199 && spark->variant == 5)
						spark->variant = -(Random16() & 7);
				}
			}
		}

		// 128×128 の絵を、上から 2 行ずつ順番どおりの点で消していく
		if (frame > 232)
		{
			s32 row = frame * 2 - 496;

			for (i = 0; i != 32; i++)
			{
				for (j = 0; j != 4; j++)
				{
					s32 y = row + i;

					if (y >= 0 && y <= 127)
					{
						s32 x = order[((y & 7) * 32 + i) * 4 + j];

						canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
					}
				}
			}
			row++;
			for (i = 0; i != 32; i++)
			{
				for (j = 0; j != 4; j++)
				{
					s32 y = row + i;

					if (y >= 0 && y <= 127)
					{
						s32 x = order[((y & 7) * 32 + i) * 4 + j];

						canvas[((y / 8 * 16 + x / 8) * 8 + (y & 7)) * 8 + (x & 7)] = 0;
					}
				}
			}
		}

		// 対象を 8 フレームずつずらして 128 ドットの高さまで持ち上げる
		if (frame >= 161 && frame <= 223)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame > i * 8 + 160)
				{
					struct BtlObj		*slot = GetBattleObjectSlotFar(work->effect->actors[i]);
					struct MotionObject *object = slot->object;
					void				*record;
					s32					 n;

					object->y += 8 << 16;
					if (object->y > 128 << 16)
						object->y = 128 << 16;
					object->vertical_motion_strength = 0;
					for (n = 0; (record = GetMotionRecordFar(slot->object, n)) != 0; n++)
						AnimationObjects_SelectAnimationFar(record, 5);
				}
			}
		}
		// 5 フレームずつずらして 96 ドットから落とし、16 フレーム後にぶつける
		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 5 + 286)
			{
				struct MotionObject *object =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				object->y = 96 << 16;
				object->vertical_motion_strength = 0xab85;
			}
			if (frame == i * 5 + 302)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
				AudioCommand_PlayFar(134);
				work->shake_frames = 8;
			}
		}

		// 爆発の絵に切りかえ、対象ごとに火の粉を 72 度おきに 10 個出す
		if (frame == 302)
		{
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
			Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
			Resource_LoadAndDecompress((s32)&ResourceId_YellowRingSheet, work->sheet, 1, 0);
			Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work->sheet + 0x1680, 1, 1);
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
			draw[0] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
			BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 7, 2);
			draw[1] = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER_ALTERNATE];
			REG_BLDCNT = 0x3f46;
			REG_BG2PA = 0x80;
			REG_BG2X = 0;
			work->transfer_mode = 2;
			work->transfer_value = 75;
			Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
			for (i = 0; i != work->effect->count; i++)
			{
				struct MotionObject *object =
					GetBattleObjectSlotFar(work->effect->actors[i])->object;

				for (j = 0; j != 10; j++)
				{
					struct EffectStep *ember = &work->particles[i * 10 + j];

					ember->x = object->x;
					ember->y = 20 << 16;
					ember->z = object->z;
					ember->velocity_x = Trig_Sin(j * 0x3334) << 2;
					ember->velocity_y = (Random16() & 0x7fff) + (1 << 16);
					ember->velocity_z = Trig_Cos(j * 0x3334) << 2;
					ember->variant = 0;
				}
			}
		}
		// 爆発 (40×24 を左右に分けて 6 コマ) と、そのあとの火の粉 5 個
		if (frame > 301)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				s32 start = i * 4 + 302;

				if (frame >= start && frame < i * 4 + 314)
				{
					s32					 cel = (frame - start) / 2;
					struct MotionObject *object =
						GetBattleObjectSlotFar(work->effect->actors[i])->object;

					point[0] = object->x;
					point[1] = 0;
					point[2] = object->z;
					EffectPosition_ApplyBaseAndYOffset(point, &screen);
					screen.x >>= 1;
					draw[0](canvas, work->sheet + cel * 480, screen.x - 20, screen.y - 24, 20, 24);
					draw[1](canvas, work->sheet + cel * 480, screen.x, screen.y - 24, 20, 24);
				}
				if (frame >= start + 6)
				{
					for (j = 0; j != 5; j++)
					{
						struct EffectStep *ember = &work->particles[i * 10 + j];

						EffectPosition_ApplyBaseAndYOffset(&ember->x, &screen);
						screen.x >>= 1;
						if (ember->variant >= 0 && ember->variant <= 26)
						{
							s32 cel = 6;
							s32 offset = ThornVines_EmberOffsets[cel];
							u32 size = ThornVines_EmberSizes[cel];

							draw[0](canvas, work->sheet + offset, screen.x - (size >> 1),
								screen.y - (size >> 1), size, size);
						}
						EffectStep_AdvanceWithGravity2D(ember, 60, 0x1000);
						ember->variant++;
					}
				}
			}
		}

		if (frame <= 127)
			Camera_ApplyShake(4, 16);
		else if (frame <= 301)
			Camera_ApplyShake(2, 2);
		else
			Camera_ApplyShake(4, 8);
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleEventRuntime_BeginPhaseFar(134);
	BattleEffect_RunImpactBurst(2, 128 << 16, height);
	for (i = 0; i != 8; i++)
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 岩と氷塊
//--------------------------------------------------------------------------

#define AnimationObjects_SelectAnimationFar \
	((s32 (*)(void *, s32))AnimationObjects_SelectAnimationFar)
#define Camera_ApplyShake ((void (*)(s32, s32))Camera_ApplyShake)

void AudioCommand_PlayFar(s32);
void Object_ApplyProjectedPlacementFar(void *, void *, void *, s32);

typedef struct Scale
{
	s32 x;
	s32 y;
} Scale;

extern const Scale BattleFx10_UnitScale;

struct Mode10Work
{
	u8							 sheet[0x7080];
	struct EffectStep			 sparks[64]; // 落ちてくる氷塊、のちに飛び散る破片
	s32							 transfer_mode;
	s32							 transfer_value;
	u8							 unknown_7788[0x20];
	s32							 shake; // 画面を揺らすフレーム数
	u8							 unknown_77ac[0x2c];
	void						*objects[9];
	u8							 unknown_77fc[0x28];
	s32							 transfer_pending;
	struct BattleEffectArgument *effect;
};

#define PARTICLES ((struct EffectStep *)Ram_MapCellBuffer)
#define TRAILS ((struct EffectStep *)(gMapCellBuffer + 0xe00)) // PARTICLES[128] から

//--------------------------------------------------------------------------
// 岩と氷塊 : 窓を開いて岩を崩し、大きな氷塊を落としてしぶきを散らしたあと、
// 氷塊を降らせて対象に当て、大岩を崩して破片を跳ねさせる
// effect : 対象の一覧
//--------------------------------------------------------------------------
void BattleFx_InitializeMode10(struct BattleEffectArgument *effect)
{
	extern struct Cells03001ad0 gBgScroll;
	s32							pts[32][2];
	s32							pos[4];
	void					   *canvas;
	struct Mode10Work		   *work;
	s32							frame;
	DrawRectangle				draw;
	DrawRectangle				draw_alt;
	u8						   *dots;
	s32							step;
	s32							row;
	u32						   *heap;
	s32							count;
	s32							i;
	s32							j;
	s32							ox;
	s32							oy;
	s32							y;
	s32							t;
	struct EffectStep		   *p;
	struct EffectStep		   *q;
	Scale						scale;

	heap = (u32 *)&gWorkSlot[HEAP_SLOT_BATTLE_CANVAS];
	canvas = (void *)heap[0];
	work = (struct Mode10Work *)heap[HEAP_SLOT_BATTLE_EFFECT - HEAP_SLOT_BATTLE_CANVAS];
	dots = (u8 *)heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_CANVAS];
	step = 1;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	BattlePres_ConfigureEffectDisplay();
	PLTT_BACKDROP = 0;
	*(u16 *)0x05000002 = 0; // BG パレット 0 の色 1
	work->transfer_mode = 0;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	REG_WININ = 0x2137;
	BattleEffect_WipeCanvas(1, 0);
	REG_WIN0H = 0xf0f0; // 幅 0 の窓
	Resource_LoadAndDecompress((s32)&ResourceId_IceBlockSheet, work->sheet, 1, 1);
	Resource_LoadAndDecompress((s32)&ResourceId_SparkleDots, dots, 0, 0);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_CANVAS];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 3, 1);
	draw_alt = (DrawRectangle)heap[HEAP_SLOT_BLITTER_ALTERNATE - HEAP_SLOT_BATTLE_CANVAS];
	gProjection.center_y = 240;
	BattleFx_SelectLivingTargets(work->effect);
	WaitFrames(1);
	BattleBackground_LoadFar(1, (s32)&ResourceId_VioletSkyBackdrop, 0);
	BattleFx_SpawnObjects(9, 0x174, 1);
	REG_DISPCNT = 0x7741;
	REG_BG2PA = 0x80;
	REG_BLDALPHA = 0x1010;
	REG_BLDCNT = 0x3f44;
	work->transfer_mode = 2;
	work->transfer_value = 50;

	for (i = 0; i != 64; i++)
	{
		PARTICLES[i].x = (Random16() & 63) + 32;
		PARTICLES[i].y = (Random16() & 31) + 120;
		PARTICLES[i].variant = -1;
	}

	for (i = 0; i != 32; i++)
	{
		pts[i][0] = BattleFx10_Points[i][0];
		pts[i][1] = BattleFx10_Points[i][1];
	}

	AudioCommand_PlayFar(141);

	//------------------------------------------------------------------
	// 前半 : 岩、大きな氷塊、しぶき (A・B で後半へ)
	//------------------------------------------------------------------
	for (frame = 0; frame != 288; frame++)
	{
		count = 16;
		if ((gKeysRepeat & (KEY_A | KEY_B)) && frame > 16)
		{
			break;
		}

		// 窓を左から開く : 右端を加速しながら動かし、各行を 0〜63 ドット引っこめる
		if (frame >= 0 && frame < 16)
		{
			u16 *edge = (u16 *)gMapCellBuffer;
			if (frame == 1)
			{
				u8 *inset = Ram_MapCellBuffer + 2;
				s32 n;
				for (n = 0; n != 128; n++)
				{
					inset[n] = Random16() & 63;
				}
				*edge = 0;
				Scheduler_AddOrUpdateCallback(BattleFx_BuildWindowEdgeTable, 0x480);
			}
			*edge += step;
			step += 3;
			if (frame == 15)
			{
				Scheduler_RemoveCallback(BattleFx_BuildWindowEdgeTable);
			}
		}

		// 残っている岩の数
		if (frame > 103)
		{
			count = 0;
		}
		else if (frame > 63)
		{
			count = 6;
		}
		else if (frame > 31)
		{
			count = 10;
		}

		// 揺れ : フレーム 167 まではでたらめ、176〜179 は表のとおり
		if (frame <= 167)
		{
			ox = (Random16() & 3) - 1;
			oy = (Random16() & 3) - 1;
			gBgScroll.unk04 = ox;
			gBgScroll.unk06 = oy + 32;
		}
		else
		{
			ox = 0;
			oy = 0;
			gBgScroll.unk04 = 0;
			gBgScroll.unk06 = 32;
		}
		if (frame >= 176 && frame < 180)
		{
			ox = -BattleFx10_ShakeOffsets[frame - 176];
			oy = BattleFx10_ShakeOffsets[frame - 176];
			gBgScroll.unk04 = ox;
			gBgScroll.unk06 = oy + 32;
		}

		for (i = 0; i != count; i++)
		{
			s32 n = i % 3;
			draw(canvas, work->sheet + BattleFx10_RockCells[n], BattleFx10_Points[i][0] - ox,
				BattleFx10_Points[i][1] - BattleFx10_RockHeights[n] - oy, BattleFx10_RockWidths[n],
				BattleFx10_RockHeights[n]);
		}

		// 3×3 のオブジェクトを 32 ドットおきに並べる
		scale = BattleFx10_UnitScale;
		if (frame == 174)
		{
			for (i = 0; i != 9; i++)
			{
				((struct {
					u8 pad[9];
					u8 tile_hi : 2;
					u8 priority : 2;
					u8 palette : 4;
				} *)work->objects[i])
					->priority = 0;
			}
		}
		if (frame > 208)
		{
			AnimationObjects_SelectAnimationFar(
				work->objects[2], BattleFx10_Animations[(frame / 4) & 3]);
		}

		scale.x = 0x10000;
		scale.y = 0x10000;
		pos[3] = 0;
		pos[1] = 255 << 16;
		for (row = 0; row != 3; row++)
		{
			for (j = 0; j != 3; j++)
			{
				pos[0] = (j * 32 + 144 - ox) << 16;
				pos[2] = (row * 32 + 76 - oy) << 16;
				Object_ApplyProjectedPlacementFar(work->objects[row * 3 + j], pos, &scale, 0);
			}
		}

		// 大きな氷塊 (24×48) : 160〜175 で右上から落ち、208 からゆっくり沈む
		t = frame - 160;
		if (t >= 0 && t < 158)
		{
			s32 u = frame - 208;
			s32 sx = 80;
			s32 sy = 8;
			if (frame <= 175)
			{
				sx = 96 - t;
				sy = t * 4 - 56;
			}
			else if (frame > 208)
			{
				sy = u / 4 + 8;
			}
			draw_alt(canvas, work->sheet + 0xc46, sx, sy, 24, 48);
		}

		if (frame == 32)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 64)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 104)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 176)
		{
			AudioCommand_PlayFar(134);
		}
		if (frame == 226)
		{
			AudioCommand_PlayFar(145);
		}
		Render_ResetTransformState();

		// 岩が崩れるたびに破片を 32 個ずつ跳ねあげる
		if (frame == 32)
		{
			for (i = 0; i != 32; i++)
			{
				PARTICLES[i].x = ((Random16() & 31) + 68) << 16;
				PARTICLES[i].y = ((Random16() & 31) + 8) << 16;
				PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
				PARTICLES[i].velocity_y = ((-Random16() & 127) - 64) << 11;
				PARTICLES[i].variant = (Random16() & 15) + 32;
			}
		}
		if (frame == 64)
		{
			for (i = 0; i != 32; i++)
			{
				PARTICLES[i + 32].x = (Random16() % 48 + 60) << 16;
				PARTICLES[i + 32].y = ((Random16() & 31) + 52) << 16;
				PARTICLES[i + 32].velocity_x = ((Random16() & 127) - 63) << 12;
				PARTICLES[i + 32].velocity_y = ((-Random16() & 31) - 32) << 13;
				PARTICLES[i + 32].variant = (Random16() & 15) + 32;
			}
		}
		if (frame == 104)
		{
			for (i = 0; i != 32; i++)
			{
				PARTICLES[i].x = ((Random16() & 63) + 52) << 16;
				PARTICLES[i].y = ((Random16() & 31) + 72) << 16;
				PARTICLES[i].velocity_x = ((Random16() & 127) - 63) << 11;
				PARTICLES[i].velocity_y = ((-Random16() & 31) - 32) << 13;
				PARTICLES[i].variant = (Random16() & 15) + 32;
			}
		}

		if (frame >= 32 && frame < 208)
		{
			for (i = 0; i != 64; i++)
			{
				if (PARTICLES[i].variant >= 0)
				{
					s32 n;
					if (frame > 191)
					{
						n = i % 7 + 4;
					}
					else
					{
						n = i & 3;
					}
					draw_alt(canvas, work->sheet + BattleFx10_DebrisCells[n], HI(PARTICLES[i].x),
						HI(PARTICLES[i].y), BattleFx10_DebrisWidths[n],
						BattleFx10_DebrisHeights[n]);
					PARTICLES[i].x += PARTICLES[i].velocity_x;
					PARTICLES[i].y += PARTICLES[i].velocity_y;
					PARTICLES[i].velocity_y += 0x2000;
				}
			}
		}

		// しぶき : (72, 56) から左下へ 128 個、228 からはその跡に煙を残す
		if (frame > 223)
		{
			if (frame == 224)
			{
				for (i = 0; i != 128; i++)
				{
					PARTICLES[i].x = 72 << 16;
					PARTICLES[i].y = 56 << 16;
					PARTICLES[i].velocity_x = (-(Random16() & 127) - 64) << 11;
					PARTICLES[i].velocity_y = ((Random16() & 127) + 16) << 11;
					PARTICLES[i].variant = Random16();
				}
			}
			for (i = 0; i != 128; i++)
			{
				if (frame >= i / 4 + 224)
				{
					s32 n = i % 3;
					if ((i & 1) == 0)
					{
						draw(canvas, work->sheet + BattleFx10_SprayCells[n], HI(PARTICLES[i].x),
							HI(PARTICLES[i].y), BattleFx10_SprayWidths[n],
							BattleFx10_SprayHeights[n]);
					}
					PARTICLES[i].x += PARTICLES[i].velocity_x;
					PARTICLES[i].y += PARTICLES[i].velocity_y;
					if ((PARTICLES[i].x >> 16) < -16 || (PARTICLES[i].y >> 16) > 120)
					{
						PARTICLES[i].x = 72 << 16;
						PARTICLES[i].y = 56 << 16;
					}
					PARTICLES[i].variant++;
				}
			}
			if (frame == 228)
			{
				for (i = 0; i != 128; i++)
				{
					p = &PARTICLES[i];
					q = &TRAILS[i];
					q->x = p->x;
					q->y = p->y;
					q->variant = 0;
				}
			}
			for (i = 0, q = TRAILS, p = PARTICLES; i != 128; i++, q++, p++)
			{
				if (frame >= i + 228)
				{
					s32 n = q->variant / 2 % 9;
					s32 sz = BattleFx_PuffSizes[n];
					draw(canvas, dots + BattleFx_PuffCells[n], HI(q->x) - sz / 2, HI(q->y) - sz / 2,
						sz, sz);
					if (++q->variant == 18)
					{
						q->x = p->x;
						q->y = p->y;
						q->variant = 0;
					}
				}
			}
		}

		work->transfer_pending = 1;
		WaitFrames(1);
	}

	//------------------------------------------------------------------
	// 後半 : 氷塊を降らせて対象に当て、大岩を崩して破片を跳ねさせる
	//------------------------------------------------------------------
	for (i = 0; i != 9; i++)
	{
		ResourceObject_ReleaseFar((struct ResourceObjectWork *)work->objects[i]);
	}
	BattleEffect_SetupBlendedDisplay();
	REG_BLDALPHA = 0x1010;
	AudioCommand_PlayFar(289);

	for (i = 0; i != 64; i++)
	{
		work->sparks[i].x = ((Random16() & 127) + 64) << 16;
		work->sparks[i].z = -(16 << 16);
		work->sparks[i].y = (-(Random16() & 127) - 64) << 16;
		work->sparks[i].velocity_x = (-(Random16() & 63) - 127) << 12;
		work->sparks[i].velocity_y = ((Random16() & 63) + 127) << 12;
		work->sparks[i].variant = 0;
	}
	for (i = 0; i != 64; i++)
	{
		PARTICLES[i].x = Random16() & 127;
		PARTICLES[i].y = (Random16() & 63) + i / 2;
		PARTICLES[i].variant = -i / 2;
	}

	for (frame = 0; frame != 146; frame++)
	{
		count = 0;
		if (frame == 96)
		{
			BattleEventRuntime_BeginPhaseFar(134);
		}
		if (frame == 16)
		{
			work->shake = 32;
		}
		// 大岩を 2 フレームに 1 つずつ、16 個まで出す
		if (frame > 16)
		{
			count = (frame - 16) / 2;
			if (count > 16)
			{
				count = 16;
			}
		}
		if (frame >= 9 && frame < 72 && (frame & 3) == 0)
		{
			AudioCommand_PlayFar(132);
		}
		if (frame == 72)
		{
			AudioCommand_PlayFar(145);
		}
		// 右上から斜めに落ちる塊 (40×80)
		if (frame > 64)
		{
			draw(canvas, work->sheet + 0x14f9, (64 - frame) * 7 + 88, frame * 14 - 992, 40, 80);
		}
		if (frame <= 71)
		{
			for (i = 0; i != count; i++)
			{
				s32 n = i % 3;
				draw(canvas, work->sheet + BattleFx10_BoulderCells[n], BattleFx10_Points[i][0] - 56,
					BattleFx10_Points[i][1] - BattleFx10_BoulderHeights[n],
					BattleFx10_BoulderWidths[n], BattleFx10_BoulderHeights[n]);
			}
		}
		// 大岩が崩れ、破片 64 個を岩の位置から跳ねあげる
		if (frame == 72)
		{
			for (i = 0; i != 64; i++)
			{
				work->sparks[i].x = (BattleFx10_Points[i & 15][0] - 56) << 16;
				work->sparks[i].y = BattleFx10_Points[i & 15][1] << 16;
				work->sparks[i].velocity_x = ((Random16() & 127) - 63) << 13;
				work->sparks[i].velocity_y = (-(Random16() & 31) - 16) << 14;
			}
			if (frame == 72)
			{
				work->shake = 4;
			}
		}
		// 破片 : 速く落ちてきたものは y = 120 で 1/4 の速さで跳ね返る
		if (frame > 71)
		{
			count = 32;
			if (frame != 72)
			{
				count = 64;
			}
			for (i = 0; i != count; i++)
			{
				q = &work->sparks[i];
				y = HI(q->y);
				if (y <= 135)
				{
					s32 n = i % 3;
					draw(canvas, work->sheet + BattleFx10_FallCells[n], HI(q->x),
						y - BattleFx10_FallHeights[n], BattleFx10_FallWidths[n],
						BattleFx10_FallHeights[n]);
					EffectStep_AdvanceWithGravity2D(q, 64, 1 << 16);
					if (HI(q->y) > 120 && q->velocity_y > 8 << 16)
					{
						q->velocity_y = -q->velocity_y / 4;
						q->y = 120 << 16;
						work->shake = 1;
					}
				}
			}
		}
		// 降ってくる氷塊 (24×48) : フレーム 47 までは上から出しなおす
		if (frame <= 71)
		{
			for (i = 0; i != 64; i++)
			{
				draw(canvas, work->sheet, HI(work->sparks[i].x) - 12, HI(work->sparks[i].y) - 24,
					24, 48);
				work->sparks[i].x += work->sparks[i].velocity_x;
				work->sparks[i].y += work->sparks[i].velocity_y;
				if ((work->sparks[i].y >> 16) > 120 && frame <= 47)
				{
					work->sparks[i].x = ((Random16() & 127) + 64) << 16;
					work->sparks[i].y = -(16 << 16);
				}
			}
		}

		for (i = 0; i != work->effect->count; i++)
		{
			if (frame == i * 8 + 32)
			{
				ObjectGroup_UpdateMembers(work->effect->actors[i], 9, 5, -1, 0);
			}
		}

		// 煙 : フレーム 127 までは出しなおす
		if (frame > 72)
		{
			for (i = 0; i != 64; i++)
			{
				if (PARTICLES[i].variant >= 0 && PARTICLES[i].variant < 18)
				{
					s32 n = PARTICLES[i].variant / 2 % 9;
					s32 sz = BattleFx_PuffSizes[n];
					draw(canvas, dots + BattleFx_PuffCells[n], PARTICLES[i].x - sz / 2,
						PARTICLES[i].y - sz / 2, sz, sz);
				}
				if (++PARTICLES[i].variant == 18 && frame <= 127)
				{
					PARTICLES[i].x = Random16() & 127;
					PARTICLES[i].y = (Random16() & 63) + (frame - 54) / 2;
					PARTICLES[i].variant = 0;
				}
			}
		}

		if (frame >= 72 && frame < 80)
		{
			Camera_ApplyShake(8, 8);
		}
		else
		{
			Camera_ApplyShake(2, 2);
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------
// 氷の破片
//--------------------------------------------------------------------------
#define SHARDS_PER_MEMBER 128 // 対象ひとりあたりの破片の数

//--------------------------------------------------------------------------
// 対象ごとに 20 フレームずつずらして、破片を跳ねあげる
// effect : 対象の一覧
//--------------------------------------------------------------------------
void BattleFx_RunIceShardBursts(struct BattleEffectArgument *effect)
{
	void				   **heap;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	s32						 point[3];
	struct EffectPosition	 screen;
	s32						 member;
	s32						 frame;
	s32						 i;

	heap = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap[HEAP_SLOT_BATTLE_SHEET - HEAP_SLOT_BATTLE_EFFECT];
	camera = heap[HEAP_SLOT_CAMERA - HEAP_SLOT_BATTLE_EFFECT];
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry((s32)&ResourceId_IceBlockSheet), 128);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw = (DrawRectangle)heap[HEAP_SLOT_BLITTER - HEAP_SLOT_BATTLE_EFFECT];

	for (i = 0; i != 8 * SHARDS_PER_MEMBER; i++)
		((struct EffectStep *)Ram_MapCellBuffer)[i].variant = -1;

	Render_ResetTransformState();
	Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);

	// 体の中ほどのまわりに置き、横へばらけて上へ跳ねる
	for (member = 0; member != work->effect->count; member++)
	{
		struct MotionObject *object;
		s32					 offset;
		s32					 height;
		struct EffectStep	*shard;

		offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
		object = GetBattleObjectSlotFar(work->effect->actors[member])->object;
		height = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
		point[0] = object->x;
		point[1] = height;
		point[2] = object->z;
		EffectPosition_ApplyBaseAndYOffset(point, &screen);
		screen.x >>= 1;
		for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
			i != SHARDS_PER_MEMBER; i++)
		{
			s32 radius;
			s32 angle;
			s32 lift;

			radius = Random16() & 0xff;
			angle = Random16() & 0xffff;
			shard->x = ((Trig_Sin(angle) * radius) >> 7) + (screen.x << 16);
			shard->y = ((Trig_Cos(angle) * radius) >> 3) + (screen.y << 16);
			shard->velocity_x = (128 - (Random16() & 0xff)) << 9;
			lift = Random16() & 0xff;
			shard->variant = 0;
			shard->velocity_y = (-lift - 128) << 10;
			shard++;
		}
	}

	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != work->effect->count * 20 + 56; frame++)
	{
		if (frame == 32)
			BattleEventRuntime_BeginPhaseFar(0);
		for (member = 0; member != work->effect->count; member++)
		{
			s32 offset;

			offset = member * SHARDS_PER_MEMBER * sizeof(struct EffectStep);
			if (frame == member * 20)
			{
				AudioCommand_PlayFar(143);
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, -1, member, 20);
			}
			// 落ちてきて y = 112 を越えたら消す
			if (frame > member * 20)
			{
				struct EffectStep *shard;

				for (i = 0, shard = (struct EffectStep *)(Ram_MapCellBuffer + offset);
					i != SHARDS_PER_MEMBER; i++)
				{
					if (shard->variant >= 0)
					{
						s32 size;

						size = i % 3 + 1;
						draw(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							HI(shard->x) - size / 2, HI(shard->y) - size, size, size * 2);
						EffectStep_AdvanceWithGravity2D(shard, 62, IceShardBursts_Gravities[i & 3]);
						shard->variant++;
						if (shard->velocity_y > 0 && HI(shard->y) > 112)
							shard->variant = -1;
					}
					shard++;
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}

//--------------------------------------------------------------------------

// gather.c

// バトル演出：対象から出た光の粒を使い手へ吸い寄せる

//--------------------------------------------------------------------------

#define MOTES_PER_MEMBER 128 // 対象ひとりあたりの粒の数

#define MOTE(n) (&((struct EffectStep *)Ram_MapCellBuffer)[n])

//--------------------------------------------------------------------------
// 対象ごとに 20 フレームずつずらして粒を出し、使い手の体の中心へ集める
// effect : variant が 0 なら IceBlock、それ以外は Blast のパレット
//--------------------------------------------------------------------------
void BattleFx_RunGatheringMotes(struct BattleEffectArgument *effect)
{
	void				   **heap_cache;
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	DrawRectangle			 draw;
	u8						*sheet;
	struct BattleCamera		*camera;
	struct EffectPosition	 screen;
	s32						 alternate;
	s32						 member;
	s32						 frame;
	s32						 i;

	heap_cache = &gWorkSlot[HEAP_SLOT_BATTLE_EFFECT];
	cursor = heap_cache;
	work = *cursor++;
	canvas = *cursor;
	sheet = heap_cache[2];
	camera = *(struct BattleCamera **)((u8 *)gWorkSlot + HEAP_SLOT_CAMERA * 4);
	if (effect->variant == 0)
		alternate = 0;
	else
		alternate = 1;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(1);
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	Iwram_CopyWords((void *)BG_PLTT,
		Resource_GetTableEntry(
			alternate == 0 ? (s32)&ResourceId_IceBlockSheet : (s32)&ResourceId_BlastSheet),
		128);

	// 対象の体の中心から、各軸 ±2 ドット/フレームの乱数速度で飛び出す
	for (i = 0; i != 8 * MOTES_PER_MEMBER; i++)
		MOTE(i)->variant = -1;

	for (member = 0; member != work->effect->count; member++)
	{
		struct MotionObject *obj;
		s32					 half;

		obj = GetBattleObjectSlotFar(work->effect->actors[member])->object;
		half = Battle_GetObjectTableValueFar(work->effect->actors[member]) / 2;
		for (i = 0; i != MOTES_PER_MEMBER; i++)
		{
			struct EffectStep *mote = MOTE(member * MOTES_PER_MEMBER + i);

			mote->x = obj->x;
			mote->y = obj->y + half;
			mote->z = obj->z;
			mote->velocity_x = ((Random16() & 0xff) - 128) << 10;
			mote->velocity_y = ((Random16() & 0xff) - 128) << 10;
			mote->velocity_z = ((Random16() & 0xff) - 128) << 10;
			mote->variant = 0;
		}
	}

	// 比較合成で描き、転送のたびにキャンバスの画素値を 4 ずつ下げて残像にする
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, GOUSEI_CLIP_X | GOUSEI_CLIP_Y, GOUSEI_HIKAKU);
	draw = (DrawRectangle)gWorkSlot[HEAP_SLOT_BLITTER];
	work->transfer_mode = 3;
	work->transfer_value = 0x04040404;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
	AudioCommand_PlayFar(142);

	for (frame = 0; frame != work->effect->count * 20 + 72; frame++)
	{
		struct MotionObject *user;
		s32					 half;

		user = GetBattleObjectSlotFar(work->effect->actor)->object;
		half = Battle_GetObjectTableValueFar(work->effect->actor) / 2;
		if (frame == 64)
			BattleEventRuntime_BeginPhaseFar(133);
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		if (frame == 40)
			ObjectGroup_UpdateMembers(work->effect->actor, 7, -1, -1, 0);
		if (frame == work->effect->count * 20 + 52)
			ObjectGroup_UpdateMembers(work->effect->actor, 0, -1, -1, 0);
		for (member = 0; member != work->effect->count; member++)
		{
			if (frame == member * 20)
				ObjectGroup_UpdateMembers(work->effect->actors[member], 7, 5, member, 42);
			if (frame > member * 20)
			{
				for (i = 0; i != 32; i++)
				{
					struct EffectStep *mote = MOTE(member * MOTES_PER_MEMBER + i);

					if (mote->variant >= 0)
					{
						s32 size;

						// キャンバスは横 2 倍で表示するので x を半分にする
						EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &screen);
						screen.x >>= 1;
						size = 6;
						draw(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
							screen.x - size / 2, screen.y - size, size, size * 2);
						EffectStep_AdvanceWithGravity3D(mote, 62, 0);

						// 出てから 10 フレーム後 (粒ごとに 1 フレームずつ遅れる) に
						// 距離の 1/256 ずつ使い手へ加速し、水平に 16 ドット以内まで近づいたら消す
						if (frame > member * 20 + i + 10)
						{
							s32 dx;
							s32 dy;
							s32 dz;

							dx = (user->x - mote->x) >> 8;
							dy = (user->y + half - mote->y) >> 8;
							dz = (user->z - mote->z) >> 8;
							mote->velocity_x += dx;
							mote->velocity_y += dy;
							mote->velocity_z += dz;
							if ((dx > -0x1000 && dx < 0x1000) && (dz > -0x1000 && dz < 0x1000))
								mote->variant = -1;
						}
					}
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
