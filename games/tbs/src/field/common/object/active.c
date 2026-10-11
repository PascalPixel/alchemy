#include "object_runtime.h"
#include "objdisp.h"
#include "fieldobj.h"
#include "field_event.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "event_runtime.h"
#include "types.h"
#include "animspr.h"
#include "fieldrun.h"
#include "game_state.h"
#include "window.h"
#include "ui.h"
#include "keystate.h"
#include "io_reg.h"
#include "global_cells.h"

struct UiWindow *UiText_OpenMessageAtObject(s32 arg);

void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

void ObjectDispatch_InitFromTable6Far(struct ObjectRuntime *);
extern const u8 ObjectMotion_StepAngleScript[];

void Object_SetTargetAndCallback(u32 object_id, s32 target_id, const void *callback)
{
    struct ObjectRuntime *first = ObjectTable_Get(object_id);
    struct ObjectRuntime *second = ObjectTable_Get(target_id & 0xff);

    if (first != NULL && second != NULL) {
        first->linked_object = second;
        if (!(target_id & 0x10000)) {
            first->action = 40;
            first->acceleration = second->acceleration * 2;
            first->speed_limit = second->speed_limit;
            first->unknown_59 = 0;
        }
        ObjectDispatch_InitializeFar((struct DispatchObject *)first, (u32)callback);
    }
}

s32 ObjectMotion_StepAngle(struct ObjectRuntime *object)
{
    s32 delta = 0;

    if (object != NULL) {
        s32 target_angle = (u16)object->action;
        s32 current_angle = object->angle;
        delta = (s16)(target_angle - current_angle);
        if (delta != 0) {
            if (delta > 4096)
                delta = 2048;
            if (delta < -4096)
                delta = -2048;
            object->angle = current_angle + delta;
        }
    }
    return delta;
}

void Object_ResetTargetAndSetMode1(u32 object_id)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->target_x = 0x80000000;
        object->target_y = 0x80000000;
        object->target_z = 0x80000000;
        ObjectDispatch_InitFromTable6Far(object);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
    }
}

void ObjectMotion_ArmCallback(s32 object_id, s32 angle, s32 wait)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL) {
        object->action = angle;
        ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)ObjectMotion_StepAngleScript);
        Battle_WaitMode0(wait);
    }
}

void ObjectMotion_SetActionVariant(s32 object_id, s32 priority)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);

    if (object != NULL && (object->animation_kind & 0xF) == 1) {
        struct FieldSprite *sprite = object->animation;

        sprite->priority = priority;
        sprite->second_priority = priority;
        object->unknown_23 &= ~ACTOR_PRIORITY_AUTOMATIC;
    }
}

void ObjectVisual_CopyAttributes(u32 target_id, u32 source_id)
{
    void *p;
    u8 flags;
    u32 shape;

    p = Object_GetById(source_id);
    p = ((struct ObjectRuntime *)p)->animation;
    flags = ((struct FieldSprite *)p)->vram_block;
    shape = ((struct FieldSprite *)p)->tile;

    p = Object_GetById(target_id);
    p = ((struct ObjectRuntime *)p)->animation;
    ((struct FieldSprite *)p)->vram_block = flags;
    ((struct FieldSprite *)p)->tile = shape;
}

void ObjectVisual_ReservedNoOp(void)
{
}

#define BattleFx_GetResourceId ((s32 (*)(s32))BattleFx_GetResourceId)
#define gEventWork (*(struct FieldStepWork *(*))&gEventWork)

void Event_SetMessageId(s16 value)
{
    ((struct EventRuntime *)gEventWork)->message = value;
}

s32 ObjectTable_ReadActiveValue(s32 key)
{
    struct FieldStepWork *state = gEventWork;
    s32 result = -1;
    struct ObjectRuntime *entry = state->actors[key & 0x0fff];

    if (entry != 0 && entry->animation_kind == 1)
        result = ((struct AnimationObject *)entry->animation)->entries[0]->anim_id;
    return result;
}

s32 ObjectTable_FindActiveByValue(s32 value)
{
    struct FieldStepWork *state = gEventWork;
    s32 result = -1;
    s32 index = 8;
    struct ObjectRuntime *object = state->actors[index];

    if (object != 0 && object->animation_kind == 1 && ((struct AnimationObject *)object->animation)->entries[0]->anim_id == value) {
        result = index;
    } else {
    next:
        index++;
        if (index <= 65) {
            object = state->actors[index];
            if (object == 0 || object->animation_kind != 1 ||
                ((struct AnimationObject *)object->animation)->entries[0]->anim_id != value)
                goto next;
            result = index;
        }
    }
    return result;
}

void WaitFrames(s32 frames);

struct UiWindow *UiText_OpenMessageAtObject(s32 arg)
{
    struct UiRenderWork *win;
    struct EventRuntime *work;
    s32 speaker;
    s32 extra;
    s32 tail;
    s32 margin;
    s32 height;
    s32 width;
    s32 top;
    s32 left;
    s32 pos[3];
    struct UiWindow *handle;
    s32 side;
    s32 flags;
    s32 message;
    struct ObjectRuntime *object;
    s32 x;
    s32 y;
    s32 ret;
    s32 column;
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
    if (work->message_busy == 0) {
        if (object != 0) {
            if (work->mode_19e == 3) {
                Render_ProjectPoint(&object->x, pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                ret = 1;
                y -= 2;
            } else {
                if (Object_GetScreenPosition(arg, pos) != -1)
                    ret = 1;
                else
                    ret = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        } else if (arg <= 7) {
            speaker = arg;
            object = ObjectTable_Get(gGameState.selected_actor);
            if (work->mode_19e == 3) {
                Render_ProjectPoint(&object->x, pos);
                x = pos[0] >> 3;
                y = pos[1] >> 3;
                ret = 1;
            } else {
                if (Object_GetScreenPosition(gGameState.selected_actor, pos) != -1)
                    ret = 1;
                else
                    ret = 0;
                x = pos[0] >> 3;
                y = pos[1] >> 3;
            }
        }
        if (!ret) {
            left = 15;
            top = 10;
        } else {
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
        if (flags & 0x1000) {
            column = x - margin - 2;
            if (column < 0)
                column = 0;
        } else if (flags & 0x2000) {
            column += 2;
            if (column + margin > 29)
                column = 29 - margin;
        } else if (column <= 15) {
            column = column - margin - 2;
            if (column < 0)
                column = x + 2;
        } else {
            column += 2;
            if (column + margin > 29)
                column = x - margin - 2;
        }
#if EDITION_INTERNATIONAL
        ret = Localization_LookupEntryIdFar(speaker);
        if (ret != -1) {
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
            if (top < tail) {
                s32 lines = height;

                UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
                message = -1;
                extra = lines - height + 1;
            }
        } else if (top < y) {
            s32 lines = height;

            UiText_GetResourceDimensionsFar(message, &left, &top, (u32 *)&width, (u32 *)&height);
            extra = lines - height + 1;
            message = ret;
        }
#else
        none = Localization_LookupEntryIdFar(speaker);
        ret = -1;
        if (none != ret) {
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
        if (top < y) {
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
        if (win->mode != 0) {
            WaitFrames(8);
            if (extra != 0)
                handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1, 18);
            else
                handle = UiText_OpenMessageWindowFar(message, left, top, 2);
        } else {
            s32 icon = BattleFx_GetResourceId(speaker);

            if (extra != 0)
                handle = UiText_OpenMessageWindowFar(message, left, top + extra - 1,
                                                     (icon << 16) | 17);
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

#undef gEventWork

#define ObjectTable_ReadActiveValue ((int (*)())ObjectTable_ReadActiveValue)

struct UiWindow;
#define UiText_OpenMessageAtObject ((struct UiWindow *(*)(s32 actor, s32 flags))UiText_OpenMessageAtObject)

void						Battle_WaitMode0(s32 mode);
#define gEventWork (*(struct EventRuntime *(*))&gEventWork)
s32							Inventory_PromptAndSetObjectMode(s32 actor, s32 force);

void BattleEv_RunWait(s32 action, s32 flag)
{
	s32					 masked_action;
	struct EventRuntime *runtime = gEventWork;
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
		runtime = gEventWork;
		runtime->message += 1;
	}
	else
	{
		runtime = gEventWork;
		runtime->message += 1;
		BattleEv_RunWait(object_id, action_id);
	}
	return result;
}

void BattleEventRuntime_EmptyCallback(void)
{
}

#undef ObjectTable_ReadActiveValue
#undef UiText_OpenMessageAtObject
#undef gEventWork
