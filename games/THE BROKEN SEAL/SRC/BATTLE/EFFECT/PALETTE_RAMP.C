#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "CANVAS.H"
#include "MEM.H"
#include "RESOURCE.H"
#include "BTLDISP.H"
#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "EFFECT_STEP.H"
#include "MOTOBJ.H"
#include "RAM_BUFFER.H"
#include "IO_REG.H"
#include "TRANSFORM.H"

#define SceneTransform_ApplyPosition ((void (*)(s32 *))SceneTransform_ApplyPosition)

void AudioCommand_PlayFar(s32 cue);

void BattleFx_RunPaletteRampMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 0);
}

void BattleFx_RunPaletteRampMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 1);
}

void BattleFx_RunPaletteRampMode2(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 2);
}

void BattleFx_RunPaletteRampMode3(struct BattleEffectArgument *effect)
{
	BattleFx_RunPaletteRamp(effect, 3);
}

#define RAMP_IMAGE 0x2580
#define RAMP_SIZE 696

void BattleFx_RunPaletteRamp(struct BattleEffectArgument *effect, s32 mode)
{
	void				   **cursor;
	struct BattleEffectWork *work;
	void					*canvas;
	s32						 frame;
	s32						 j;
	s32						 x_offset;
	DrawRectangle			*draw;
	struct BattleCamera		*camera;
	s32						 origin[3];
	struct EffectPosition	 screen;
	s32						 point[3];
	DrawRectangle			 callbacks[2];
	s32						 i;
	s32						 palette;

	cursor = (void **)gBattleFxWork;
	work = *cursor++;
	canvas = *cursor;
	work->effect = effect;
	BattleFx_BeginCanvasLayer(0);
	REG_BG2PA = 0x100;
	if (mode == 0)
		Resource_LoadAndDecompress((s32)&ResourceId_DaggerSheet, work->sheet, 1, 1);
	else
		Resource_LoadAndDecompress((s32)&ResourceId_ShieldSheet, work->sheet, 1, 1);
	if (mode == 0)
		palette = (s32)&ResourceId_PinkBurstSheet;
	else if (mode == 1)
		palette = (s32)&ResourceId_GlowOrbSheet;
	else
		palette = (s32)&ResourceId_PinkBurstSheet;
	Iwram_CopyWords((void *)BG_PLTT, Resource_GetTableEntry(palette), 128);
	Resource_LoadAndDecompress((s32)&ResourceId_PaletteRampImage, work->sheet + RAMP_IMAGE, 0, 0);

	for (i = 1; i != 8; i++)
	{
		for (j = 0; j != RAMP_SIZE; j++)
		{
			s32 level = work->sheet[RAMP_IMAGE + j];

			if (level > 64 - i * 7)
				level = 64 - i * 7;
			if (level < 0)
				level = 0;
			work->sheet[RAMP_IMAGE + i * RAMP_SIZE + j] = level;
		}
	}

	if (work->effect->side == 1)
	{
		REG_BG2X = -0x7000;
		x_offset = -112;
	}
	else
	{
		REG_BG2X = 0;
		x_offset = 0;
	}

	for (i = 0; i != 512; i++)
	{
		struct EffectStep *shard = &((struct EffectStep *)Ram_MapCellBuffer)[i];
		s32				   radius = 192;
		s32				   angle;

		angle = Random16() & 0xffff;
		shard->x = 0;
		if (mode == 0)
		{
			shard->y = ((i & 31) / 4 * 6 - 10) << 16;
			shard->z = (i % 4 * 2 - 2) << 16;
		}
		else
		{
			shard->y = ((i & 31) / 4 * 6 - 10) << 16;
			shard->z = (i % 4 * 8 - 16) << 16;
		}
		if (work->effect->side == 1)
			shard->velocity_x = 0x20000;
		else
			shard->velocity_x = -0x20000;
		shard->velocity_y = ((Trig_Cos(angle) * radius) >> 6) + 0x10000;
		shard->velocity_z = (Trig_Sin(angle) * radius) >> 6;
		shard->variant = Random16() & 0xff;
	}

	BattleFx_FetchRectangleBlitters(work->effect->side, draw = callbacks);
	work->transfer_mode = 2;
	work->transfer_value = 50;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	for (frame = 0; frame != work->effect->count * 4 + 64; frame++)
	{
		camera = gCameraWork;
		if (frame == 72)
			BattleEventRuntime_BeginPhaseFar(0);
		for (j = 0; j != work->effect->count; j++)
		{
			struct MotionObject *object;
			s32					 time;
			s32					 x;
			s32					 y;

			time = frame - j * 4;
			object = GetBattleObjectSlotFar(work->effect->actors[j])->object;
			if (time > 0)
			{
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				point[0] = object->x;
				point[1] = 160 << 13;
				point[2] = object->z;
				Render_ResetTransformState();
				Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
				SceneTransform_ApplyPosition(point);
				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;
				EffectPosition_ApplyBaseAndYOffset(origin, &screen);
				x = screen.x + x_offset;
				y = screen.y;
				if (mode == 0)
				{
					if (time <= 26)
						callbacks[0](
							canvas, work->sheet + time / 4 % 7 * 960, x - 12, y - 20, 24, 40);
				}
				else
				{
					if (time <= 23)
						draw[1](canvas, work->sheet + time / 4 % 6 * 1600, x - 20, y - 20, 40, 40);
				}
				if (time == 24)
					AudioCommand_PlayFar(143);
				if (time >= 24 && time <= 60)
				{
					s32 level;

					level = 0;
					if (time > 28)
					{
						level = (time - 24) / 4;
						if (level > 7)
							level = 7;
					}
					for (i = 0; i != 24; i++)
					{
						struct EffectStep *shard =
							&((struct EffectStep *)Ram_MapCellBuffer)[j * 32 + i];
						s32 cell;

						cell = i % 4 * 3 + (shard->variant + time) / 8 % 3;
						EffectPosition_ApplyBaseAndYOffset((s32 *)shard, &screen);
						x = screen.x + x_offset;
						y = screen.y;
						callbacks[0](canvas,
							work->sheet + (level * RAMP_SIZE + PaletteRamp_ShardCells[cell])
								+ RAMP_IMAGE,
							x, y, PaletteRamp_ShardWidths[cell], PaletteRamp_ShardHeights[cell]);
						EffectStep_AdvanceWithGravity3D(shard, 60, 0);
					}
				}
			}
		}
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(47);
	Runtime_ReleaseHeapBlock(46);
	BattleFx_EndCanvasLayer();
}
