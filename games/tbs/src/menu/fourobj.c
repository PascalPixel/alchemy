#include "unit/menu/menu.h"
#include "unit/menu/item.h"
#include "unit/menu/status.h"
#include "unit/menu/fourobj.h"
#include "unit/runtime/sched.h"
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
#include "callback_scheduler.h"
#include "dma.h"
#include "inventory_menu.h"
#include "io_reg.h"
#include "uiwindow.h"
#include "mem.h"
#include "character_menu.h"
#include "battle_unit.h"
#include "item.h"
#include "edition.h"
#include "btlsys.h"
#include "ui.h"
#include "text/msg_ids.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "fixed_math.h"
#include "lib.h"
#include "psynergy_menu.h"
#include "animspr.h"
#include "field_sprite.h"
#include "scene.h"
#include "m7_interfaces.h"
#include "global_cells.h"
#include "keystate.h"
#include "window.h"
#include "menu_result.h"
#include "tbs_edition.h"
#include "game_state.h"
#include "sound_ids.h"
#include "iwram_call.h"
#include "heap_state.h"
#include "menu_list.h"
#include "irq.h"
#include "object_factory.h"
#include "far_runtime.h"
#include "equipment_menu.h"
#include "owner_state.h"
#include "inventory.h"
#include "layout_guard.h"
#include "btltype.h"
#include "btlcalc.h"
#include "eventwrk.h"
#include "djinn_menu.h"
#include "a9_motion.h"
#include "battle_efx.h"
#include "four_object_motion.h"

struct InventoryMenuState;

void FourObjectMotion_InitializeTopRow(void *unused0, s32 unused1)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  index;

	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object = state->objects[index];

		if (object != NULL)
		{
			ResourceObject_ReleaseFar(object);
			state->objects[index] = NULL;
		}
	}

	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object =
			ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

		if (object != NULL)
		{
			AnimationObjects_SelectAnimationFar(object, 2);
		}
		state->objects[index] = object;
		state->positions_x[index] = 0x10;
		state->positions_y[index] = 0x20;
	}
	Scheduler_AddOrUpdateCallback((s32)FourObjectMotion_UpdateAllPositions, 0xc80);
}

// 4 つのオブジェクトを解放し、スケジューラから update を外す
#define DEFINE_FOUR_OBJECT_RELEASE(name, update)                    \
	void name(void)                                                 \
	{                                                               \
		struct FourObjectMotionState *state = gMenuWork;            \
		s32							  index = 0;                    \
                                                                    \
		do                                                          \
		{                                                           \
			struct AnimationObject *object = state->objects[index]; \
                                                                    \
			if (object != NULL)                                     \
			{                                                       \
				ResourceObject_ReleaseFar(object);                  \
				state->objects[index] = NULL;                       \
			}                                                       \
			index++;                                                \
		} while (index < 4);                                        \
		Scheduler_RemoveCallback((u32)update);                      \
	}

DEFINE_FOUR_OBJECT_RELEASE(FourObjectMotion_ClearSlotsAndSchedule,
	FourObjectMotion_UpdateAllPositions)

#define FIXED_ONE 0x10000u
#define VERTICAL_ORIGIN 0x01e20000u

void FourObjectMotion_UpdateAllPositions(void)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  index;
	u32							  motion[2];
	u32							  request[4];

	index = 0;
	while (1)
	{
		u32 object = (u32)state->objects[index];

		if (object != 0)
		{
			u32 vertical = VERTICAL_ORIGIN - ((s32)state->vertical_origins[index] << 16);
			((struct FieldSprite *)object)->priority = 0;
			motion[0] = FIXED_ONE;
			motion[1] = FIXED_ONE;
			request[0] = (s32)state->positions_x[index] << 16;
			request[1] = vertical;
			request[2] = ((s32)state->positions_y[index] << 16) + vertical;
			request[3] = 0;
			Object_ApplyProjectedPlacementFar(object, request, motion, 0x4000);
		}
		index += 1;
		if (index > 3)
			break;
	}
}

void FourObjectMotion_UpdateBottomRow(void)
{
	struct FourObjectMotionState *work = gMenuWork;
	s32							  i;
	u32							  motion[2];
	u32							  request[4];

	Link_DrawShiftedTilePairFar((void *)0x06002500);
	i = 0;
	do
	{
		u32 obj = (u32)work->objects[i];

		if (obj != 0)
		{
			u32 y = (241u << 17) - ((u32)(s32)work->vertical_origins[i] << 16);
			s32 phase;
			s32 limit;
			/* Ordinary s32/s16 locals, a direct field test, an earlier limit
             * initializer and an inline sign/limit helper all differ 81 in
             * EN (119 native, 118 built), changing the outer high-register
             * allocation as well as this read. */
			/* FAKEMATCH: the row's second read lives in r2, as the native allocates it. 2026-10-11: plain locals take r3 (local-alloc's first free register, REG_ALLOC_ORDER 3,2,1,0), which costs limit its r3 preference and reshuffles the high registers (differ 81); reading py before request[3] = 0 puts it in r2 but emits the ldrsh before the zero store (differ 4 in all six editions). Measured 2026-10-13 with py plain: the loop index as u8, s8 or u16 differs 130 to 140, as s16 82. */
			register s32 py asm("r2");

			((struct FieldSprite *)obj)->priority = 0;
			phase = work->phases[i];
			if (phase < 0)
			{
				motion[0] = -phase;
				motion[1] = -phase;
			}
			else
			{
				motion[0] = phase + (s32)(0x10000 - phase) / 3;
				motion[1] = motion[0];
				work->phases[i] = motion[0];
			}
			request[0] = (u32)(s32)work->positions_x[i] << 16;
			request[1] = y;
			request[2] = ((u32)(s32)work->positions_y[i] << 16) + y;
			request[3] = 0;
			py = work->positions_y[i];
			limit = 0x8000;
			if (py >= 0)
				limit = 0x4000;
			Object_ApplyProjectedPlacementFar(obj, request, motion, limit);
		}
		i++;
	} while (i <= 3);
}

void FourObjectMotion_InitializeBottomRow(s32 unused0, s32 unused1)
{
	struct FourObjectMotionState *state = gMenuWork;
	s32							  index;

	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object = state->objects[index];

		if (object != NULL)
		{
			ResourceObject_ReleaseFar(object);
			state->objects[index] = NULL;
		}
	}
	for (index = 0; index < 4; index++)
	{
		struct AnimationObject *object =
			ResourceObject_CreateFar(FourObjectMotion_ResourceIds[index]);

		if (object != NULL)
			AnimationObjects_SelectAnimationFar(object, 2);
		state->objects[index] = object;
		state->phases[index] = 0x10000;
		state->positions_x[index] = 0x10;
		state->positions_y[index] = 0xc8;
	}
	Scheduler_AddOrUpdateCallback((s32)FourObjectMotion_UpdateBottomRow, 0xc80);
}

s32 FourObjectMotion_SetSlotPosition(s32 index, s32 x, s32 y, s32 negative)
{
	struct FourObjectMotionState *state = gMenuWork;

	if (state->objects[index] != NULL)
	{
		state->positions_x[index] = x;
		state->positions_y[index] = negative != 0 ? y | (s32)0xffff8000 : y;
		return 0x23c + index * 2;
	}
	return index;
}

s32 FourObjectMotion_SetSlotPhase(s32 index, s32 phase)
{
	((struct FourObjectMotionState *)gMenuWork)->phases[index] = phase;
}

s32 FourObjectMotion_ReplaceSlot(s32 index, s32 kind, s32 value)
{
	struct FourObjectMotionState *state = gMenuWork;

	if (state->objects[index] != NULL)
	{
		ResourceObject_ReleaseFar(state->objects[index]);
		state->objects[index] = NULL;
	}
	{
		struct AnimationObject *object =
			ResourceObject_CreateFar(FourObjectMotion_ResourceIds[kind]);

		if (object != NULL)
			AnimationObjects_SelectAnimationFar(object, value);
		state->objects[index] = object;
	}
	return 1;
}

DEFINE_FOUR_OBJECT_RELEASE(FourObjectMotion_ClearSlotsAndScheduleAlt,
	FourObjectMotion_UpdateBottomRow)

void Menu_EnableAllItemObjects(void)
{
	struct InventoryMenuState *state = gMenuWork;
	s32						   index;

	for (index = 0; index < state->party_count; ++index)
	{
		AnimationObjects_SelectAnimationFar(state->owner_objects[index], 1);
	}
}
