#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "MEM.H"
#include "HEAP_STATE.H"
#include "CANVAS.H"
#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BTLDISP.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTOBJ.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "MENU_LIST.H"
#include "TRANSFORM.H"

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)

void BattleFx_RunParticlePoolMode1(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticlePool(effect, 1);
}

void BattleFx_RunParticlePoolMode0(struct BattleEffectArgument *effect)
{
	BattleFx_RunParticlePool(effect, 0);
}

struct BlitterPair
{
	DrawRectangle upper;
	DrawRectangle lower;
};

void BattleFx_RunParticlePool(struct BattleEffectArgument *effect, s32 mode)
{
	s32						 point[3];
	struct EffectPosition	 position;
	struct BattleEffectWork *work;
	void					*canvas;
	struct BlitterPair		 draw;
	struct BattleCamera		*camera;
	u8						*sheet;
	struct MotionObject		*source;
	struct EffectStep		*mote;
	s32						 resource;
	s32						 frame;
	s32						 i;

	work = *(gWorkSlot + HEAP_SLOT_BATTLE_EFFECT);
	canvas = *(gWorkSlot + HEAP_SLOT_BATTLE_CANVAS);
	camera = *(gWorkSlot + HEAP_SLOT_CAMERA);
	sheet = *(gWorkSlot + HEAP_SLOT_BATTLE_SHEET);
	work->effect = effect;
	if (mode == 0)
		BattleFx_BeginCanvasLayer(0);
	else
		BattleFx_BeginCanvasLayer(1);
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER, 7, 7, 3, 2);
	draw.upper =
		(BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)->slots[HEAP_SLOT_BLITTER];
	BattleEffect_LoadWork(HEAP_SLOT_BLITTER_ALTERNATE, 7, 7, 11, 2);
	draw.lower = (BattleEffectDrawRectangle)((union HeapState *)gWorkSlot)
					 ->slots[HEAP_SLOT_BLITTER_ALTERNATE];
	Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, sheet, 0, 0);
	if (mode == 0)
		resource = (s32)&ResourceId_OrangePaletteA;
	else
		resource = (s32)&ResourceId_EarthWallSheet;
	Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
	work->transfer_mode = 2;
	work->transfer_value = 75;
	Scheduler_AddOrUpdateCallback(BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

	source = GetBattleObjectSlotFar(work->effect->actor)->object;
	for (i = 0; i != 256; i++)
	{
		s32 speed;
		s32 angle;

		mote = &gMotes[i];

		speed = 0x3ff;
		speed &= Random16();
		angle = Random16() & 0xffff;
		mote->x = source->x;
		mote->y = source->y + 0x50000;
		mote->z = source->z;
		mote->velocity_x = Trig_Sin(angle) * (speed + 32) >> 8;
		mote->velocity_y = ((Random16() & 255) - 32) << 9;
		mote->velocity_z = -(Trig_Cos(angle) * (speed + 32) << 1) >> 8;
		mote->variant = (Random16() & 31) + 48;
		if (mode == 0)
		{
			mote->velocity_x /= 2;
			mote->velocity_z /= 2;
		}
	}

	for (frame = 0; frame != 128; frame++)
	{
		Render_ResetTransformState();
		Graphics_PrepareTransferInIwramWork(camera->eye, camera->pos);
		for (i = 0; i != 128; i++)
		{
			mote = &gMotes[i];
			if (frame >= i / 32 * 8 && mote->variant >= 0)
			{
				s32 size;

				point[0] = mote->x + Trig_Sin((i * 4 + mote->variant) << 10) * 16;
				point[1] = mote->y;
				point[2] = mote->z;
				EffectPosition_ApplyBaseAndYOffset(point, &position);
				position.x >>= 1;
				if (position.depth <= 313)
					position.depth = 314;
				if (position.depth > 634)
					position.depth = 634;
				size = 6 - (position.depth - 314) / 64;
				draw.upper(canvas, sheet + ParticleStreams_CellOffsets[size - 1],
					position.x - size / 2, position.y - size, size, size * 2);
				EffectStep_AdvanceWithGravity3D(mote, 62, 0x400);
				if (mode == 1)
				{
					if (source->x < 0)
						mote->velocity_x += 0x2000;
					else
						mote->velocity_x -= 0x2000;
				}
				mote->variant--;
			}
		}
		if (mode == 1)
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i * 8 + 48)
				{
					BattleEventRuntime_BeginPhaseFar(-1);
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, 5, i, 8);
				}
			}
		}
		else
		{
			for (i = 0; i != work->effect->count; i++)
			{
				if (frame == i * 8 + 48)
				{
					AudioCommand_PlayFar(126);
					BattleEventRuntime_BeginPhaseFar(-1);
					ObjectGroup_UpdateMembers(work->effect->actors[i], 7, -1, i, 8);
				}
			}
		}
		ObjectGroup_TickMemberTimers();
		work->transfer_pending = 1;
		WaitFrames(1);
	}

	Scheduler_RemoveCallback(BattlePresentation_ProcessPendingGraphicsTransfer);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER_ALTERNATE);
	Runtime_ReleaseHeapBlock(HEAP_SLOT_BLITTER);
	BattleFx_EndCanvasLayer();
}
