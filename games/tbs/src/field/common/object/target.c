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
#include "object_lookup.h"
#include "types.h"
#include "field_event.h"
#include "object_effect.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "map_scroll.h"
#include "event_runtime.h"
#include "animspr.h"
#include "fieldobj.h"
#include "metadata_lookup.h"
#include "objdisp.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "iwram_call.h"
#include "fixed_math.h"
#include "battle_effect_runtime.h"
#include "heap_state.h"
#include "game_state.h"
#include "script_object_runtime.h"
#include "field_sprite.h"
#include "btlpty.h"
#include "b5_context.h"
#include "ram_buffer.h"
#include "map.h"
#include "fieldcoo.h"
#include "fieldrun.h"
#include "dma.h"
#include "io_reg.h"
#include "scroll.h"
#include "btlsys.h"
#include "disptran.h"
#include "resource.h"
#include "fxblend.h"

void Object_AttachWorkTargetToObject(s32 id, s32 flag)
{
    struct ObjectRuntime *obj;
    struct ObjectRuntime *target;
    struct EventWork *work;
    struct MapScrollWork *p;

    obj = ObjectTable_Get(id);
    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    target = (struct ObjectRuntime *)work->view_center;
    p = gMapWork[0];
    if (obj != 0) {
        p->origin = &target->x;
        ObjectDispatch_InitFromTable4WithArgumentFar(target, (void *)obj);
        if (flag == 0) {
            target->x = obj->x;
            target->y = obj->y;
            target->z = obj->z;
            WaitFrames(1);
            if (((struct EventRuntime *)work)->mode_19e != 3) {
                Map_ApplyWorkOriginAndSpanFar();
            }
        }
    }
}

void ObjectTable_AllocateAndSetObjectSpeed(s32 first, s32 second)
{
    struct EventWork *owner = Runtime_AllocateBlock(0x1b, 0xccc);
    ((struct ObjectRuntime *)owner->view_center)->speed_limit = first;
    ((struct ObjectRuntime *)owner->view_center)->acceleration = second;
}

void ObjectDispatch_InitFromTable6Far(void *);

void Motion_CamBounds(s32 requested_x, s32 requested_y, s32 requested_z, s32 use_setter)
{
    s32 should_use_setter;
    struct EventWork *runtime_block;
    s32 minimum_x;
    s32 minimum_z;
    s32 maximum_x;
    s32 object_z_offset;
    s32 maximum_z;
    s32 position_x;
    s32 position_z;
    s32 position_y;
    struct MapScrollWork *camera_state;
    struct ObjectRuntime *object;

    position_x = requested_x;
    position_y = requested_y;
    should_use_setter = use_setter;
    position_z = requested_z;
    runtime_block = Runtime_AllocateBlock(0x1B, 0xCCC);
    object = (struct ObjectRuntime *)runtime_block->view_center;
    camera_state = gMapWork[0];
    minimum_x = camera_state->min_x + 0x780000;
    object_z_offset = object->y;
    minimum_z = camera_state->min_y + object_z_offset + 0x600000;
    maximum_x = camera_state->max_x + 0xFF880000;
    maximum_z = camera_state->max_y + object_z_offset + 0xFFC00000;
    camera_state->origin = &object->x;
    ObjectDispatch_InitFromTable6Far(object);
    if (position_x == -1) {
        position_x = object->x;
    }
    if (position_y == -1) {
        position_y = object->y;
    }
    if (position_z == -1) {
        position_z = object->z;
    }
    if (position_x < minimum_x) {
        position_x = minimum_x;
    }
    if (position_z < minimum_z) {
        position_z = minimum_z;
    }
    if (position_x > maximum_x) {
        position_x = maximum_x;
    }
    if (position_z > maximum_z) {
        position_z = maximum_z;
    }
    if (should_use_setter == 0) {
        object->x = position_x;
        object->y = position_y;
        object->z = position_z;
        WaitFrames(1U);
        if (((struct EventRuntime *)runtime_block)->mode_19e != 3) {
            Map_ApplyWorkOriginAndSpanFar();
        }
    } else {
        Object_SetMoveTargetFar(object, position_x, position_y, position_z);
    }
}

void Object_PlaceCurrentWithinCameraBounds(s32 arg0, s32 arg1)
{
    struct ObjectRuntime *obj;

    obj = ObjectTable_Get();
    Runtime_AllocateBlock(0x1B, 0xCCC);
    if (obj != NULL) {
        Motion_CamBounds(obj->x, -1, obj->z, arg1);
    }
}

void Script_WaitForEventTimeoutFar(struct ObjectRuntime *);

void BattleFx_CommitObjectPositionAndWait(void)
{
    Script_WaitForEventTimeoutFar((struct ObjectRuntime *)
        ((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center);
    Battle_WaitMode0(2);
}

s32 Battle_GetWorkObject1e0(void)
{
    return (s32)((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center;
}

void BattleFx_LinkObjectToTarget(struct ObjectRuntime *target, s32 keep_current_position)
{
    struct ObjectRuntime *object;

    object = (struct ObjectRuntime *)
        ((struct EventWork *)Runtime_AllocateBlock(0x1B, 0xCCC))->view_center;
    if (target != NULL) {
        ObjectDispatch_InitFromTable4WithArgumentFar(object, NULL);
        object->linked_object = target;
        if (keep_current_position == 0) {
            object->x = target->x;
            object->y = target->y;
            object->z = target->z;
        }
    }
}

//--------------------------------------------------------------------------
// mapfx.c
// マップの表示範囲とズームの移り変わり、ほかのオブジェクトについて回るオブジェクト
//--------------------------------------------------------------------------

#define WaitFrames ((s32 (*)(s32))WaitFrames)

#define gScanlinePitch ((u8 *)&gScanlinePitch)

// スプライトの OAM 属性2 の上位バイト (bit 2〜3 が表示優先度)
#define ATTR2_HI(spr) (((u8 *)&((struct AnimationObject *)(spr))->part[0])[9])

s16 *BattleAction_FindDescriptor(s16 action);
void			   ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 mode);
void			   AudioCommand_PlayFar(s32 cue);
void			   Battle_WaitMode0(s32 state);

//--------------------------------------------------------------------------
// マップの表示範囲を決める
//--------------------------------------------------------------------------
void Map_SetViewBounds(s32 min_x, s32 min_y, s32 max_x, s32 max_y)
{
	struct MapScrollWork *work = gMapWork[0];

	work->min_x = min_x;
	work->min_y = min_y;
	work->max_x = max_x;
	work->max_y = max_y;
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : ズームを前の値から目標へ 1 段すすめる
// 画面中心のオブジェクトが止まっているあいだは待つ
//--------------------------------------------------------------------------
s32 BattleFx_StepRatioTransition(void)
{
	struct PerspectiveWork *work;
	s16					   *frames;
	s32					   *from;
	s16					   *step;
	s32						zoom;
	s32						delta;

	work = gMapWork[0];
	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->view_center->paused
		!= 0)
		return;
	frames = (s16 *)&work->zoom_transition_frames;
	if (*frames == 0)
		return;
	from = &work->previous_zoom;
	delta = work->zoom - *from;
	step = &work->zoom_transition_step;
	(*step)++;
	zoom = *from + __divsi3(delta * *step, *frames);
	work->distance = Iwram_MulQ16(work->far_plane, zoom);
	*(u32 *)gScanlinePitch = work->pitch + 1;
	if (*step == *frames)
	{
		*frames = 0;
		Scheduler_RemoveCallback((SchedulerCallback)BattleFx_StepRatioTransition);
	}
}

//--------------------------------------------------------------------------
// ズームの移り変わりを始める (イベントの mode が 3 のときだけ)
// zoom   : 倍率 (16.16 固定小数)
// frames : かけるフレーム数
//--------------------------------------------------------------------------
void BattleFx_ScheduleRatioTransition(s32 zoom, s32 frames)
{
	struct PerspectiveWork *work = gMapWork[0];
	s32						inv;

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode != 3)
		return;
	{
		inv = Iwram_RatioMulQ14(zoom, 0x10000); // 1 / zoom
	}
	work->previous_zoom = work->zoom;
	work->zoom = inv;
	work->zoom_transition_frames = frames;
	work->zoom_transition_step = 0;
	Scheduler_AddOrUpdateCallback((SchedulerCallback)BattleFx_StepRatioTransition, 0xc94);
}

//--------------------------------------------------------------------------
// ズームの移り変わりが終わるまで待つ (イベントの mode が 3 のとき、最長 300 フレーム)
//--------------------------------------------------------------------------
void Event_WaitForZoomTransition(void)
{
	s32						frames;
	struct PerspectiveWork *work = gMapWork[0];

	if (((struct EventWork *)Runtime_AllocateBlock(HEAP_SLOT_EVENT, 0xccc))->mode == 3)
	{
		frames = 0;
		if (*(s16 *)&work->zoom_transition_frames != 0)
		{
			do
			{
				WaitFrames(1);
				frames++;
			} while (frames < 300 && *(s16 *)&work->zoom_transition_frames != 0);
		}
	}
}

//--------------------------------------------------------------------------
// 毎フレームの処理 : 付いていく相手の位置に置き、y には絵の box_x と 8 ドットを足す
//--------------------------------------------------------------------------
s32 BattleFx_CopyLinkedObjectPosition(void *self)
{
	struct FieldActor *obj = self;
	struct FieldActor *link;

	link = obj->linked_object;
	if (link != NULL)
	{
		obj->motion_flags = 0;
		obj->x.fixed = link->x.fixed;
		obj->y.fixed =
			link->y.fixed
			+ ((s8)Resource_GetMetadataRecordFar(*BattleAction_FindDescriptor(*(s16 *)&obj->work))
					->box_x
				<< 16)
			+ (8 << 16);
		obj->terrain_height = link->terrain_height;
		obj->z.fixed = link->z.fixed;
	}
	return 0;
}

//--------------------------------------------------------------------------
// id のオブジェクトについて回るオブジェクト (種類 21) を出し、wait フレーム待つ
// mode : 下位 4 ビットはオブジェクトの mode、下位 8 ビットが 6 なら音 110、
//        0x100 で表示優先度 1 (なければ相手と同じ優先度)
//--------------------------------------------------------------------------
void BattleFx_SpawnLinked(s32 id, s32 mode, s32 wait)
{
	struct FieldActor *target;

	if ((mode & 0xff) == 6)
	{
		AudioCommand_PlayFar(110);
	}

	target = ObjectTable_Get(id);
	if (target != 0)
	{
		struct FieldActor *obj =
			Object_CreateFar(21, target->x.fixed, target->y.fixed, target->z.fixed);

		if (obj != 0)
		{
			ObjectDispatch_InitializeFar((struct DispatchObject *)obj, BattleFx_LinkedObjectScript);
			ObjectDispatch_ApplyArgumentToChildrenFar(obj, mode & 15);
			obj->motion_flags = 0;
			obj->timer = 0;
			obj->work = id;
			obj->update = (void (*)(union FieldObject *))BattleFx_CopyLinkedObjectPosition;
			obj->sprite->flags = 0;
			obj->linked_object = target;

			if ((mode & 0x100) != 0)
			{
				s32 mask = 13;
				u8	attr = ATTR2_HI(obj->sprite);

				mask = -mask;
				mask &= attr;
				mask |= 4;
				ATTR2_HI(obj->sprite) = mask;
			}
			else
			{
				s32 pri = 12;
				u8	src = ATTR2_HI(target->sprite);
				u8	dst;
				s32 mask = 13;

				pri &= src;
				dst = ATTR2_HI(obj->sprite);
				mask = -mask;
				mask &= dst;
				mask |= pri;
				ATTR2_HI(obj->sprite) = mask;
			}
		}
		Battle_WaitMode0(wait);
	}
}

void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);
void ObjectDispatch_ReleaseFar(struct DispatchObject *work);

#define ObjectDispatch_ReleaseFar ((void (*)(void*))ObjectDispatch_ReleaseFar)

struct LinkedEffectObject {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[60];
    struct FieldSprite *visual;
    u8 value_54;
    u8 value_55;
    u8 unknown_56[14];
    u16 counter;
    u16 resource_id;
    struct LinkedEffectObject *resource;
    s32 (*callback)(void *);
};

void ObjectDispatch_SetSingleChildFlagsFar(void *, s32);
s32 ArcTan2(s32, s32);
void Object_SetMoveTargetFar(struct ObjectRuntime *, s32, s32, s32);

void BattleFx_ConfigureLinkedObject(s32 id, s32 flags)
{
    struct LinkedEffectObject *object = ObjectTable_Get(id);
    struct LinkedEffectObject *child;
    struct FieldSprite *visual;
    s32 mode;

    child = 0;
    visual = 0;

    if (object == 0)
        return;

    if ((flags & 3) != 0) {
        if ((flags & 3) == 2 || object->resource == 0) {
            child = Object_CreateFar(209, object->x, object->y, object->z);
        }
    } else {
        child = object->resource;
        if (child == 0)
            return;
        ObjectDispatch_ReleaseFar(child);
        object->resource = 0;
        return;
    }

    if (child == 0)
        return;

    mode = flags & 3;
    switch (mode) {
    case 1:
        ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)child, 1);
        object->resource = child;
        child->counter = 1;
        break;
    case 2:
        ObjectDispatch_ApplyArgumentToChildrenFar((struct ObjectRuntime *)child, 2);
        ObjectDispatch_InitializeFar((struct DispatchObject *)child, (u32)ObjectMotion_WaitScript);
        child->counter = 1;
        break;
    }

    child->resource_id = id;
    child->value_55 = 0;
    child->callback = BattleFx_CopyLinkedObjectPosition;
    visual = child->visual;
    visual->flags = 0;
    child->resource = object;

    if (flags & 0x100) {
        s32 mask = 13;
        u8 visual_flags = ((u8 *)visual)[9];

        mask = -mask;
        mask &= visual_flags;
        mask |= 4;
        ((u8 *)visual)[9] = mask;
    } else {
        s32 copied_flags = 12;
        u8 source_flags = ((u8 *)object->visual)[9];
        u8 destination_flags;
        s32 clear_mask = 13;

        copied_flags &= source_flags;
        destination_flags = ((u8 *)visual)[9];
        clear_mask = -clear_mask;
        clear_mask &= destination_flags;
        clear_mask |= copied_flags;
        ((u8 *)visual)[9] = clear_mask;
    }
}

s32 Object_ClearChildAndCollisionFlags(void *obj)
{
    ObjectDispatch_SetSingleChildFlagsFar(obj, 0);
    ((struct ScriptObjectRuntime *)obj)->flags_59 = 0;
    return 0;
}

s32 ObjectMotion_MoveTowardTarget(struct ObjectRuntime *object)
{
    s32 step;
    struct ObjectRuntime *target;
    s32 deltaX;
    s32 deltaY;
    s32 cellX;
    s32 cellY;
    s32 newX;
    s32 distance;

    target = object->linked_object;
    if (target != 0) {
        deltaX = target->x - object->x;
        if (deltaX < 0)
            deltaX += 0xffff;
        cellX = deltaX >> 16;
        deltaY = target->z - object->z;
        if (deltaY < 0)
            deltaY += 0xffff;
        cellY = deltaY >> 16;
        distance = Iwram_Sqrt(cellX * cellX + cellY * cellY);
        step = object->action;
        if (distance >= step) {
            newX = object->x +
                (cellX << 20) / step;
            Object_SetMoveTargetFar(object, newX, object->y,
                          object->z +
                              __divsi3(cellY << 20, step));
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
        } else {
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
        }
    }
    return 1;
}

s32 ObjectMotion_TurnTowardLinkedTarget(struct ObjectRuntime *object)
{
    s16 turn_step;
    u16 current_angle;
    s32 target_angle;
    struct ObjectRuntime *target;

    target = object->linked_object;
    if (target != NULL) {
        object->action_flags &= 0xfe;
        target_angle = (u16)ArcTan2(target->z - object->z, target->x - object->x);
        current_angle = object->angle;
        turn_step = target_angle - current_angle;
        if (turn_step != 0) {
            if (turn_step > 0x1000)
                turn_step = 0x1000;
            if (turn_step < -0x1000)
                turn_step = -0x1000;
            object->angle = (u16)(current_angle + turn_step);
        }
    }
    return 1;
}

void ObjectMotion_SetActionCallback(struct ObjectRuntime *object, s32 kind)
{
    switch ((u32)(kind - 1)) {
    case 0:
        kind = (s32)ObjectMotion_ActionKind1Script;
        break;
    case 1:
        kind = (s32)ObjectMotion_ActionKind2Script;
        break;
    case 2:
        kind = (s32)ObjectMotion_ActionKind3Script;
        break;
    case 3:
        kind = (s32)ObjectMotion_ActionKind4Script;
        break;
    case 4:
        kind = (s32)ObjectMotion_MoveTowardTargetScript;
        break;
    case 5:
        object->linked_object = Object_GetById(gGameState.selected_actor);
        kind = (s32)ObjectMotion_TurnTowardLinkedScript;
        break;
    case 6:
        kind = (s32)ObjectMotion_ResetActionScript;
        break;
    default:
        break;
    }
    ObjectDispatch_InitializeFar((struct DispatchObject *)object, (u32)kind);
}

struct ObjectRuntime *Object_FindNearestFacingTarget(struct ObjectRuntime *self, s32 id)
{
    struct ObjectRuntime *entry;
    struct ObjectRuntime *found;
    s32 cnt;
    s32 best;
    s32 dy;
    s32 dx;
    s32 dz;
    s32 dist;
    s32 angle;
    s32 turn;

    found = NULL;
    best = 40;
    entry = gObjectSlots;
    for (cnt = 0; cnt < 64; cnt++, entry++) {
        if (entry->script == NULL)
            continue;
        if (entry == self)
            continue;
        if (entry->animation_kind != 1)
            continue;
        dy = entry->y - self->y;
        if (dy >= 0) {
            if (dy > 0x2fffff)
                continue;
        } else {
            if (self->y - entry->y > 0x2fffff)
                continue;
        }
        dx = (entry->x - self->x) / 0x10000;
        dz = (entry->z - self->z) / 0x10000;
        dist = Iwram_Sqrt(dx *dx + dz *dz);
        if (dist >= best)
            continue;
        angle = (u16)ArcTan2(entry->z - self->z, entry->x - self->x);
        if (dist > 23) {
            turn = (s16)(angle - self->angle);
            if (turn < -0x2fff)
                continue;
            if (turn > 0x2fff)
                continue;
        }
        found = entry;
        best = dist;
    }
    if (found == NULL)
        return NULL;
    if (((struct AnimationObject *)found->animation)->entries[0]->anim_id != id)
        return NULL;
    return found;
}

#undef WaitFrames

void Vector_AddPolarOffset(s32 distance, s32 angle, struct FieldPosition *position);
s32 Map_CheckObjectStepFar(struct ObjectRuntime *actor, struct FieldPosition *position);
void Battle_Reset(void);
void WaitFrames(s32 frames);
void BattleFx_FinishAction(void);

#define TILE_CELLS ((struct MapCell *)Ram_MapCellBuffer)
#define TILE_CELLS_TARGET ((struct MapCell *)(Ram_MapCellBuffer + 0x200))
#define TILE_CELLS_ABOVE ((struct MapCell *)(Ram_MapCellBuffer - 0x200))
void ObjectMotion_ArmCallback(s32, s32, s32);
void Object_RefreshSelectorById(s32);
void ObjectMotion_CommitCurrentPositionAndActivate(s32);
void Battle_WaitMode0(s32);

s32 Field_TryJumpForward(void)
{
    struct ObjectRuntime *leader = Object_GetById(gGameState.selected_actor);
    s32 result = -1;
    s32 angle = (leader->angle + 0x2000) & 0xc000;
    u8 *flags = &leader->flags;
    u8 saved = *flags;
    struct FieldStepWork *work = gEventWork;
    s32 child_mode = 1;
    struct FieldPosition position;
    struct FieldPosition *pos;
    struct ObjectRuntime *target;

    pos = &position;
again:
    pos->x = (leader->x & 0xfff00000) + 0x80000;
    pos->y = leader->y;
    pos->z = (leader->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x100000, angle, pos);
    if (Map_CheckObjectStepFar(leader, pos) == 1)
        return -1;
    pos->x = (leader->x & 0xfff00000) + 0x80000;
    pos->y = leader->y;
    pos->z = (leader->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, angle, pos);
    if (Map_CheckObjectStepFar(leader, pos) != 0)
        goto done;
    if (leader->animation_kind == 1)
        child_mode = ((struct FieldSprite *)leader->animation)->flags;
    Battle_Reset();
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
    WaitFrames(6);
    AudioCommand_PlayFar(152);
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 7);
    leader->speed_limit = 0x30000;
    leader->acceleration = 0x20000;
    leader->velocity_y = 0x40000;
    *flags &= 0x7e;
    ObjectDispatch_SetSingleChildFlagsFar(leader, child_mode & 0xfe);
    ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, ((union FieldCoordinate *)&pos->x)->part.pixel, ((union FieldCoordinate *)&pos->z)->part.pixel);
    ObjectDispatch_ApplyArgumentToChildrenFar(leader, 6);
    ObjectDispatch_SetSingleChildFlagsFar(leader, child_mode);
    if ((target = Object_FindNearestFacingTarget(leader, 207)) != NULL
        || (target = Object_FindNearestFacingTarget(leader, 205)) != NULL) {
        ObjectDispatch_ApplyArgumentToChildrenFar(target, 7);
        leader->y -= 0x10000;
        leader->terrain_height -= 0x10000;
        WaitFrames(2);
        leader->y -= 0x10000;
        leader->terrain_height -= 0x10000;
        WaitFrames(10);
        leader->y += 0x10000;
        leader->terrain_height += 0x10000;
        WaitFrames(4);
        leader->y += 0x10000;
        leader->terrain_height += 0x10000;
    } else {
        WaitFrames(6);
    }
    *flags = saved;
    BattleFx_FinishAction();
    if (work != NULL)
        work->encounter_steps += Iwram_MulQ16(work->encounter_rate, 0x200000);
    if (GetMapCellCollisionFar(leader->terrain_id, pos->x, pos->z) == 0xf9) {
        ObjectDispatch_ApplyArgumentToChildrenFar(leader, 1);
        WaitFrames(6);
        goto again;
    }
    result = 0;
done:
    return result;
}

s32 FieldEffect_UpdateGridPlacement(void)
{
    struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
    s32 grid_x = 8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
    s32 grid_z = 8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
    s32 tile_x = grid_x - 8;
    s32 tile_z = grid_z - 8;
    s32 result;

    Battle_Reset();

    if (gGameState.movement_mode == 0) {
        s32 index = grid_x / 16 + (grid_z / 16) * 128;

        if (TILE_CELLS[index].collision_code == TILE_CELLS_TARGET[index].collision_code) {
            /* FAKEMATCH: needed to match: the six-word probe preserves the 24-byte frame; a three-word position halves it. */
            s32 vector[6];
            struct FieldPosition *position = (struct FieldPosition *)vector;

            position->x = object->x;
            position->y = object->y - 0x100000;
            position->z = object->z;
            result = CheckMapPositionCellOccupiedFar((const s32 *)position);
            if (result != 0)
                goto failure;

            ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
            object->speed_limit = 0x10000;
            ObjectMotion_ArmCallback(gGameState.selected_actor, 0xc000, 0);
            Object_RefreshSelectorById(gGameState.selected_actor);
            object->action_flags = 1;
            object->flags = 0;
            ObjectDispatch_SetSingleChildFlagsFar(object, 0);
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 13);
            Object_SetMoveTargetFar(object, grid_x << 16,
                object->y - 0x100000, (grid_z << 16) + 0x100000);
            ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
            gGameState.movement_mode = 1;
        } else {
            goto failure;
        }
    } else {
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 10);
        object->flags = 3;
        object->velocity_y = 0x40000;
        object->terrain_height = object->y;
        ObjectDispatch_SetSingleChildFlagsFar(object, 1);
        Battle_WaitMode0(6);
        gGameState.movement_mode = 0;
        object->action_flags = 1;
        object->angle = 0xc000;
    }

    BattleFx_FinishAction();
    return 0;

failure:
    BattleFx_FinishAction();
    return -1;
}

s32 battle_owner_69(void)
{
    struct ObjectRuntime *object = Object_GetById(gGameState.selected_actor);
    s16 child_mode = 1;
    s32 grid_x = 8 + ((((union FieldCoordinate *)&object->x)->part.pixel) & 0xfff0);
    s32 grid_z = 8 + ((((union FieldCoordinate *)&object->z)->part.pixel) & 0xfff0);
    s32 tile_x = grid_x - 8;
    s32 tile_z = grid_z - 8;
    s32 result;

    Battle_Reset();

    if (object->animation_kind == 1)
        child_mode = ((struct FieldSprite *)object->animation)->flags;

    if (gGameState.movement_mode == 0) {
        s32 index = grid_x / 16 + (grid_z / 16) * 128;

        if (TILE_CELLS[index].collision_code == TILE_CELLS_ABOVE[index].collision_code) {
            /* FAKEMATCH: needed to match: the six-word probe preserves the 24-byte frame; a three-word position halves it. */
            s32 vector[6];
            struct FieldPosition *position = (struct FieldPosition *)vector;

            position->x = object->x;
            position->y = object->y;
            position->z = object->z;
            result = CheckMapPositionCellOccupiedFar((const s32 *)position);
            if (result != 0)
                goto failure;

            object->action_flags = 0;
            ObjectMotion_SetPositionAndCommit(gGameState.selected_actor, grid_x, grid_z);
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 6);
            WaitFrames(4);
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 7);
            object->velocity_y = 0x40000;
            WaitFrames(4);
            object->flags = 0;
            child_mode &= 0xfe;
            ObjectDispatch_SetSingleChildFlagsFar(object, child_mode);
            object->speed_limit = 0x10000;
            object->velocity_y = 0;
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
            WaitFrames(4);
            gGameState.movement_mode = 1;
            object->action_flags = 1;
            WaitFrames(8);
        } else {
            goto failure;
        }
    } else {
        object->flags = 0;
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 11);
        Object_SetMoveTargetFar(object, grid_x << 16, object->y + 0x80000,
            (grid_z << 16) - 0x100000);
        ObjectMotion_CommitCurrentPositionAndActivate(gGameState.selected_actor);
        object->flags = 3;
        child_mode |= 1;
        object->terrain_height = object->y;
        ObjectDispatch_SetSingleChildFlagsFar(object, child_mode);
        Battle_WaitMode0(4);
        gGameState.movement_mode = 0;
        object->action_flags = 1;
    }

    BattleFx_FinishAction();
    return 0;

failure:
    BattleFx_FinishAction();
    return -1;
}

s32 Object_GetScreenPosition(s32 object_id, s32 *position)
{
    struct ObjectRuntime *object = ObjectTable_Get(object_id);
    s32 *camera;
    s32 camera_x;
    s32 camera_z;
    s32 x;
    s32 z;

    if (object == 0)
        return -1;
    /* FAKEMATCH: needed to match: the adjacent coordinate-word read avoids four bytes added by separate named bases. */
    camera = &((struct MapScrollWork *)gMapWork[0])->view_x;
    camera_x = camera[0] & 0xffff0000;
    camera_z = camera[1] & 0xffff0000;
    x = object->x - camera_x;
    z = object->z - camera_z - object->y;
    *position++ = x / 0x10000;
    *position = z / 0x10000;
    if ((object->animation_kind & 15) == 1)
        *position -= (s8)Resource_GetMetadataRecordFar(((struct AnimationObject *)object->animation)->entries[0]->anim_id)->box_x;
    return 0;
}

void ObjectEffect_ReservedNoOp941DC(void)
{
}

#undef WaitFrames

void AudioCommand_PlayFar(s32);
void Battle_WaitMode0(s32 arg0);

s32 Scene_FadeColorFromWhite(void)
{
    struct EventRuntime *work = gEventWork;
    s32 i;
    s32 c;

    AudioCommand_PlayFar(gGameState.scene_cue);
    AudioCommand_PlayFar(288);
    AudioCommand_PlayFar(147);
    if (work->mode_19e == 3) {
        s32 color = 0x7fff;

        do {
            /* FAKEMATCH: needed to match: the one-pass loop keeps the store before the call's arguments. */
            *(u16 *)0x050001e6 = color;
        } while (0);
        DisplayTransition_Finish(0x401, 16);
        work->status_1c6 = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            color = (c << 10) | (c << 5) | c;
            *(u16 *)0x050001e6 = color;
            WaitFrames(1);
        }
    } else {
        s32 color = 0x7fff;

        PLTT_BACKDROP = color;
        DisplayTransition_Finish(0x207, 16);
        work->status_1c6 = 0;
        WaitFrames(16);
        for (i = 0; i < 16; i++) {
            c = 30 - i * 2;
            color = (c << 10) | (c << 5) | c;
            PLTT_BACKDROP = color;
            WaitFrames(1);
        }
    }
}

void ObjectEffect_PrepareContextEffect(s32 value)
{
    u32 zero;
    u8 kind;
    struct ObjectRuntime *object;
    struct AnimationObject *context;
    struct AnimationEntry *effect;

    object = ObjectTable_Get(gGameState.selected_actor);
    context = object->animation;
    effect = (struct AnimationEntry *)ResourceMetadata_RegisterFar(context, 27);
    zero = 0;
    kind = 15;

    context->flags = zero;
    effect->param = kind;
    object->x = (object->x & 0xFFF00000) + 0x80000;
    object->z = (object->z & 0xFFF00000) + 0x100000;
    object->velocity_x = zero;
    object->velocity_z = zero;
    object->target_x = 0x80000000;
    object->target_z = 0x80000000;
    ObjectDispatch_ApplyArgumentToChildrenFar(object, value);
    WaitFrames(18);
}

void ObjectEffect_BeginContextEffect26(void)
{
    ObjectEffect_PrepareContextEffect(0x1A);
    GameFlag_SetBitFar(0x120);
}

void ObjectEffect_BeginContextEffect25(void)
{
    ObjectEffect_PrepareContextEffect(0x19);
    GameFlag_SetBitFar(0x121);
}

void ObjectEffect_EndContextEffect(s32 arg0)
{
    s32 zero;
    s32 mask;
    struct ObjectRuntime *obj = ObjectTable_Get(gGameState.selected_actor);
    struct AnimationObject *ctx = obj->animation;
    struct AnimationEntry *eff = (struct AnimationEntry *)
        ResourceMetadata_RegisterFar(ctx, 27);

    zero = 0;
    mask = 0xfff00000;
    ctx->flags = zero;
    eff->param = 15;
    obj->x = (obj->x & mask) + 0x80000;
    obj->z &= mask;
    ObjectDispatch_ApplyArgumentToChildrenFar(obj, arg0);
    WaitFrames(30);
    ctx->count = 1;
    ResourceMetadata_ClearRecordFar(ctx->entries[1]);
    ctx->entries[1] = (void *)zero;
    ctx->flags = 1;
    obj->acceleration = 0x10000;
    obj->speed_limit = 0x10000;
    Object_SetMoveTargetFar(obj,
        obj->x, obj->y, obj->z + 0x80000);
    Script_WaitForEventTimeoutFar(obj);
}

s32 ObjectEffect_RunPendingFlagEvent(void)
{
    s32 result = 0;
    s32 flag = 0x120;

    if (GameFlag_TestFar(flag) != 0) {
        ObjectEffect_EndContextEffect(24);
        GameFlag_ClearBitFar(flag);
        result = 1;
    } else {
        flag = 0x121;
        if (GameFlag_TestFar(flag) != 0) {
            ObjectEffect_EndContextEffect(23);
            GameFlag_ClearBitFar(flag);
            result = 2;
        } else {
            flag = 0x122;
            if (GameFlag_TestFar(flag) != 0) {
                s32 id;
                struct ObjectRuntime *obj;

                GameFlag_ClearBitFar(flag);
                id = gGameState.selected_actor;
                obj = ObjectTable_Get(id);
                obj->y += 0x00a00000;
                Motion_CamBounds(-1, -1, -1, 0);
                while (obj->y + obj->velocity_y >
                       obj->terrain_height) {
                    WaitFrames(1);
                }
                AudioCommand_PlayFar(159);
                obj->y = obj->terrain_height;
                ObjectDispatch_ApplyArgumentToChildrenFar(obj, 22);
                Battle_WaitMode0(15);
                Object_AttachWorkTargetToObject(id, 1);
                result = 3;
            }
        }
    }
    return result;
}

void DisplayScroll_ArmHBlankDma(void)
{
    volatile u16 *dma;
    struct DisplayScrollWork *state;
    u32 *source;
    u16 status;
    u32 control;
    volatile u32 *destination;

    state = gHBlankScrollWork;
    source = (u32 *)state->rows[state->page];

    dma = REG_DMA0;
    destination = (volatile u32 *)0x04000014;
    status = dma[5];
    control = status & (u16)~DMA_START_HBLANK_REPEAT;
    dma[5] = control;
    status = dma[5];
    control = status & (u16)~DMA_ENABLE;
    dma[5] = control;
    (void)dma[5];

    *destination = *source++;
    *destination = *source++;
    *destination = *source++;

    control = 0xa6600003;
    Dma_Set(source, destination, control, (volatile u32 *)dma);
}

void DisplayScroll_BuildAndSwapHBlankPage(void)
{
    struct DisplayScrollWork *work = gHBlankScrollWork;
    s32 x3 = (s16)gBgScroll[3].x;
    s32 y3 = (s16)gBgScroll[3].y;
    s32 x2 = (s16)gBgScroll[2].x;
    s32 y2 = (s16)gBgScroll[2].y;
    s32 x1 = (s16)gBgScroll[1].x;
    s32 y1 = (s16)gBgScroll[1].y;
    u16 *row;
    s32 step;
    s32 phase;
    s32 amp;
    s32 i;
    u16 d;

    row = &work->rows[work->page ^ 1][0][0].x;
    step = work->step_x;
    phase = (work->frame + (u16)y3) * work->freq_x;
    if (step == 0) {
        for (i = 0; i != 160; i++) {
            row[i * 6] = x3;
            row[i * 6 + 2] = x2;
            row[i * 6 + 4] = x1;
        }
    } else {
        u16 a = x3;
        u16 b = x2;
        u16 c = x1;

        amp = work->amp_x;
        for (i = 0; i != 160; i++) {
            d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
            *row = a + d;
            row += 2;
            *row = b + d;
            row += 2;
            *row = c + d;
            row += 2;
            phase += step;
        }
    }
    row = &work->rows[work->page ^ 1][0][0].y;
    step = work->step_y;
    phase = (work->frame + (u16)y3) * work->freq_y;
    if (step == 0) {
        for (i = 0; i != 160; i++) {
            row[i * 6] = y3;
            row[i * 6 + 2] = y2;
            row[i * 6 + 4] = y1;
        }
    } else {
        u16 a = y3;
        u16 b = y2;
        u16 c = y1;

        amp = work->amp_y;
        for (i = 0; i != 160; i++) {
            d = Iwram_MulQ16(DisplayScroll_WaveSine[(phase >> 16) & 255], amp) / 256;
            *row = a + d;
            row += 2;
            *row = b + d;
            row += 2;
            *row = c + d;
            row += 2;
            phase += step;
        }
    }
    work->frame++;
    work->page ^= 1;
}

void DisplayScroll_DisarmHBlankDma(void)
{
    volatile u16 *dma0;

    Scheduler_RemoveCallback(DisplayScroll_ArmHBlankDma);
    Scheduler_RemoveCallback(DisplayScroll_BuildAndSwapHBlankPage);
    dma0 = REG_DMA0;
    dma0[5] = 0xC5FF & dma0[5];
    dma0[5] = 0x7FFF & dma0[5];
    (void)dma0[5];
}

#define BattleFx_ApplyColorToTargetBuffer ((void (*)(void *, s32))BattleFx_ApplyColorToTargetBuffer)

extern void *gFieldBlendWork[];

struct GroundParticle
{
	u8	unknown_00[4];
	u16 y : 8;
	u16 affine : 2;
	u16 blend_mode : 2;
	u16 mosaic : 1;
	u16 full_color : 1;
	u16 shape : 2;
	u16 x : 9;
	u16 affine_index : 5;
	u16 size : 2;
	u16 tile : 10;
	u16 priority : 2;
	u16 palette : 4;
	u8	unknown_0a[2];
	s32 pos_x;
	s32 pos_y;
	s32 pos_z;
	u8	unknown_18[4];
	u16 timer;
	u8	unknown_1e[2];
};

struct GroundParticleWork
{
	s32					  entry;
	s32					  tile_base;
	struct GroundParticle particles[32];
	s32					  unknown_408[2];
};

void Resource_DecodeByteLz(const void *source, void *destination);
void BattleFx_SetQueuedSoundAndPlay(s32 sound);

void FieldFx_UpdateGroundParticles(void)
{
	struct MapScrollWork	  *view = gMapWork[0];
	struct GroundParticleWork *work = gMapWork[21];
	s32						  *camera = &view->view_x;
	s32						   camera_x = camera[0];
	s32						   camera_z = camera[1];
	struct GroundParticle	  *particle = work->particles;
	u32						   i;

	for (i = 0; i < 32; i++, particle++)
	{
		s16 *frame;
		s32	 x;
		s32	 y;

		if (--particle->timer == 0xffff)
			continue;
		if (GameFlag_TestFar(0x166))
			particle->timer++;
		frame = &FieldFx_GroundParticleFrames[5 * particle->timer];
		x = (particle->pos_x - camera_x) / 0x10000 + *frame++;
		y = (particle->pos_z - particle->pos_y - camera_z) / 0x10000 + *frame++;
		if ((x >= -16 && x <= 239) && y >= -32 && y <= 159)
		{
			particle->priority = 1;
			particle->x = x;
			particle->y = y;
			particle->tile = work->tile_base + *(u16 *)frame;
			frame++;
			particle->shape = *(u8 *)frame;
			particle->size = *(u8 *)(frame + 1);
			Runtime_PushSlotEntry(particle, 240);
		}
		if (particle->timer == 0)
		{
			s32 *leader = view->origin;

			x = leader[0] + (Random16() << 8) - 0x800000;
			y = leader[2] + (Random16() << 8) - 0x800000;
			particle->pos_x = x;
			particle->pos_z = y;
			particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
			particle->timer = 16;
		}
	}
}

void BattleFx_UpdateStormFlash(void)
{
	struct FieldBlendWork	   *work = gFieldBlendWork[0];
	struct BattleEffectBuffers *target = gFieldBlendWork[2];
	s16						   *timer = &work->timer;

	if (*timer < 0)
		return;
	if (GameFlag_TestFar(358))
		work->timer = 128;
	switch ((*timer)--)
	{
	case 0:
		if (work->enabled != 0)
		{
			u32 a = Random16();
			u32 b = Random16();
			work->timer = ((a * 400) >> 16) - ((b * 100) >> 16) + 150;
			if (work->loud != 0)
				AudioCommand_PlayFar(172);
			else
				AudioCommand_PlayFar(171);
		}
	case 5:
	case 10:
		BattleFx_ApplyColorToTargetBuffer(work, 1);
		Dma_Set(work->delta, target->delta, 0x840002a0, REG_DMA3);
		target->duration = 12;
		target->step = 0;
		break;
	case 1:
	case 6:
	case 11:
		BattleFx_ApplyColorToTargetBuffer(work->to, 1);
		target->duration = 1;
		target->step = 0;
		break;
	}
}

void FieldFx_StartGroundParticles(void)
{
	struct GroundParticleWork *work;
	s32						  *leader;
	struct GroundParticle	  *particle;
	u32						  *words;
	volatile u32			   fill;
	volatile u16			  *blend;
	u32						   i;
	s32						   value;

	/* FAKEMATCH: needed to match: reusing the cursor preserves its register */
	work = Runtime_AllocateBlock(29, sizeof(struct GroundParticleWork));
	leader = ((struct MapScrollWork *)gMapWork[0])->origin;
	BattleFx_SetQueuedSoundAndPlay(170);
	words = 0;
	particle = work->particles;
	fill = (u32)words;
	Dma_Set(
		(const void *)&fill, work, 0x85000000 | (sizeof(struct GroundParticleWork) / 4), REG_DMA3);
	words = Runtime_AllocateBlock(14, 0x400);
	Resource_DecodeByteLz(FieldFx_GroundParticleTiles, words);
	work->entry = Resource_FindFreeEntry();
	work->tile_base = VramBlock_LoadCached(work->entry, 0x300, words);
	Runtime_ReleaseHeapBlock(14);
	for (i = 0; i < 32; i++)
	{
		s32 x;
		s32 z;

		words = (u32 *)particle;
		*words++ = 0;
		*words++ = 0x40000400;
		*words = 0xd400;
		x = leader[0];
		z = leader[2];
		particle->pos_x = x;
		particle->pos_z = z;
		particle->pos_y = Map_GetTerrainHeightFar(0, x >> 16, z >> 16) << 16;
		particle->timer = (i & 15) + 1;
		particle++;
	}
	do
	{
		blend = &REG_BLDCNT;
		value = 0x3f00;
		*blend = value;
		value = 0x1008;
		*++blend = value;
		*++blend = 0;
	} while (0);
	Scheduler_AddOrUpdateCallback(FieldFx_UpdateGroundParticles, 0xc80);
}

void Resource_DecodeByteLz(const void *src, void *dst);

struct Mote {
    s32 state;
    s32 attr;
    s32 scale;
    s32 x;
    s32 y;
    s32 z;
    s32 unknown_18;
    u16 phase;
    u16 pad;
};

struct MoteWork {
    s32 resource;
    s32 vram;
    struct Mote motes[32];
    u8 unknown_408[8];
};

struct Sparkle {
    u8 padding00[4];
    u16 y : 8;
    u16 affine : 2;
    u16 blend_mode : 2;
    u16 mosaic : 1;
    u16 full_color : 1;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
    u16 tile : 10;
    u16 priority : 2;
    u16 palette : 4;
    u8 padding0a[2];
    s32 pos_x;
    s32 height;
    s32 pos_z;
    s32 delay;
    u16 timer;
    u8 padding1e[2];
};

struct SparkleWork {
    u8 padding00[4];
    s32 tile_base;
    struct Sparkle sparkles[32];
    u8 padding408[4];
    s32 stopped;
};

struct SparkleFrame {
    s16 dy;
    u16 tile;
};

extern const struct SparkleFrame FieldFx_SparkleFrames[];

union DustWord {
    u32 value;
    void *link;
};

struct DustParticle {
    union DustWord link;
    u32 attr01;
    u32 attr2;
    s32 pos_x;
    s32 pos_y;
    s32 pos_z;
    u8 pad18[4];
    u16 timer;
    u8 pad1e[2];
};

struct DustWork {
    s32 vram_entry;
    s32 tile_base;
    struct DustParticle particles[32];
    u8 pad408[8];
};

void FieldMotes_Update(void)
{
    /* FAKEMATCH: needed to match: an unsigned-long frame read keeps size and flip stores together. */
    struct SparkleWork *work = WORK_SLOT(29);
    struct MapScrollWork *map = WORK_SLOT(8);
    u32 spawned = 0;
    u32 i;
    struct Sparkle *mote = work->sparkles;

    for (i = 0; i < 32; i++, mote++) {
        s32 x;
        s32 y;

        if (--mote->timer != 0xffff) {
            s32 *camera = &map->view_x;
            s32 camera_x = camera[0];
            s32 camera_z = camera[1];
            u16 age = mote->timer;
            u32 a;
            u32 b;

            if (GameFlag_TestFar(0x166)) {
                mote->timer++;
                mote->delay--;
            }
            a = Random16();
            b = Random16();
            x = ((mote->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1) - 1;
            y = ((mote->pos_z - mote->height - camera_z) >> 16) - age;
            if ((u32)(x + 16) <= 255 && y >= -32 && y <= 159) {
                if (mote->timer < 60) {
                    mote->tile = work->tile_base + 16;
                    mote->delay += 3;
                } else if (mote->timer < 90) {
                    mote->tile = work->tile_base + 8;
                    mote->delay++;
                } else {
                    mote->tile = work->tile_base;
                }
                if ((*(unsigned long *)&gFrameCount >> 3) & 1)
                    mote->tile += 4;
                mote->x = x;
                mote->y = y - (mote->delay >> 2);
                mote->shape = 0;
                mote->size = 1;
                Runtime_PushSlotEntry(mote, 240);
            } else {
                mote->timer = 0;
            }
        }
        if (spawned < 8 && mote->timer == 0) {
            s32 *leader = map->origin;

            x = leader[0] + (Random16() << 8) - 0x800000;
            y = leader[2] + (Random16() << 8) - 0x800000;
            mote->pos_z = y;
            mote->pos_x = x;
            mote->height = Map_GetTerrainHeightFar(0, x >> 16, y >> 16) << 16;
            mote->timer = 120;
            mote->delay = 0;
            spawned++;
        }
    }
}

void FieldMotes_Start(void)
{
    struct MoteWork *work;
    struct Mote *mote;
    u8 *tiles;
    u32 i;
    u32 size;
    volatile u32 zero;
    volatile u16 *reg;

    size = 0x410;
    work = Runtime_AllocateBlock(29, size);
    mote = work->motes;
    /* FAKEMATCH: needed to match: reusing the allocation size for the cleared word keeps it in r1. */
    size = 0;
    zero = size;
    Dma_Set((void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
    tiles = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_MoteTiles, tiles);
    work->resource = Resource_FindFreeEntry();
    work->vram = VramBlock_LoadCached(work->resource, 0x300, tiles);
    Runtime_ReleaseHeapBlock(14);
    for (i = 0; i < 32; i++) {
        s32 *pos = ((struct MapScrollWork *)gMapWork[0])->origin;
        /* FAKEMATCH: needed to match: reusing the integer field cursor for the height x argument keeps the stores in r1. */
        u32 p = (u32)mote;
        s32 z;
        *(s32 *)p = 0;
        p += sizeof(s32);
        *(s32 *)p = 0x40000400;
        p += sizeof(s32);
        *(s32 *)p = 0xd400;
        p = pos[0];
        z = pos[2];
        mote->x = (s32)p;
        mote->z = z;
        mote->y = Map_GetTerrainHeightFar(0, (s32)p >> 16, z >> 16) << 16;
        mote->phase = (i & 15) + 1;
        mote++;
    }
    do { s32 v; v = 0x3f00; reg = &REG_BLDCNT; *reg = v; v = 0x1008; reg++; *reg = v; reg++; *reg = 0; } while (0);
    Scheduler_AddOrUpdateCallback((s32)(FieldMotes_Update), 0xc80);
}

void FieldEffect_UpdateSparkles(void)
{
    /* FAKEMATCH: needed to match: an unsigned-long frame read keeps size and flip stores together. */
    struct SparkleWork *work;
    u32 spawned;
    u32 i;
    struct MapScrollWork *map;
    s32 *camera;
    s32 spawn_x;
    s32 spawn_z;
    struct Sparkle *sparkle;
    const u16 *frame;
    s32 *leader;
    s32 camera_x;
    s32 camera_z;
    s32 sx;
    s32 sy;
    s32 delay;
    s32 x;
    u32 a;
    u32 b;
    u32 frames;

    work = WORK_SLOT(29);
    spawned = 0;
    map = WORK_SLOT(8);
    camera = &map->view_x;
    spawn_x = 0;
    spawn_z = 0;
    i = 0;
    sparkle = work->sparkles;
    delay = 0;
    sx = 0;
    sy = 0;
    do {
        if (sparkle->timer != 0) {
            camera_x = camera[0];
            camera_z = camera[1];
            if (GameFlag_TestFar(0x166) != 0)
                sparkle->timer++;
            frame = (const u16 *)&FieldFx_SparkleFrames[sparkle->timer >> 1];
            a = Random16();
            b = Random16();
            x = ((sparkle->pos_x - camera_x) >> 16) + (((a & 1) + (b & 1)) >> 1);
            sx = x - 1;
            sy = (sparkle->pos_z - sparkle->height - camera_z) / 0x10000 + *(const s16 *)frame;
            frame++;
            if ((u32)(x + 15) <= 255 && sy >= -32 && sy <= 159) {
                sparkle->tile = work->tile_base + *frame;
                sparkle->x = sx;
                sparkle->shape = 0;
                sparkle->size = 1;
                frames = *(unsigned long *)&gFrameCount;
                sparkle->y = sy;
                sparkle->matrix = ((frames >> 1) & 1) << 3;
                Runtime_PushSlotEntry(sparkle, 240);
            }
            sparkle->timer--;
        }
        if (spawned <= 3 && sparkle->timer == 0 && work->stopped == 0) {
            if (delay != 0) {
                sparkle->pos_x = spawn_x;
                sparkle->pos_z = spawn_z;
                sparkle->height = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
                sparkle->timer = 62 - delay;
                sparkle->delay = 0;
                spawned++;
                delay += 4;
            } else if ((Random16() & 255) == 0) {
                leader = map->origin;
                spawn_x = leader[0] + (Random16() << 8) - 0x800000;
                spawn_z = leader[2] + (Random16() << 8) - 0x800000;
                sparkle->pos_x = spawn_x;
                sparkle->pos_z = spawn_z;
                sparkle->height = Map_GetTerrainHeightFar(0, sx >> 16, sy >> 16) << 16;
                sparkle->timer = 30;
                sparkle->delay = delay;
                spawned++;
                delay = 4;
            }
        }
        i++;
        sparkle++;
    } while (i <= 31);
}

void FieldEffect_InitSparkles(void)
{
    u32 size = 0x410;
    struct DustWork *work = Runtime_AllocateBlock(29, size);
    struct DustParticle *p = work->particles;
    u8 *buf;
    u32 i;
    s32 clear;
    volatile u32 zero;

    /* FAKEMATCH: needed to match: reusing the allocation size for the fill word keeps zero in r1; a separate helper value differs by four instructions. */
    size = 0;
    zero = size;
    Dma_Set(&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x104, REG_DMA3);
    buf = Runtime_AllocateBlock(14, 0x400);
    Resource_DecodeByteLz(FieldFx_SparkleTiles, buf);
    work->vram_entry = Resource_FindFreeEntry();
    work->tile_base = VramBlock_LoadCached(work->vram_entry, 0x200, buf);
    Runtime_ReleaseHeapBlock(14);
    i = 0;
    clear = 0;
loop:
    {
        struct MapScrollWork *view = gMapWork[0];
        s32 *leader;
        /* FAKEMATCH: needed to match: an integer field cursor keeps the view load order; reusing it for the height x argument keeps the leader in r2. Separate temporaries differ by 14 instructions. */
        u32 attr;
        s32 z;

        attr = (u32)p;
        *(u32 *)attr = clear;
        attr += sizeof(union DustWord);
        leader = view->origin;
        *(u32 *)attr = 0x40000400;
        attr += sizeof(union DustWord);
        *(u32 *)attr = 0xd400;
        attr = leader[0];
        z = leader[2];
        p->pos_x = clear;
        p->pos_z = clear;
        p->pos_y = Map_GetTerrainHeightFar(0, (s32)attr >> 16, z >> 16) << 16;
        p->timer = (i & 15) + 1;
    }
    i++;
    p++;
    if (i < 32)
        goto loop;
    Scheduler_AddOrUpdateCallback((s32)(FieldEffect_UpdateSparkles), 0xc80);
}

void BattleFx_StartTwelveFrameBlend(void)
{
    struct FieldBlendWork *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;

    work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    buffers = gPaletteBufferWork;
    zero = 0;
    Dma_Set((const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
    BattleFx_BuildBuffer(0x10003, buffers, work, 1);
    BattleFx_BuildBuffer(0x10005, buffers, work->to, 1);
    BattleFx_InterpolateBuffers(work->to, work->from, work->delta, 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
    work->timer = 600;
    work->enabled = 1;
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}

void BattleFx_SetBlock30ValuesMaxZero(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    state->timer = 0x7FFF;
    state->enabled = 0;
}

void BattleFx_SetBlock30Values12Zero(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    state->timer = 12;
    state->enabled = 0;
}

void BattleFx_SetBlock30Values128One(void)
{
    struct FieldBlendWork *state = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    state->timer = 128;
    state->enabled = 1;
}

void BattleFx_StartBufferBlend(s32 from, s32 to)
{
    struct FieldBlendWork *work;
    struct BattleEffectBuffers *buffers;
    volatile u32 zero;
    s16 *target;

    work = Runtime_AllocateBlock(30, sizeof(struct FieldBlendWork));
    buffers = gPaletteBufferWork;
    zero = 0;
    Dma_Set((const void *)&zero, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | 0x7e2, REG_DMA3);
    BattleFx_BuildBuffer(from, buffers, work, 1);
    target = work->to;
    BattleFx_BuildBuffer(to, buffers, target, 1);
    BattleFx_InterpolateBuffers(target, work->from, work->delta, 12);
    BattleFx_BuildBuffer((s32)work, 0, buffers->target, 1);
    work->timer = 120;
    work->enabled = 0;
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateStormFlash), 0xc80);
}
