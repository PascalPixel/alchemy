#include "unit/battle/effect/btlcue.h"
#include "unit/battle/effect/zonedata.h"
#include "vram_tab.h"
#include "unit/battle/effect/object.h"
#include "unit/battle/effect/object2.h"
#include "unit/battle/effect/object3.h"
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
#include "unit/field/common/effect/move.h"
#include "unit/graphics/display/scroll_initialize_hblank_dma.h"
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
#include "types.h"
#include "scene.h"
#include "btltype.h"
#include "game_state.h"
#include "fieldrun.h"
#include "btlsys.h"
#include "battle.h"
#include "battle_unit.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "dma.h"
#include "scene_ids.h"
#include "io_reg.h"
#include "iowrite.h"
#include "irq.h"
#include "callback_scheduler.h"
#include "flag_ids.h"
#include "iwram_call.h"
#include "eventwrk.h"
#include "resource_ids.h"
#include "battle_effect_runtime.h"
#include "resource.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "event_runtime.h"
#include "field_sprite.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "ownerval.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "btlcalc.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"
#include "effect_runtime.h"
#include "object_effect.h"
#include "item.h"
#include "motobj.h"
#include "layout_guard.h"
#include "frame.h"
#include "io_write_queue.h"
#include "palbuf.h"
#include "vram_block.h"
#include "item_ids.h"
#include "inventory.h"
#include "ui.h"
#include "window.h"
#include "btlpty.h"
#include "b5_context.h"
#include "edition.h"
#include "facing_object.h"
#include "script_motion.h"
#include "tbs_edition.h"
#include "metadata_lookup.h"
#include "fieldcoo.h"
#include "scroll.h"
#include "fxblend.h"
#include "fx_scene.h"
#include "effect_slot.h"
#include "object_efx.h"
#include "menu_list.h"
#include "field_effect.h"
#include "fixed_point_position.h"

void Event_SetMessageId(s32 value)
{
	((struct EventRuntime *)gEventWork)->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
	struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
	s32					  result = -1;
	struct ObjectRuntime *entry = state->actors[key & 0x0fff];

	if (entry != 0 && entry->animation_kind == 1)
		result = ((struct AnimationObject *)entry->animation)->entries[0]->anim_id;
	return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
	struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
	s32					  result = -1;
	s32					  index = 8;
	struct ObjectRuntime *object = state->actors[index];

	if (object != 0 && object->animation_kind == 1
		&& ((struct AnimationObject *)object->animation)->entries[0]->anim_id == value)
	{
		result = index;
	}
	else
	{
	next:
		index++;
		if (index <= 65)
		{
			object = state->actors[index];
			if (object == 0 || object->animation_kind != 1
				|| ((struct AnimationObject *)object->animation)->entries[0]->anim_id != value)
				goto next;
			result = index;
		}
	}
	return result;
}

struct UiWindow *UiText_OpenMessageAtObject(s32 arg, s32 unused)
{
	struct UiRenderWork	 *win;
	struct EventRuntime	 *work;
	s32					  speaker;
	s32					  extra;
	s32					  tail;
	s32					  margin;
	s32					  height;
	s32					  width;
	s32					  top;
	s32					  left;
	s32					  pos[3];
	struct UiWindow		 *handle;
	s32					  side;
	s32					  flags;
	s32					  message;
	struct ObjectRuntime *object;
	s32					  x;
	s32					  y;
	s32					  ret;
	s32					  column;
#if !EDITION_INTERNATIONAL
	s32 none;
#endif

	win = (struct UiRenderWork *)gWindowWork[0];
	work = ((struct WorkPointers *)gWindowWork)->event;
	handle = 0;
	side = 0;
	speaker = ObjectTable_ReadActiveValue(arg);
	extra = 0;
	tail = 0;
	margin = 4;
	flags = arg & 0xf000;
	message = work->message;
	arg &= 0xfff;
	x = 0;
	object = ObjectTable_Get(arg);
	work->speaker = arg;
	y = 0;
	ret = 0;
	if (work->message_busy == 0)
	{
		if (object != 0)
		{
			if (work->mode_19e == 3)
			{
				Render_ProjectPoint(&object->x, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				ret = 1;
				y -= 2;
			}
			else
			{
				if (Object_GetScreenPosition(arg, pos) != -1)
					ret = 1;
				else
					ret = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		}
		else if (arg <= 7)
		{
			speaker = arg;
			object = ObjectTable_Get(gGameState.selected_actor);
			if (work->mode_19e == 3)
			{
				Render_ProjectPoint(&object->x, pos);
				x = pos[0] >> 3;
				y = pos[1] >> 3;
				ret = 1;
			}
			else
			{
				if (Object_GetScreenPosition(gGameState.selected_actor, pos) != -1)
					ret = 1;
				else
					ret = 0;
				x = pos[0] >> 3;
				y = pos[1] >> 3;
			}
		}
		if (!ret)
		{
			left = 15;
			top = 10;
		}
		else
		{
			left = 0;
			top = 0;
			UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			left = x - width / 2;
			if (flags & 0x4000)
				top = y - height - 1;
			else if (!(flags & 0x8000) && y <= 8)
				top = y - height - 1;
			else
				top = y + 4;
		}
		if (win->mode != 0)
			margin = 5;
		column = x;
		if (flags & 0x1000)
		{
			column = x - margin - 2;
			if (column < 0)
				column = 0;
		}
		else if (flags & 0x2000)
		{
			column += 2;
			if (column + margin > 29)
				column = 29 - margin;
		}
		else if (column <= 15)
		{
			column = column - margin - 2;
			if (column < 0)
				column = x + 2;
		}
		else
		{
			column += 2;
			if (column + margin > 29)
				column = x - margin - 2;
		}
#if EDITION_INTERNATIONAL
		ret = Localization_LookupEntryIdFar(speaker);
		if (ret != -1)
		{
			UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			message = -1;
			if (top > y)
				tail = top - 5;
			else
				tail = top + height;
			if (tail < 0)
				tail = top + height;
			else if (tail + 5 > 19)
				tail = top - 5;
			if (top < tail)
			{
				s32 lines = height;

				UiText_GetResourceDimensionsFar(
					message, &left, &top, (u32 *)&width, (u32 *)&height);
				message = -1;
				extra = lines - height + 1;
			}
		}
		else if (top < y)
		{
			s32 lines = height;

			UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			extra = lines - height + 1;
			message = ret;
		}
#else
		none = Localization_LookupEntryIdFar(speaker);
		ret = -1;
		if (none != ret)
		{
			UiText_GetResourceDimensionsAltFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			message = ret;
			if (top > y)
				tail = top - 5;
			else
				tail = top + height;
			if (tail < 0)
				tail = top + height;
			else if (tail + 5 > 19)
				tail = top - 5;
		}
		if (top < y)
		{
			s32 lines = height;

			UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
			message = -1;
			extra = lines - height + 1;
		}
#endif
		if (column < 0)
			column = 0;
		else if (column + margin > 29)
			column = 29 - margin;
		if (win->mode != 0)
		{
			WaitFrames(8);
			if (extra != 0)
				handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
			else
				handle = UiText_OpenMessageWindowFar(message, left, top, 2);
		}
		else
		{
			s32 icon = BattleFx_GetResourceId(speaker);

			if (extra != 0)
				handle =
					UiText_OpenMessageWindowFar(message, left, top + extra - 1, (icon << 16) | 17);
			else
				handle = UiText_OpenMessageWindowFar(message, left, top, (icon << 16) | 1);
		}
		if (win->mode != 0)
			side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
		else
			side = UiWindow_CreateWithSideObjectFar(speaker, 0, column, tail);
		while (!UiWork_IsCompleteFar())
			WaitFrames(1);
	}
	work->message_window = (s32)handle;
	work->side_window = side;
	work->message++;
	return handle;
}

void BattleEv_RunWait(s32 action, s32 flag)
{
	s32					 masked_action;
	struct EventRuntime *runtime = (struct EventRuntime *)gEventWork;
	struct UiWindow		*window = UiText_OpenMessageAtObject(action, flag);
	s32					 message_id;
	u32					 frames;

	frames = 0;
	WaitFrames(1);
	message_id = ObjectTable_ReadActiveValue(action);
	if (action <= 7)
	{
		masked_action = action & 0x0fff;

		if (BattleAction_FindDescriptor(masked_action) == 0)
		{
			message_id = masked_action;
		}
	}
	UiWork_FinalizeEntityMatchingLocalizedIdFar(message_id);

	if (runtime->message_busy == 0)
	{
		while (UiWork_IsIdleFar(window) == 0)
		{
			WaitFrames(1);
			frames++;
			if (frames > 600
				|| ((gKeysHeld & KEY_SELECT) && (gKeysHeld & KEY_R) && (gKeysHeld & KEY_L)
					&& (gKeysHeld & KEY_A)))
			{
				UiWork_FinalizePendingCoreFar();
			}
		}
	}

	WaitFrames(1);
}

void BattlePres_RunActionThenWaitIfModeZero(s32 first, s32 second, s32 value)
{
	BattleEv_RunWait(first, second);
	Battle_WaitMode0(value);
}

s32 BattleEventRuntime_ProcessAction(s32 object_id, s32 action_id)
{
	s32					 result;
	struct EventRuntime *runtime;
	struct GameState	*save;

	UiText_OpenMessageAtObject(object_id, action_id);
	save = &gGameState;
	result = Inventory_PromptAndSetObjectMode(save->selected_actor, 0);
	if (result == 0)
	{
		BattleEv_RunWait(object_id, action_id);
		runtime = (struct EventRuntime *)gEventWork;
		runtime->message += 1;
	}
	else
	{
		runtime = (struct EventRuntime *)gEventWork;
		runtime->message += 1;
		BattleEv_RunWait(object_id, action_id);
	}
	return result;
}

void BattleEventRuntime_EmptyCallback(void)
{
}

void Event_ShowSpeakerMessage(s32 speaker)
{
	// 位置は未初期化のまま読まれる (original bug: the position is never set)
	struct
	{
		s32 x, y;
	} pos;
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;

	speaker &= 0xfff;
	ObjectTable_Get(speaker);
	state->speaker = speaker;
	if (state->message_busy == 0)
	{
		s32 px = pos.x;
		s32 py = pos.y;
		s32 win;

		if (py > 119)
			py += 32;
		else
			py -= 32;
		px = px < 8 ? 8 : px;
		if (px > 312)
			px = 312;
		py = py < 20 ? 20 : py;
		if (py > 220)
			py = 220;
		win = UiText_OpenMessageWindowFar(state->message, px, py, 1);
		state->message_window = win;
		while (UiWork_IsIdleFar((struct UiWindow *)win) == 0)
			WaitFrames(1);
	}
	state->message++;
}

void ObjectTable_CallRefreshHook(s32 unused0, s32 unused1, s32 x, s32 y)
{
	Event_ShowMessageAtPosition(unused0, unused1, x, y);
}

void Event_ShowMessageAtPosition(s32 unused0, s32 unused1, s32 x, s32 y)
{
	struct EventRuntime *state = (struct EventRuntime *)gEventWork;
	s32					 py = y;
	s32					 px = x;
	s32					 min_x = 8;
	s32					 min_y = 20;
	s32					 ret;

	if (py > 119)
		py += 32;
	else
		py -= 32;

	if (x < min_x)
		px = min_x;
	if (px > 312)
		px = 312;
	if (py < min_y)
		py = min_y;
	if (py > 220)
		py = 220;

	ret = UiText_OpenMessageWindowFar(state->message, px, py, 1);
	while (UiWork_IsIdleFar((struct UiWindow *)ret) == 0)
		WaitFrames(1);
	state->message++;
}

//--------------------------------------------------------------------------
// table_run_if_active.c
// フィールド：話し手の表示の後しまつ
//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// アクターに絵があれば、その絵番号を話し手としてウィンドウの表示を終える
// id : アクター番号
//--------------------------------------------------------------------------
void ObjectTable_RunIfActive(s32 id)
{
	s32 no;

	no = ObjectTable_ReadActiveValue(id);
	if (no != -1)
		UiWork_FinalizeEntityMatchingLocalizedIdFar(no);
}

void Battle_ShowPairedUnitWorkAndWait(s32 first, s32 first_x, s32 first_y, s32 first_arg,
	s32 first_extra, s32 second, s32 second_x, s32 second_y, s32 second_arg, s32 second_extra)
{
	struct EventRuntime *rt = (struct EventRuntime *)gEventWork;
	s32					 id0 = ObjectTable_ReadActiveValue(first);
	s32					 id1 = ObjectTable_ReadActiveValue(second);
	s32					 h0;
	s32					 h1;

	h0 = UiText_OpenMessageWindowFar(
		rt->message++, first_x, first_y, BattleFx_GetResourceId(id0) << 16);
	UiWindow_CreateWithSideObjectFar(id0, 0, first_arg, first_extra);

	h1 = UiText_OpenMessageWindowFar(
		rt->message++, second_x, second_y, BattleFx_GetResourceId(id1) << 16);
	UiWindow_CreateWithSideObjectFar(id1, 0, second_arg, second_extra);

	while (UiWork_IsCompleteFar() == 0)
		WaitFrames(1);

	WaitFrames(1);
	while ((gKeyState & (KEY_A | KEY_B | KEYS_SHOULDERS)) == 0)
		WaitFrames(1);

	WaitFrames(1);
	UiWork_FinalizeEntityMatchingLocalizedIdFar(id0);
	UiWork_FinalizeEntityMatchingLocalizedIdFar(id1);
	UiWork_FinalizePendingCoreFar();
	WaitFrames(1);

	while (UiWork_IsIdleFar((struct UiWindow *)h0) == 0)
		WaitFrames(1);
	while (UiWork_IsIdleFar((struct UiWindow *)h1) == 0)
		WaitFrames(1);

	WaitFrames(1);
}

void Ui_SetRenderResultFromObject(s32 arg0)
{
	struct UiRenderWork *state = (struct UiRenderWork *)gWindowWork[0];
	u8					 value;
	s32					 ret;

	if (arg0 == 0x80000000)
	{
		*(s16 *)&state->result[0] = (value = 0);
		*(s16 *)&state->result[1] = value;
	}
	else
	{
		ret = BattleFx_GetResourceId(ObjectTable_ReadActiveValue(arg0));
		value = Ui_RenderResultValues[gGameState.unknown_207[5]];
		*(s16 *)&state->result[0] = ret;
		*(s16 *)&state->result[1] = value;
	}
}
