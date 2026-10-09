#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "map_scroll.h"
#include "event_runtime.h"
#include "battle_effect_runtime.h"
#include "types.h"
#include "heap_state.h"
#include "script_motion.h"
#include "fx_scene.h"
#include "callback_scheduler.h"
#include "system.h"
#include "scene.h"
#include "ram_buffer.h"
#include "dma.h"
#include "global_cells.h"
#include "frame.h"
#include "io_reg.h"
#include "btlsys.h"
#include "text/msg_ids.h"
void AudioCommand_PlayFar(s32 cue);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

extern struct BattleRuntime *gEventWork;

#define BattleFx_UpdateScaledArcObjectB ((void (*)(void))BattleFx_UpdateScaledArcObjectB)
#define BattleFx_UpdateScaledArcObjectA ((void (*)(void))BattleFx_UpdateScaledArcObjectA)
#define ObjectGroup_SetActionForOthers ((void (*)(void *, s32, s32))ObjectGroup_SetActionForOthers)

#define gObjectSlots ((u8(*)) & gObjectSlots)

extern struct BattleFxScene *gEffectWork;

struct BattleEffectRuntime
{
	u8	reserved_000[0xcc0];
	s16 restore_requested;
	u8	reserved_cc2[4];
	s8	teardown_blocked;
	s8	teardown_state;
};

struct BattleEffectPosition
{
	u8	reserved_00[4];
	s32 x;
	s32 y;
	s32 z;
};

#define BATTLE_OBJECT_SLOTS (*(struct ScriptMotionObject **)gObjectSlots)
void BattleFx_ClearOwnedSlot(struct EffectSlot *object);
void BattleFx_PlayQueuedSound(void);

void BattleFx_FlickerObjectAndTick();
void BattleFx_CycleObjectValueByCounter();
#define CALLBACK_1 (u32) BattleFx_FlickerObjectAndTick
#define CALLBACK_2 (u32) BattleFx_CycleObjectValueByCounter

s32 __umodsi3(s32, s32);

void BattleEffect_CleanupSceneObjects(void)
{
	struct BattleFxScene	   **scene_cell;
	struct BattleFxScene		*scene;
	struct BattleEffectRuntime	*runtime;
	struct BattleEffectPosition *position;
	struct EffectSlot			*scene_object;
	s32							 remaining;

	scene_cell = &gEffectWork;
	scene = *scene_cell;
	runtime = *(struct BattleEffectRuntime **)((u8 *)scene_cell - 116);
	position = *(struct BattleEffectPosition **)((u8 *)scene_cell - 192);
	for (remaining = 0; remaining < 24; remaining++)
	{
		scene_object = &scene->slots[remaining];
		if (scene_object->active != 0)
			BattleFx_ClearOwnedSlot(scene_object);
	}

	if (runtime->teardown_blocked == 0)
	{
		s32 waited = 0;
		void (*first_active_update)(void) = (void (*)(void))BattleFx_UpdateScaledArcObjectB;
		void (*second_active_update)(void) = (void (*)(void))BattleFx_UpdateScaledArcObjectA;
		s32 active;

		do
		{
			struct ScriptMotionObject *slot = BATTLE_OBJECT_SLOTS;

			active = 0;
			remaining = 0;
			while (remaining <= 63)
			{
				void (*update)(void) = (void (*)(void))slot->hook;

				if (update == first_active_update || update == second_active_update)
				{
					active = 1;
					break;
				}
				remaining++;
				slot++;
			}

			if (active != 0)
			{
				waited++;
				WaitFrames(1);
			}
		} while (active != 0 && waited <= 29);

		runtime->teardown_state = 0;
		Scheduler_RemoveCallback(BattleFx_UpdateAllEffectSlots);
		Resource_ResetEntry((s16)scene->tile_slot);
		position->x = scene->saved_x;
		position->y = scene->saved_y;
		position->z = scene->saved_z;
		if (scene->animation != 8)
			runtime->restore_requested = 1;
		BattleFx_PlayQueuedSound();
		Runtime_ReleaseHeapBlock(0x38);
	}
}

void EventObject_Initialize(void)
{
	s32					  zero;
	s32					  event_value;
	s32					  event_index;
	void				 *event_object;
	struct BattleFxScene *event_state;
	void				 *render_state;

	event_state = *(void **)Ram_EffectWork;
	render_state = *(void **)Ram_EventWork;
	zero = 0;
	event_object = event_state->main_object;
	event_value = (s16)event_state->action;
	event_index = event_value;
	ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x14);
	FIELD_AT_OFFSET(event_object, u32 *, 0x38) = *(s32 *)((u8 *)(event_object) + 8);
	FIELD_AT_OFFSET(event_object, s32 *, 0x3C) = FIELD_AT_OFFSET(event_object, s32 *, 0xC);
	FIELD_AT_OFFSET(event_object, s32 *, 0x40) = FIELD_AT_OFFSET(event_object, s32 *, 0x10);
	FIELD_AT_OFFSET(event_object, u32 *, 0x24) = 0;
	FIELD_AT_OFFSET(event_object, u32 *, 0x28) = 0;
	*(s32 *)((u8 *)(event_object) + 0x2C) = 0;
	if (event_state->visible != 0)
	{
		AudioCommand_PlayFar(212);
		FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_1;
	}
	if (event_state->active != 0)
	{
		ObjectGroup_SetActionForOthers(event_object, 1, 0);
		UiWork_PushValueSlotFar(event_index, 4);
		if ((s8)event_state->mode != 0)
		{
			UiText_ShowPositionedMessageAndWaitFar(
				(void *)MsgAbilityAnnounce, event_state->message_mode);
		}
		else
		{
			UiText_ShowPositionedMessageAndWaitFar(
				(void *)MsgAbilityAnnounce, event_state->message_mode);
		}
		ObjectGroup_SetActionForOthers(event_object, 0, 0x10);
	}
	if (GameFlag_TestFar(0x140) != 0)
	{
		if (event_state->visible != 0)
		{
			FIELD_AT_OFFSET(event_object, s32 *, 0x6C) = CALLBACK_2;
		}
		ObjectDispatch_ApplyArgumentToChildrenFar(event_object, 0x15);
	}
	else
	{
		EffectRuntime_StopCurrentObject();
	}
	FIELD_AT_OFFSET(render_state, s8 *, 0xCC7) = 1;
}

void BattleEffect_InitializeSharedScene(void)
{
	u8						   *scene;
	struct BattleEffectBuffers *work;
	s32							no;

	scene = *(gWorkSlot + HEAP_SLOT_EVENT);
	work = *(gWorkSlot + 32);
	Dma_Set(&work->target[0x2a0], scene + 0x776, 0x84000150, REG_DMA3);
	if (*(s16 *)(scene + 0xcb8) == 0)
		Dma_Set(work->target, scene + 0x236, 0x84000150, REG_DMA3);
	Dma_Set(work->target, work->current, 0x84000000 | (sizeof work->current / 4), REG_DMA3);

	no = gFrameCount & 7;
	if (GameFlag_TestFar(0x148))
		no = 0;
	if (GameFlag_TestFar(0x149))
		no = 1;
	if (GameFlag_TestFar(0x14a))
		no = 2;
	if (GameFlag_TestFar(0x14b))
		no = 3;
	if (GameFlag_TestFar(0x14c))
		no = 4;
	if (GameFlag_TestFar(0x14d))
		no = 5;
	if (GameFlag_TestFar(0x14e))
		no = 6;
	if (GameFlag_TestFar(0x14f))
		no = 7;
	BattleFx_ApplyColorToTargetBuffer(BattleFx_TargetColors[no], 1);
	BattleFx_StartBufferInterpolation(8);
}

void BattleFx_PrepareBufferInterpolation(void)
{
	s32 value;
	u8 *state;

	state = (u8 *)gEventWork;
	value = (s32)(state + 0x236);
	BattleFx_ApplyColorToTargetBuffer(value, 2);
	if (FIELD_AT_OFFSET(state, s16 *, 0xCB8) != 0)
	{
		BattleFx_ApplyColorToTargetBuffer(0x10001, 1);
	}
	else
	{
		BattleFx_ApplyColorToTargetBuffer(value, 1);
	}
	BattleFx_StartBufferInterpolation(8);
}

void Camera_WorldToScreen(struct EffectVector *position)
{
	struct EventRuntime **data = (struct EventRuntime **)&gEventWork;

	if ((*data)->mode_19e == 3)
	{
		struct EffectVector result;

		Render_ProjectPoint(position, &result);
		position->x = result.x << 16;
		position->z = result.y << 16;
		position->y = 0;
	}
	else
	{
		struct MapScrollWork *state = *(struct MapScrollWork **)((u8 *)data - 76);
		s32					  x = state->view_x & 0xffff0000;
		s32					  z = state->view_y & 0xffff0000;

		position->x -= x;
		position->z -= position->y;
		position->z -= z;
		position->y = 0;
	}
}

void RunSceneTransitionEffect(s32 x, s32 y)
{
	u8			*work;
	volatile u32 zero;

	work = Runtime_AllocateBlock(22, 0x298);
	BattleEffect_InitializeSharedScene();
	zero = 0;
	Dma_Set((const void *)&zero, work, 0x850000a6, REG_DMA3);
	*(u16 *)(work + 0x28e) = __umodsi3(gFrameCount << 1, 360);
	BattleFx_AdvanceHueCycle();
	BattleFx_ApplyColorToTargetBuffer(
		((s8)work[0x28d] << 10) | ((s8)work[0x28c] << 5) | (s8)work[0x28b] | 0x200000, 1);
	BattleFx_StartBufferInterpolation(8);
	*(u16 *)(work + 0x290) = x;
	*(u16 *)(work + 0x292) = y;
	work[0x294] = 8;
	Ui_FillBank15PaletteGrey();
	Scheduler_AddOrUpdateCallback((void (*)(void))BattleFx_UpdateWaveScene, 0xc80);
}
