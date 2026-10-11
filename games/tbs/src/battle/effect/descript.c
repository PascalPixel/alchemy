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
#include "types.h"
#include "game_state.h"
#include "field_scene.h"
#include "map.h"
#include "map_scroll.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "scene.h"
#include "global_cells.h"
#include "object_runtime.h"
#include "script_object_runtime.h"
#include "animspr.h"
#include "fieldrun.h"
#include "fieldobj.h"
#include "object_dispatch.h"
#include "field_event.h"
#include "btlsys.h"
#include "event_runtime.h"
#include "dma.h"
#include "field_sprite.h"
#include "io_reg.h"
#include "battle_effect_counters.h"
#include "heap_state.h"
#include "keystate.h"
#include "mem.h"
#include "ownerval.h"
#include "btltype.h"
#include "party_state.h"
#include "sound_ids.h"
#include "fixed_math.h"
#include "itemgift.h"
#include "resource_ids.h"
#include "scene_ids.h"
#include "btlcalc.h"
#include "callback_scheduler.h"
#include "disptran.h"
#include "text/msg_ids.h"
#include "object_lookup.h"
#include "objdisp.h"

void BattleParty_ApplyDrain(s32 amount);

void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);
void ObjectDispatch_ReleaseFar(struct DispatchObject *work);

#define ObjectDispatch_ReleaseFar ((void (*)(void*))ObjectDispatch_ReleaseFar)

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

extern const struct ScenePlacement Field_DefaultPlacements[2];
#define Object_CreateFar ((struct ObjectRuntime *(*)(s32 character, s32 x, s32 y, s32 z))Object_CreateFar)
u32 Random16(void);
u32 __umodsi3(u32 numerator, u32 denominator);

void Event_SpawnObjectTable(struct ScenePlacement *entry, s32 slot)
{
    struct FieldStepWork *work;
    struct ObjectRuntime *object;
    struct ObjectRuntime *previous;
    struct AnimationObject *sprite;
    s32 i;
    s32 index;
    s32 character;
    s32 condition;
    u8 resource;
    u16 id;

    work = (struct FieldStepWork *)gEventWork;
    for (i = 0; i < 4; i++) {
        if (work->tables[i] == entry)
            break;
        if (work->tables[i] == 0) {
            work->tables[i] = entry;
            break;
        }
    }
    for (id = entry->sprite; (s16)id != -1 && slot <= 65; entry++, id = entry->sprite) {
        /* FAKEMATCH: needed to match: the second row-id read prevents sharing the shifted id between the test and body. */
        id = entry->sprite;
        if ((s16)id <= 7)
            index = (s16)id;
        else if ((s16)id <= 0x2705)
            index = slot++;
        condition = entry->condition;
        if (!GameFlag_IsConditionActive(condition))
            continue;
        if ((u32)(condition - 48) <= 79 && work->mode != 3
            && !GameFlag_IsConditionActive(condition + 80))
            continue;
        character = Party_RemapCharacterIdByFlags(entry->sprite);
        object = ObjectTable_Get(index);
        if (object == 0) {
            object = Object_CreateFar(character, entry->x, entry->y, entry->z);
            if (entry->flags & 1) {
                previous = ObjectTable_Get(index - 1);
                if (previous->animation_kind == 1 && object->animation_kind == 1) {
                    sprite = (struct AnimationObject *)previous->animation;
                    sprite->display_flags |= 1;
                    resource = sprite->slot;
                    sprite = (struct AnimationObject *)object->animation;
                    sprite->display_flags |= 1;
                    Resource_ResetEntry(sprite->slot);
                    sprite->slot = resource;
                }
            }
            if (GameFlag_TestFar(33) && (u32)(character - 18) <= 1)
                ObjectDispatch_RegisterChildMetadataFar((struct DispatchObject *)object, 226);
        } else if (!GameFlag_TestFar(0x109)) {
            Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
        }
        if (object) {
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
            if (object->animation_kind == 1 && (sprite = (struct AnimationObject *)object->animation) != 0)
                sprite->last_no = __umodsi3(Random16(), 30);
            object->angle = entry->facing;
            object->unknown_59 = 1;
            ObjectMotion_SetActionCallback(object, entry->behavior);
            ObjectDispatch_ApplyArgumentToChildrenFar(object, 1);
            ((struct ScriptObjectRuntime *)object)->home_x = object->x / 0x10000;
            ((struct ScriptObjectRuntime *)object)->home_z = object->z / 0x10000;
            if (object->y != 0) {
                object->flags = 4;
                object->y += 0x8000;
            }
            if (work->mode == 3) {
                object->flags &= 0xfe;
                if (!GameFlag_TestFar(33))
                    sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
            } else {
                object->terrain_height = Map_GetTerrainHeightFar(0, object->x, object->z);
                object->y += object->terrain_height;
            }
            object->unknown_23 = 1;
        }
        work->actors[index] = object;
    }
}

void ObjectTable_DestroyAtIndex(s32 index)
{
    struct FieldStepWork *state = gEventWork;
    void *object = ObjectTable_Get(index);

    if (object != NULL) {
        ObjectDispatch_ReleaseFar(object);
        state->actors[index] = NULL;
    }
}

void ObjectTable_ResetForObject(struct ScenePlacement *table)
{
	void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *object, s32 value);
    struct FieldStepWork *work;
    struct ScenePlacement *entry;
    struct ObjectRuntime *object;
    struct ObjectRuntime *camera;
    struct MapCell *cell;
    struct MapCell *above;
    s32 leader;
    s32 pos;
    s32 hgt;

    work = (struct FieldStepWork *)gEventWork;
    leader = gGameState.selected_actor;
    entry = (struct ScenePlacement *)((u8 *)work + 0x200);
    entry[0] = Field_DefaultPlacements[0];
    entry[1] = Field_DefaultPlacements[1];
    for (pos = 0; pos < 4; pos++)
        work->tables[pos] = 0;
    ObjectTable_ClearBattleSlots();
    entry->condition = -1;
    entry->sprite = leader;
    entry->x = gGameState.x;
    entry->y = 0;
    entry->z = gGameState.z;
    entry->facing = gGameState.heading;
    Event_SpawnObjectTable(entry, leader);
    Event_SpawnObjectTable(table, 8);

    object = work->actors[leader];
    object->terrain_id = gGameState.turn;
    pos = (object->x / 0x100000) + (object->z / 0x100000) * 128;
    cell = &MAP_CELLS[pos];
    above = &MAP_CELLS[pos - 128];
    if (gGameState.y != 0 && cell->collision_code == 0xfd && above->collision_code == 0xfd) {
        gGameState.movement_mode = 1;
        hgt = Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) - 0x200000;
        object->y += hgt;
        object->terrain_height = object->y;
        object->flags = 0;
        ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, 0);
        ObjectDispatch_ApplyArgumentToChildrenFar(object, 12);
    } else {
        gGameState.movement_mode = 0;
    }

    camera = Object_CreateFar(0x8000, object->x, object->y, object->z);
    camera->terrain_height = object->terrain_height;
    ObjectDispatch_InitFromTable4WithArgumentFar((struct DispatchObject *)camera, (s32)object);
    if (work->mode == 3) {
        struct AnimationEntry *meta = (struct AnimationEntry *)
            ResourceMetadata_RegisterFar(object->animation, 23);
        meta->param = 15;
        meta->priority = 9;
    }
    ((struct MapScrollWork *)gMapWork[0])->origin = &camera->x;
    ((struct EventWork *)work)->view_center = (struct FieldActor *)camera;
}

s32 ObjectTable_FindLastActiveId(void)
{
    struct FieldStepWork *state = (struct FieldStepWork *)gEventWork;
    s32 result = 7;
    s32 index = 8;
    struct ObjectRuntime **value = &state->actors[ACTOR_FIRST_PLACED];

    do {
        struct ObjectRuntime *current = *value++;

        if (current != 0) {
            result = index;
        }
        index++;
    } while (index <= 65);
    result++;
    if (result == 66) {
        result = -1;
    }
    return result;
}

void *ObjectTable_GetSlotAddress(u32 index)
{
    return (u8 *)gEventWork + 0x14 + index * sizeof(void *);
}

#undef Object_CreateFar

//--------------------------------------------------------------------------

// bounds.c

// フィールド：画面の範囲による条件フラグと、置いた物体の片付け

//--------------------------------------------------------------------------

#undef ObjectDispatch_ReleaseFar

#define FLG_IN 0x164  // 範囲の中 (下ろしたフラグ)
#define FLG_OUT 0x165 // 範囲の外 (立てたフラグ)

#define PLACED_MAX 58 // 置いた物体の数

// 範囲で条件を決める表の一行
struct SceneRegionEntry
{
	s16 id; // -1 で終わり
	s16 flag;
	u8	unknown_04[4];
	s32 x;
	u8	unknown_0c[4];
	s32 z;
	u8	unknown_14[4];
};
#define ObjectDispatch_ReleaseFar ((void (*)(struct FieldActor *object))ObjectDispatch_ReleaseFar)

//--------------------------------------------------------------------------
// フラグの決まっていない行に、マップの範囲の中か外かでフラグを割り当てる
//--------------------------------------------------------------------------
void Scene_AssignViewFlags(struct SceneRegionEntry *entry)
{
	struct MapScrollWork *view = gMapWork[0];

	GameFlag_ClearBitFar(FLG_IN);
	GameFlag_SetBitFar(FLG_OUT);
	while (entry->id != -1)
	{
		/* FAKEMATCH: needed to match: the flag read through a u16 keeps loop.c to one hoisted bound address */
		u16 raw = entry->flag;
		s16 flag = raw;

		if (flag == 0)
		{
			s32 x = entry->x;
			s32 z = entry->z;

			if (view->min_x <= x && x <= view->max_x && view->min_y <= z && z <= view->max_y)
				entry->flag = FLG_IN;
			else
				entry->flag = FLG_OUT;
		}
		entry++;
	}
}

//--------------------------------------------------------------------------
// 画面の中心から離れた物体を消す
// x は ±160、z は -200 から +100 ドットの外。(0, 0) にある物体は残す
//--------------------------------------------------------------------------
void BattleEffect_ClearOutOfBoundsObjects(void)
{
	struct EventWork   *work = gEventWork;
	struct FieldActor  *center;
	s32					x;
	s32					z;
	s32					left;
	s32					right;
	s32					top;
	s32					bottom;
	struct FieldActor **slot;
	s32					mask;
	s32					i;

	center = work->view_center;
	x = center->x.fixed;
	left = x - 0xa00000;
	right = x + 0xa00000;
	z = center->z.fixed;
	top = z - 0xc80000;
	bottom = z + 0x640000;
	slot = work->placed_actors;
	mask = ~1;

	for (i = PLACED_MAX - 1; i >= 0; i--)
	{
		struct FieldActor *obj = *slot;

		if (obj != NULL)
		{
			s32 obj_x = obj->x.fixed;
			s32 obj_z = obj->z.fixed;

			if (obj_x != 0 || obj_z != 0)
			{
				if (obj_x < left || obj_x > right || obj_z < top || obj_z > bottom)
				{
					u8 *spr;
					u8 *removed = &obj->active;
					u32 flags;

					// 表示を止めて消す
					*removed = 1;
					spr = (u8 *)obj->sprite;
					flags = spr[29];
					spr[29] = flags & mask;
					ObjectDispatch_ReleaseFar(obj);
					spr = NULL;
					*slot = (struct FieldActor *)spr;
				}
			}
		}

		slot++;
	}
}

//--------------------------------------------------------------------------
// 置いた物体をすべて消し、場面の配置表から置き直す
//--------------------------------------------------------------------------
void BattleEffect_ClearAllObjects(void)
{
	struct EventWork	  *work = gEventWork;
	struct ScenePlacement *table;
	s32					   i;

	for (i = 0; i < PLACED_MAX; i++)
	{
		struct FieldActor *obj = work->placed_actors[i];

		if (obj != 0)
		{
			obj->active = 1;
			obj->sprite->unknown_1d_0 = 0;
			ObjectDispatch_ReleaseFar(obj);
			work->placed_actors[i] = 0;
		}
	}
	table = work->tables[1];
	work->tables[1] = 0;
	work->tables[2] = 0;
	work->tables[3] = 0;
	if (table != 0)
		Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}

#undef ObjectDispatch_ReleaseFar

extern struct ObjectRuntime gObjectSnapshots[32];

void ObjectTable_ClearBattleSlots(void)
{
    s32 *current;
    s32 zero;
    s32 count;
    zero = 0;
    count = 0x41;
    current = (s32 *)&((struct FieldStepWork *)gEventWork)->actors[65];
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}

void *ObjectTable_Get(u32 index)
{
    u8 *base = (u8 *)gEventWork;
    u32 offset;
    if (index > 0xbf)
        return 0;
    offset = (index * 4) + 0x14;
    return *(void **)(base + offset);
}

void ObjectTable_Snapshot(void)
{
    struct ObjectRuntime *copy = gObjectSnapshots;
    u8 *frames = (u8 *)&gObjectSnapshots[32];
    u8 *next_frames = frames + 32;
    u8 *priorities = frames + 64;
    u8 *indices = (u8 *)gObjectSnapshots - 32;
    u32 count = 0;
    s32 limit = 66;
    struct ObjectRuntime *object;
    s32 i;
    u32 frame;
    u32 next;
    u32 priority;

    if (((struct EventRuntime *)gEventWork)->mode_19e == 3)
        limit = 8;
    for (i = 0; i < limit; i++) {
        object = (struct ObjectRuntime *)ObjectTable_Get(i);
        if (object == NULL)
            continue;
        *indices++ = i;
        Dma_Set(object, copy, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
        if (object->animation_kind == 1) {
            frame = ((struct AnimationObject *)object->animation)->last_no;
            next = ((struct FieldSprite *)object->animation)->flags;
            priority = ((struct FieldSprite *)object->animation)->priority;
        } else {
            frame = 0;
            next = 0;
            priority = 0;
        }
        *frames++ = frame;
        *next_frames++ = next;
        *priorities++ = priority;
        count++;
        copy++;
        if (count > 31)
            break;
    }
    for (i = count; i < 32; i++)
        *indices++ = 255;
}

void ObjectTable_Restore(void)
{
	void ObjectDispatch_InitFromTable6Far(void *object);
	void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *object, s32 value);
    struct ObjectRuntime *copy = gObjectSnapshots;
    u8 *indices = (u8 *)gObjectSnapshots - 32;
    u8 *frames = (u8 *)&gObjectSnapshots[32];
    u8 *next_frames = frames + 32;
    u8 *priorities = frames + 64;
    s32 count;
    struct ObjectRuntime *object;
    struct FieldSprite *sprite;
    struct ObjectRuntime *camera;
    s32 *view;
    s32 index;
    s32 frame;
    s32 priority;
    s32 y;
    s32 *selected = &gGameState.selected_actor;
    struct EventWork **work = &gEventWork;
    struct MapScrollWork **map = (struct MapScrollWork **)gMapWork;

    count = 0;
    index = *indices++;
    while (index != 255) {
        object = ObjectTable_Get(index);
        if (object != NULL) {
            sprite = object->animation;
            Dma_Set(copy, object, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
            frame = *frames;
            if (frame != 0)
                ObjectDispatch_ApplyArgumentToChildrenFar(object, frame);
            ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, *next_frames);
            priority = *priorities;
            sprite->priority = priority;
            sprite->second_priority = priority;
            object->animation = sprite;
            if (index == *selected) {
                camera = (struct ObjectRuntime *)(*work)->view_center;
                view = (*map)->origin;
                y = object->y;
                camera->terrain_height = y;
                camera->y = y;
                view[1] = y;
                ObjectDispatch_InitFromTable6Far(object);
            }
        }
        copy++;
        frames++;
        next_frames++;
        priorities++;
        count++;
        if (count > 31)
            break;
        index = *indices++;
    }
}

#define BattleFx_ApplyLookupResult ((s32 (*)(void*,s32))BattleFx_ApplyLookupResult)
#define gEventWork (*(struct EventRuntime *(*))&gEventWork)

void AudioCommand_PlayFar(s32 sound_id);
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

extern u8 gMapCellBuffer[];

struct StepOwner {
    u8 unknown_000[0x38];
    s16 hp;
    u8 unknown_03a[0x131 - 0x3a];
    u8 doku;
};

enum StepTileEvent {
    STEP_EVENT_FIRST = 1,
    STEP_EVENT_LAST = 239,
    STEP_TERRAIN_FIRST = 240,
    STEP_TERRAIN_LAST = 241,
    STEP_EVENT_HAZARD = 250,
    STEP_WARP_FIRST = 252,
    STEP_WARP_LAST = 254
};

#define STEP_MODE_WORLD_MAP 3
#define STEP_NO_TARGET ((s32)0x80000000)
#define STEP_FLAG_DOUBLE_SPEED 0x167
#define STEP_CUE_EFFECT 139
#define BattleParty_ApplyDrain ((void (*)(s32 steps, s32 unused))BattleParty_ApplyDrain)
void AudioCommand_PlayFar(s32 cue);

void BattleFx_ResetCounters(void)
{
  short cleared_value;
  struct BattleEffectCounterState *state;
  cleared_value = 0;
  state = (struct BattleEffectCounterState *)gEventWork;
  state->counters[0] = cleared_value;
  state->counters[1] = 0;
  state->counters[2] = 0;
  state->counters[3] = 0;
  state->counters[4] = 0;
  state->counters[5] = 0;
  state->counters[6] = 0;
  state->counters[7] = 0;
  state->counters[8] = 0;
  state->counters[9] = 0;
  state->counters[10] = 0;
  state->counters[11] = 0;
}

s32 BattleFx_SumCounters(void)
{
    struct BattleEffectCounterState *state = (struct BattleEffectCounterState *)gEventWork;

    return state->counters[0] + state->counters[1] + state->counters[2]
         + state->counters[3] + state->counters[4] + state->counters[5]
         + state->counters[6] + state->counters[7] + state->counters[6]
         + state->counters[8] + state->counters[9] + state->counters[10]
         + state->counters[11];
}

s32 GetFocusedObjectCollision(void)
{
	void Vector_AddPolarOffset(s32, s32, void *);
    struct EventRuntime *runtime;
    struct ObjectRuntime *object;
    struct MapState *map;
    struct FieldPosition position;
    u8 *tile;
    s32 x;
    s32 y;

    runtime = WORK_SLOT(HEAP_SLOT_EVENT);
    object = ((struct FieldStepWork *)runtime)->actors[gGameState.selected_actor];

    map = WORK_SLOT(8);

    if (object == 0)
        return 0;

    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x100000, object->angle, &position);

    if (runtime->mode_19e == 3) {
        u32 tile_x;
        u32 tile_y;

        x = position.x;
        if (x < 0)
            x += 0x1fffff;
        tile_x = (x >> 21) & 31;

        y = position.z;
        if (y < 0)
            y += 0x1fffff;
        tile_y = (y >> 21) & 31;

        tile = (u8 *)gMapBlocks + ((tile_x + (tile_y << 5)) << 2);
    } else {
        tile = (u8 *)map->layers[0].cells;

        x = position.x;
        if (x < 0)
            x += 0xfffff;
        {
            u32 tile_x = x >> 20;

            y = position.z;
            if (y < 0)
                y += 0xfffff;

            tile = (u8 *)((u32)tile +
                ((tile_x + ((u32)(y >> 20) << 7)) << 2));
        }
    }

    return tile[2];
}

void UpdateMapRegionAtPosition(s32 position_x, s32 position_y, s32 position_z)
{
    s32 selected_value;
    struct EventRuntime *runtime;
    s32 z;
    s32 y;
    s32 x;
    s16 condition;
    s16 min_z;
    s16 max_y;
    s16 min_y;
    s32 max_z;
    s16 max_x;
    s16 min_x;
    const struct SceneRegion *region;

    x = position_x;
    y = position_y;
    z = position_z;
    region = gOverlayArea.regions();
    if (region != 0 &&
        (runtime = gEventWork, min_x = region->min_x, min_x != -1)) {
loop:
        min_y = region->min_y;
        min_z = region->min_z;
        max_x = region->max_x;
        max_y = region->max_y;
        max_z = region->max_z;
        condition = region->condition;
        selected_value = region->value;
        if (GameFlag_IsConditionActive(condition)!= 0 &&
            y >= REGION_FIXED(min_y)&&
            y < REGION_FIXED(max_y)&&
            x >= REGION_FIXED(min_x)&&
            x < REGION_FIXED(max_x)&&
            z >= REGION_FIXED(min_z)&&
            z < REGION_FIXED(max_z)) {
            runtime->value_170 = (u16)selected_value;
            AudioCommand_PlayFar(123);
            Battle_InitializeRenderObject();
            return;
        }
        runtime = gEventWork;
        region++;
        min_x = region->min_x;
        if (min_x != -1)
            goto loop;
    }
}

void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: needed to match: the StepOwner read prefix preserves the party-load order and the 44-byte frame. */
    struct FieldStepWork *work = WORK_SLOT(HEAP_SLOT_EVENT);
    struct MapState *map = WORK_SLOT(8);
    s32 selected = gGameState.selected_actor;
    struct ObjectRuntime *actor = work->actors[selected];
    s16 hp[8];
    struct MapCell *tile;
    struct StepOwner *owner;
    u32 count;
    s32 fell;
    u32 i;
    u32 event;
    s32 speed;
    s32 steps;
    s32 n;
    s32 sum;
    s32 full;

    fell = 0;
    if (work == NULL)
        return;

    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++)
        hp[i] = UnitGetFar(gGameState.active_owners[i])->hp;

    if (work->mode == STEP_MODE_WORLD_MAP) {
        tile = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
    } else {
        if ((u32)layer <= 2)
            tile = map->layers[layer].cells;
        else
            tile = (struct MapCell *)gMapCellBuffer;
        tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
    }
    event = tile->collision_code;
    work->previous_tile = work->tile;
    work->tile = tile;
    if (event != 0)
        UpdateMapRegionAtPosition(x, y, z);
    if (event >= STEP_EVENT_FIRST && event <= STEP_EVENT_LAST)
        work->step_event = event;
    if (event >= STEP_WARP_FIRST && event <= STEP_WARP_LAST)
        work->warp_event = event;

    if (gGameState.movement_mode == 0 && actor != NULL && actor->target_x != STEP_NO_TARGET) {
        speed = actor->speed_limit;
        if (GameFlag_TestFar(STEP_FLAG_DOUBLE_SPEED))
            speed <<= 1;
        if (work->mode == STEP_MODE_WORLD_MAP) {
            full = 1;
            work->unknown_17c = BattleFx_ApplyLookupResult(&actor->x, speed);
        } else if (event == STEP_TERRAIN_FIRST || event == STEP_TERRAIN_LAST) {
            full = work->encounter_full[event - (STEP_TERRAIN_FIRST - 1)];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(event - (STEP_TERRAIN_FIRST - 1), speed);
        } else {
            full = work->encounter_full[0];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(0, speed);
        }
        steps = Iwram_MulQ16(work->encounter_rate, actor->speed_limit);
        if (!full)
            steps /= 2;
        sum = work->encounter_steps + steps;
        work->encounter_steps = sum;
        if (sum > 0xffff) {
            n = sum / 0x10000;
            work->encounter_steps = sum & 0xffff;
            BattleParty_ApplyDrain(n, 0);
            if (BattlePlacement_UpdateTimedEntriesFar())
                AudioCommand_PlayFar(STEP_CUE_EFFECT);
            Event_ClearInvalidPackedValues();
            fell = BattleParty_ApplyStatusDamage();
        }
        if (gGameState.hazard_mode == 0 && event == STEP_EVENT_HAZARD) {
            if (work->previous_tile->collision_code == STEP_EVENT_HAZARD)
                gGameState.hazard_steps += actor->speed_limit / 0x10000;
            else
                gGameState.hazard_steps = gGameState.hazard_limit / 2;
        }
        if (gGameState.avoid_distance != 0 && gGameState.scene_variant != 2) {
            gGameState.avoid_distance -= actor->speed_limit;
            if (gGameState.avoid_distance <= 0) {
                gGameState.avoid_distance = 1;
                if (work->psynergy_request == 0)
                    work->psynergy_request = 0x2096;
            }
        }
    }

    if (gGameState.hazard_mode == 1) {
        gGameState.hazard_steps++;
        if (gGameState.hazard_steps == gGameState.hazard_limit / 2)
            BattleFx_ConfigureLinkedObject(selected, 0x101);
        if (gGameState.hazard_steps == gGameState.hazard_limit)
            BattleFx_ConfigureLinkedObject(selected, 0x100);
    }
    if (gGameState.hazard_steps >= gGameState.hazard_limit) {
        n = gGameState.hazard_damage;
        gGameState.hazard_steps = 0;
        BattleParty_ApplyHealthDelta(-(n & 255), n & 0x100);
        fell++;
    }

    if (fell) {
        work->fallen_count = 0;
        work->standing_count = 0;
        actor->velocity_y = 0x40000;
        BattleFx_ConfigureLinkedObject(selected, 0x102);
        for (i = 0; i < count; i++) {
            owner = (struct StepOwner *)UnitGetFar(gGameState.active_owners[i]);
            if (owner->hp > 0) {
                work->standing_count++;
            } else if (hp[i] != 0) {
                work->fallen[work->fallen_count++] = gGameState.active_owners[i];
                work->raised_trigger = 0xffff;
                owner->doku = 0;
            }
        }
    }
}

#undef gEventWork
#undef BattleParty_ApplyDrain

void UiWork_InitializeWithResourceCountersFar(void);
s32	 GameFlag_TestFar(s32 flag);

#define gOverlayArea (*(struct OverlayEntries *)&gOverlayArea)

#define Map_LoadAreaGraphicsFar ((s32 (*)(void))Map_LoadAreaGraphicsFar)
#define ObjectTable_ResetForObject ((void (*)(void))ObjectTable_ResetForObject)
#define BattleFx_RunDescriptorAction ((void (*)(s32))BattleFx_RunDescriptorAction)
#define BattleFx_RunKind6DescriptorAction ((void (*)(s32))BattleFx_RunKind6DescriptorAction)
#define BattleAction_RunDescriptor ((void (*)(s32))BattleAction_RunDescriptor)
#define Battle_DispatchInputEvent ((void (*)(s32))Battle_DispatchInputEvent)
#define Field_RunTileAction ((void (*)(s32))Field_RunTileAction)
#define BattleCommand_ExecuteSelectedItem ((void (*)(s32, s32))BattleCommand_ExecuteSelectedItem)
#define BattleCommand_ExecuteSelectedAction ((void (*)(s32))BattleCommand_ExecuteSelectedAction)
#define Scene_FadeColorFromWhite ((void (*)(s32))Scene_FadeColorFromWhite)

#define gMenuCtrlWork (*(struct MenuCtrl **)&gMenuCtrlWork)

s32	 Party_CountActiveOwnersFar(void);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);

#define FIELD_AT_OFFSET(base, type, offset) (*(type)((u8 *)(base) + (offset)))

void BattleParty_ApplyDrain(s32 amount)
{
	s32 target_count = Party_CountActiveOwnersFar();
	s32 i;

	for (i = 0; i < target_count; i++)
		UnitEpAddFar(gGameState.active_owners[i], amount);
}

void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled)
{
	s32				count;
	s32				i;
	s32				value;
	struct BtlUnit *unit;

	if (amount < 0)
	{
		BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
		BattleFx_StartBufferInterpolation(4);
		if (amount < -10)
			AudioCommand_PlayFar(SOUND_HEAVY_IMPACT);
		else
			AudioCommand_PlayFar(133);
	}
	else
	{
		AudioCommand_PlayFar(SOUND_RECOVERY);
	}

	count = Party_CountActiveOwnersFar();
	for (i = 0; i < count; i++)
	{
		unit = UnitGetFar(gGameState.active_owners[i]);
		if (!scaled)
		{
			value = amount;
		}
		else
		{
			value = unit->max_hp * amount / 100;
			if (value == 0)
			{
				value = amount;
				if (value < 0)
					value = -value;
			}
		}
		Owner_AdjustFirstValueFar(gGameState.active_owners[i], value);
	}
}

s32 BattleParty_ApplyStatusDamage(void)
{
	s32 result = 0;
	s32 count = Party_CountActiveOwnersFar();
	s32 i;

	for (i = 0; i < count; i++)
	{
		struct BtlUnit *unit = UnitGetFar(gGameState.active_owners[i]);
		s32				amount;

		switch (unit->doku)
		{
		case 1:
			amount = -((unit->max_hp + 10) / 20);
			if (amount == 0)
				amount = -1;
			if (result <= 0)
				result = 1;
			break;
		case 2:
			amount = -((unit->max_hp + 5) / 10);
			if (amount == 0)
				amount = -1;
			if (result <= 1)
				result = 2;
			break;
		default:
			amount = 0;
			break;
		}
		Owner_AdjustFirstValueFar(gGameState.active_owners[i], amount);
	}

	if (result != 0)
	{
		BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
		BattleFx_StartBufferInterpolation(4);
		AudioCommand_PlayFar(133);
	}

	return result;
}

void Battle_SetObjectFlag5bWhenMode3(void)
{
	struct EventRuntime *work;
	void				*blk;

	work = Runtime_AllocateBlock(0x1B, 0xCCC);
	if (work->mode_19e == 3)
	{
		blk = Runtime_AllocateBlock(0x1F, 0x540);
		if ((blk != NULL) && (FIELD_AT_OFFSET(blk, s8 *, 0x53D) != 0))
		{
			FIELD_AT_OFFSET(blk, s8 *, 0x53A) = 0;
			FIELD_AT_OFFSET(blk, s8 *, 0x53B) = 0;
			FIELD_AT_OFFSET(blk, s8 *, 0x53C) = 1;
			FIELD_AT_OFFSET(blk, s8 *, 0x53D) = 0;
		}
		((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 1;
		Map_ResumeAnimationFar();
	}
}

void Battle_ClearObjectFlag5bWhenMode3(void)
{
	struct EventRuntime *work;

	work = Runtime_AllocateBlock(0x1B, 0xCCC);
	if (work->mode_19e == 3)
	{
		Map_LoadAreaGraphicsFar();
		((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 0;
	}
}

struct FieldWork
{
	u8				   unknown_000[0x10];
	s32				   view;
	struct FieldActor *actors[86];
	s16				   action;
	s16				   input;
	s16				   exit;
	s16				   menu;
	s16				   examine;
	s16				   start;
	u16				   descriptor;
	s16				   tile;
	s16				   scene;
	s16				   command;
	s16				   item;
	s16				   event;
	s16				   count;
	s16				   survived;
	s16				   quantities[9];
	s16				   item_argument;
	u8				   unknown_19c[2];
	s16				   mode;
	u8				   unknown_1a0[8];
	s32				   unknown_1a8;
	u8				   unknown_1ac[4];
	s32				   unknown_1b0;
	s32				   unknown_1b4;
	u8				   unknown_1b8[4];
	u8				  *cells;
	s32				   transition;
	u16				   unknown_1c4;
	u16				   transition_active;
	s32				   transition_frames;
	u8				   unknown_1cc[0xaea];
	u16				   busy;
	u8				   unknown_cb8[8];
	u16				   unknown_cc0;
	u8				   unknown_cc2[6];
	u16				   unknown_cc8;
};

struct OverlayEntries
{
	void *unknown_00;
	s32 (*start)(void);
	void *unknown_08[5];
	s32 (*place)(void);
	void *unknown_20;
	s32 (*view)(void);
};

struct MenuCtrl
{
	u8	unknown_00[4];
	u16 wait;
};

void WaitFrames(s32 frames);
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *actor, s32 mode);
void UiText_ShowPositionedMessageAndWaitFar(s32 message, s32 mode);
void UiWork_PushValueSlotFar(s32 quantity, s32 mode);
void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);
void ObjectTable_Snapshot(void);
void Battle_ResetEffectCounter(void);
void Djinn_ResolvePendingEvent();

extern u8			ResourceBlockOwners[];
extern volatile u8	gDebugMode;

static __inline__ s32 Vitals_Clamp(s32 ratio)
{
	s32 clamped;

	if (ratio > 0x4000)
		clamped = 0x4000;
	else if (ratio < 0)
		clamped = 0;
	else
		clamped = ratio;
	return clamped;
}

s32 Field_RunScene(s32 entrance)
{
	void ObjectDispatch_InitFromTable6Far(void *actor);
	volatile s32	   zero;
	struct FieldWork  *work;
	struct FieldActor *actor;
	s32				   mode;
	s32				   result = 0;
	s32				   code;

	work = Runtime_AllocateBlock(27, sizeof(struct FieldWork));
	zero = 0;
	Dma_Set(&zero, work, 0x85000000 | (sizeof(struct FieldWork) / 4), REG_DMA3);
	GameFlag_ClearBitFar(0x103);
	gGameState.default_scene = gGameState.scene;
	gGameState.default_entrance = gGameState.entrance;
	gGameState.next_scene = -1;
	gGameState.next_entrance = -1;
	gGameState.defeat_scene = -1;
	gGameState.defeat_entrance = -1;
	Scheduler_ResetTaskTable();
	Djinn_ResolvePendingEvent(0);
	if ((s16)gGameState.map == (s32)&SceneId_Clear)
	{
		Map_InitializePerspectiveSceneFar();
		gGameState.perspective_scene = 1;
		mode = 3;
		BattleFx_SelectLocationRule(0);
	}
	else
	{
		Map_LoadLayeredSceneFar((s16)gGameState.map);
		mode = 2;
		BattleFx_SelectLocationRule(1);
	}
	work->mode = mode;
	ObjectSystem_InitializeFar(mode);
	UiWork_InitializeWithResourceCountersFar();
	BattleFx_ResetCounters();
	work->view = gOverlayArea.view();
	BattleMap_ApplyEntranceView();
	gOverlayArea.place();
	ObjectTable_ResetForObject();
	if (GameFlag_TestFar(0x109) != 0)
		ObjectTable_Restore();
	if (gGameState.pending_djinn_event != 0)
		FieldObject_PlaceSceneActors();
	if (mode == 3)
	{
		Map_UpdateCurrentTileBlockFar();
		WorldMap_LoadGraphicsFar(gGameState.x, gGameState.z);
	}
	else
	{
		Map_ApplyWorkOriginAndSpanFar();
	}
	Battle_PlaceMapMarkers();
	BattleEffect_InitializeBuffers();
	BattleFx_ApplyColorToTargetBuffer(0x10000, 0);
	work->transition = 0x100;
	work->transition_frames = 16;
	work->transition_active = 0;
	work->unknown_1b0 = 0x199;
	work->unknown_1b4 = 0;
	if (gGameState.scene_variant == 2)
	{
		work->unknown_1b0 = 0;
		GameFlag_SetBitFar(0x144);
	}
	work->cells = Ram_MapCellBuffer;
	if (GameFlag_TestFar(0x109) == 0)
	{
		Party_ResolveTablePair();
		gGameState.hazard_limit = 16;
		gGameState.hazard_mode = 0;
		gGameState.hazard_damage = 1;
		gGameState.paused_object = -1;
		gGameState.cloaked = 0;
	}
	BattleFx_ScheduleCloakedMapCheck();
	work->unknown_cc8 = 0xffff;
	gOverlayArea.start();
	code = work->exit;
	if (code != 0)
	{
		result = code;
		work->exit = 0;
		goto done;
	}
	GameFlag_ClearBitFar(0x109);
	if (BattleFx_SumCounters() == 0)
	{
		if (work->transition_active == 0)
		{
			DisplayTransition_Start(work->transition, work->transition_frames);
			work->transition_active = 1;
			PLTT_BACKDROP = 0;
			if (ObjectEffect_RunPendingFlagEvent() == 0)
				WaitFrames((work->transition_frames + 1) / 2);
		}
		if (GameFlag_TestFar(0x12f) != 0)
		{
			GameFlag_ClearBitFar(0x12f);
			UiTimedNotice_CreateFar(gGameState.scene);
		}
		if (gGameState.pending_djinn_event != 0)
		{
			Djinn_ResolvePendingEvent(gGameState.pending_djinn_event, 1);
			gGameState.pending_djinn_event = 0;
		}
		if (gGameState.pending_item != 0)
		{
			Battle_SetObjectFlag5bWhenMode3();
			if (Party_CheckMemberValueTotal(gGameState.pending_item) == 0)
				PartyInventory_GiveItem(gGameState.pending_item, 0);
			Battle_ClearObjectFlag5bWhenMode3();
			gGameState.pending_item = 0;
		}
	}

	for (;;)
	{
		GameFlag_SetBitFar(0x104);
		actor = work->actors[gGameState.selected_actor];
		ObjectDispatch_InitFromTable6Far(actor);
		gGameState.x = actor->x.fixed;
		gGameState.y = 0;
		gGameState.z = actor->z.fixed;
		gGameState.heading = actor->facing;
		gGameState.turn = actor->terrain_id;

		while (BattleFx_SumCounters() != 0)
		{
			s32 next = work->exit;

			if (next != 0)
			{
				result = next;
				work->exit = 0;
				goto done;
			}
			if (work->event != 0)
			{
				work->busy = 1;
				if (work->event == -1)
				{
					u32 i;

					Battle_InitializeRenderObject();
					Battle_SetObjectFlag5bWhenMode3();
					for (i = 0; i < work->count; i++)
					{
						ObjectDispatch_ApplyArgumentToChildrenFar(actor, 22);
						UiWork_PushValueSlotFar(work->quantities[i], 1);
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldStrengthExhausted, 1);
					}
					if (work->survived == 0)
					{
						struct BtlUnit *vitals;

						if (gGameState.selected_actor == 0)
						{
							if (GameFlag_TestFar(32) != 0)
								ObjectDispatch_ApplyArgumentToChildrenFar(actor, 21);
							else
								ObjectDispatch_ApplyArgumentToChildrenFar(actor, 37);
						}
						else
						{
							ObjectDispatch_ApplyArgumentToChildrenFar(actor, 19);
						}
						AudioCommand_PlayFar(59);
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldPartyAnnihilated, 1);
						vitals = UnitGetFar(gGameState.selected_actor);
						vitals->hp = 1;
						/* FAKEMATCH: needed to match: the one-pass block keeps the reloaded 1 below the hp store */
						do
						{
							vitals->hp_gauge = Vitals_Clamp((vitals->hp << 14) / vitals->max_hp);
							if (vitals->hp_gauge == 0 && vitals->hp != 0)
								vitals->hp_gauge = 1;
						} while (0);
						vitals->ep_gauge = Vitals_Clamp((vitals->ep << 14) / vitals->max_ep);
						if (vitals->ep_gauge == 0 && vitals->ep != 0)
							vitals->ep_gauge = 1;
						gGameState.scene = gGameState.saved_scene;
						gGameState.entrance = gGameState.saved_entrance;
						Battle_ClearObjectFlag5bWhenMode3();
						result = 999;
						goto done;
					}
					Battle_ClearObjectFlag5bWhenMode3();
				}
				else if (work->event == -888)
				{
					Battle_InitializeRenderObject();
					Map_ShowWorldMap((s32)&ResourceId_WorldMapPicture);
				}
				else if (work->event == -889)
				{
					Battle_InitializeRenderObject();
					BattleFx_RunVisibilityTransition();
				}
				else
				{
					BattleFx_RunKind6DescriptorAction(work->event);
				}
				work->busy = 0;
				work->event = 0;
			}
			else if (work->scene != 0)
			{
				Battle_InitializeRenderObject();
				ObjectTable_Snapshot();
				gGameState.return_cue = -1;
				gGameState.scene = 510;
				gGameState.entrance = work->scene;
				work->exit = 999;
				Scene_FadeColorFromWhite(work->scene);
				work->unknown_1a8 = 0;
				work->scene = 0;
				gGameState.encounter_steps = 0;
			}
			else if (work->action != 0)
			{
				work->busy = 1;
				BattleAction_RunDescriptor(work->action);
				work->busy = 0;
				work->action = 0;
			}
			else if (work->input != 0)
			{
				Battle_DispatchInputEvent(work->input);
				work->input = 0;
			}
			else if (work->examine != 0)
			{
				s32 object;

				UiTimedNotice_CloseIfActiveFar();
				object = BattleEffect_SelectNearbyObject(gGameState.selected_actor);
				mode = 0;
				if (object != -1)
				{
					mode = BattleFx_FindDescriptorWithOverride(object) != 0;
				}
				if (mode != 0)
				{
					work->descriptor = object | 0x1000;
					work->menu = 0;
				}
				else
				{
					mode = Object_GetTriggerTileAheadOfCurrent();
					if (mode != 0)
					{
						work->tile = mode;
						work->menu = 0;
					}
					else
					{
						work->menu = 1;
					}
				}
				work->examine = 0;
			}
			else if (work->menu != 0)
			{
				UiTimedNotice_CloseIfActiveFar();
				Battle_InitializeRenderObject();
				AudioCommand_PlayFar(111);
				Battle_SetObjectFlag5bWhenMode3();
				GameFlag_SetBitFar(0x106);
				if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0 && (gKeysHeld & KEY_SELECT) != 0)
				{
					Menu_RunSelectionWithCursorObjectFar();
				}
				else if (GameFlag_TestFar(0x107) != 0)
				{
					work->event = 250;
				}
				else
				{
					work->unknown_cc0 = 0;
					Menu_RunTopSelectionFar();
					work->unknown_cc0 = 1;
				}
				Battle_ClearObjectFlag5bWhenMode3();
				GameFlag_ClearBitFar(0x106);
				GameFlag_RefreshLureCapFar();
				work->menu = 0;
			}
			else if ((s16)work->descriptor != 0)
			{
				Battle_SetObjectFlag5bWhenMode3();
				BattleFx_RunDescriptorAction(work->descriptor & 0xfff);
				Battle_ClearObjectFlag5bWhenMode3();
				work->descriptor = 0;
			}
			else if (work->tile != 0)
			{
				Battle_SetObjectFlag5bWhenMode3();
				Field_RunTileAction(work->tile);
				Battle_ClearObjectFlag5bWhenMode3();
				work->tile = 0;
			}
			else if (work->command != 0)
			{
				UiTimedNotice_CloseIfActiveFar();
				Battle_SetObjectFlag5bWhenMode3();
				BattleCommand_ExecuteSelectedAction(work->command);
				Battle_ClearObjectFlag5bWhenMode3();
				work->command = 0;
			}
			else if (work->item != 0)
			{
				Battle_SetObjectFlag5bWhenMode3();
				BattleCommand_ExecuteSelectedItem(work->item, work->item_argument);
				Battle_ClearObjectFlag5bWhenMode3();
				work->item = 0;
			}
			else if (work->start != 0)
			{
				AudioCommand_PlayFar(111);
				Battle_InitializeRenderObject();
				Battle_SetObjectFlag5bWhenMode3();
				GameFlag_SetBitFar(0x106);
				if (gDebugMode != 0 && (gKeysHeld & KEY_B) != 0)
				{
					Menu_RunSelectionFar();
				}
				else if (gDebugMode != 0 && (gKeysHeld & KEY_L) != 0)
				{
					Debug_RunPaletteEditor();
				}
				else if (GameFlag_TestFar(0x107) != 0)
				{
					work->event = 250;
				}
				else
				{
					Battle_ResetEffectCounter();
					UiTimedNotice_CloseIfActiveFar();
					gGameState.return_cue = AudioCommand_GetSecondaryStateByteFar();
					if (GameFlag_TestFar(0x17e) == 0)
					{
						struct MenuCtrl *ctrl = gMenuCtrlWork;
						u8				*owner = ResourceBlockOwners;
						s32				 free = 0;
						s32				 n = 512;

						do
						{
							if (*owner++ == 0xff)
								free++;
						} while (--n != 0);
						if (free - 136 < 0)
						{
							ctrl->wait = 1;
							WaitFrames(1);
						}
						Menu_RunWorkspaceResultLoopFar(0);
						ctrl->wait = 0;
					}
					else
					{
						UiText_ShowPositionedMessageAndWaitFar((s32)MsgFieldPauseUnavailable, 1);
					}
				}
				Battle_ClearObjectFlag5bWhenMode3();
				GameFlag_ClearBitFar(0x106);
				work->start = 0;
			}
		}

		GameFlag_ClearBitFar(0x104);
		actor = work->actors[gGameState.selected_actor];
		if (actor != NULL)
		{
			if (gGameState.movement_mode == 2)
				ObjectDispatch_InitFromTable0Far(actor);
			else if (gGameState.movement_mode == 1)
				ObjectDispatch_InitFromTable1Far(actor);
			else if (work->mode == 3)
				ObjectDispatch_InitFromTable3Far(actor);
			else
				ObjectDispatch_InitFromTable2Far(actor);
		}
		do
		{
			WaitFrames(1);
			actor = work->actors[gGameState.selected_actor];
			if (gDebugMode == 0 || GameFlag_TestFar(0x163) == 0)
				Field_ProcessStep(
					actor->terrain_id, actor->x.fixed, actor->y.fixed, actor->z.fixed);
		} while (BattleFx_SumCounters() == 0);
	}

done:
	if (work->transition_active != 0)
	{
		DisplayTransition_Finish(work->transition, work->transition_frames);
		work->transition_active = 0;
		WaitFrames(work->transition_frames);
	}
	Runtime_ReleaseHeapBlock(27);
	return result;
}

#undef gOverlayArea

#define ObjectTable_Get ((struct Object * (*)(s32)) ObjectTable_Get)
#define gOverlayArea (*(struct ViewServices *)&gOverlayArea)

#define BattleFx_FindDescriptor ((s32 (*)(s32, s32))BattleFx_FindDescriptor)

struct Vec
{
	s32 x;
	s32 y;
	s32 z;
};

struct Object
{
	u8		   pad0[6];
	u16		   angle;
	struct Vec pos;
	u8		   pad1[14];
	u8		   kind;
};

struct EntranceView
{
	s16 entrance;
	s16 flag;
	s16 x;
	s16 y;
	s16 z;
	u16 heading;
	s16 unknown_0c;
	s16 left;
	s16 top;
	s16 right;
	s16 bottom;
	s16 unknown_16;
};

struct ViewServices
{
	u8 unknown_00[0x0c];
	struct EntranceView *(*entrance_views)(void);
};

struct MapScrollWork_2
{
	u8	unknown_000[0xec];
	s32 left;
	s32 top;
	s32 right;
	s32 bottom;
};

extern volatile u32 gFrameCount;

void WaitFrames(s32 count);
#define PALETTE ((u16 *)0x05000000)
#define EDITOR_MAP ((u16 *)0x0600205a)
#define HEX_DIGIT 0xf0e0

struct ActionDescriptor
{
	u16 sprite;
	s16 condition;
	s32 behavior;
	s32 x;
	s32 y;
	s32 z;
	u16 facing;
	u8	talk_facing;
	u8	flags;
};

struct ActionDescriptorTables
{
	struct ActionDescriptor *tables[4];
};
#define gEventWork (*(struct ActionDescriptorTables *(*))&gEventWork)

s32 Object_GetTriggerTileAheadOfCurrent(void)
{
	void Vector_AddPolarOffset(s32, s32, void *);
	u8			  *state;
	u8			  *map;
	struct Object *obj;
	struct Vec	   pos;
	u8			  *cell;
	u8			  *base;
	s32			   kind;
	s32			   height;
	s32			   result;

	result = 0;
	obj = ObjectTable_Get(gGameState.selected_actor);
	state = *(u8 **)Ram_EventWork;
	map = *(u8 **)Ram_MapWork;
	if (obj != 0)
	{
		pos.x = obj->pos.x;
		pos.y = obj->pos.y;
		pos.z = obj->pos.z;
		Vector_AddPolarOffset(0x100000, obj->angle, &pos);
		if (*(s16 *)(state + 0x19e) == 3)
		{
			cell = Ram_MapBlocks
				   + ((((pos.x / 0x200000) & 31) + (((pos.z / 0x200000) & 31) << 5)) << 2);
		}
		else
		{
			base = *(u8 **)(map + 0x130);
			cell = base + (((pos.x / 0x100000) + ((pos.z / 0x100000) << 7)) << 2);
		}
		kind = cell[2];
		if (kind >= 242 && kind <= 247)
		{
			height = Map_GetTerrainHeightFar(obj->kind, pos.x, pos.z);
			if (height >= obj->pos.y && height <= obj->pos.y + 0x400000)
			{
				result = kind;
			}
		}
		else if (BattleFx_FindDescriptor(3, kind) != 0)
		{
			result = kind;
		}
	}
	return result;
}

void BattleMap_ApplyEntranceView(void)
{
	struct MapScrollWork_2 *work = gMapWork[0];
	s32					  entrance = gGameState.entrance;
	struct EntranceView	 *view = gOverlayArea.entrance_views();
	s32					  found = 0;

	while (view->entrance != -1)
	{
		if (view->entrance == entrance && (view->flag == -1 || GameFlag_TestFar(view->flag)))
		{
			found = 1;
			break;
		}
		view++;
	}
	if (!found)
		view = gOverlayArea.entrance_views();

	if (!GameFlag_TestFar(0x109))
	{
		gGameState.x = view->x << 16;
		gGameState.y = view->y << 16;
		gGameState.z = view->z << 16;
		gGameState.heading = view->heading;
		gGameState.turn = 0;
	}

	if (view->left != -1)
		work->left = view->left << 16;
	if (view->top != -1)
		work->top = view->top << 16;
	if (view->right != -1)
		work->right = view->right << 16;
	if (view->bottom != -1)
		work->bottom = view->bottom << 16;

	if (work->left + (240 << 16) > work->right)
		work->left = work->right - (240 << 16);
	if (work->top + (160 << 16) > work->bottom)
		work->top = work->bottom - (160 << 16);
}

void Debug_RunPaletteEditor(void)
{
	s32	 palette = 0;
	s32	 channel = 1;
	s32	 color = 1;
	u16 *map;
	u16 *p;
	u16 *colors;
	u32	 tile;
	u32	 value;
	u32	 red;
	u32	 green;
	u32	 blue;
	u32	 i;

	Resource_DecompressHalfwords(Debug_PaletteSwatchTiles, (void *)0x06001a00);
redraw:
	tile = (palette << 12) + 0xd1;
	colors = &PALETTE[palette * 16 + 1];
	map = EDITOR_MAP;
	p = map;
	*p = HEX_DIGIT + palette;
	p += 32;
	*p = 0xf000 | 'R';
	p += 32;
	*p = 0xf000 | 'G';
	p += 32;
	*p = 0xf000 | 'B';
	map++;
	for (i = 1; i < 16; i++)
	{
		p = map;
		*p = tile++;
		value = *colors++;
		p += 32;
		*p = (value & 31) + HEX_DIGIT;
		p += 32;
		*p = ((value >> 5) & 31) + HEX_DIGIT;
		p += 32;
		*p = ((value >> 10) & 31) + HEX_DIGIT;
		map++;
	}
	WaitFrames(1);
	for (;;)
	{
		if (gKeysRepeat & KEY_UP)
		{
			channel--;
			if (channel <= 0)
				channel = 3;
		}
		if (gKeysRepeat & KEY_DOWN)
		{
			channel++;
			if (channel > 3)
				channel = 1;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			color--;
			if (color <= 0)
				color = 15;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			color++;
			if (color > 15)
				color = 1;
		}
		if (gKeysRepeat & KEY_L)
		{
			palette--;
			if (palette < 0)
				palette = 13;
			goto redraw;
		}
		if (gKeysRepeat & KEY_R)
		{
			palette++;
			if (palette > 13)
				palette = 0;
			goto redraw;
		}
		if (gKeysRepeat & KEY_A)
		{
			colors = &PALETTE[palette * 16 + color];
			value = *colors;
			red = value & 31;
			green = (value >> 5) & 31;
			blue = (value >> 10) & 31;
			if (channel == 1 && red < 31)
				red++;
			if (channel == 2 && green < 31)
				green++;
			if (channel == 3 && blue < 31)
				blue++;
			*colors = (blue << 10) | (green << 5) | red;
			goto redraw;
		}
		if (gKeysRepeat & KEY_B)
		{
			colors = &PALETTE[palette * 16 + color];
			value = *colors;
			red = value & 31;
			green = (value >> 5) & 31;
			blue = (value >> 10) & 31;
			if (channel == 1 && red != 0)
				red--;
			if (channel == 2 && green != 0)
				green--;
			if (channel == 3 && blue != 0)
				blue--;
			*colors = (blue << 10) | (green << 5) | red;
			goto redraw;
		}
		if (gKeysRepeat & KEY_START)
		{
			colors = &PALETTE[palette * 16 + color];
			value = *colors;
			red = 0;
			for (;;)
			{
				WaitFrames(1);
				if (!(gKeysHeld & KEY_START))
					break;
				if (red == 0)
					*colors = 0x7fff;
				if (red == 10)
					*colors = value;
				if (red == 20)
					*colors = 0;
				if (red == 30)
					*colors = value;
				if (++red >= 40)
					red = 0;
			}
			*colors = value;
		}
		if (gKeysRepeat & KEY_SELECT)
			break;
		gFrameCount;
		WaitFrames(1);
	}
	Ui_LoadWindowGraphics();
	Bg0_ClearTilemap();
}

struct ActionDescriptor *BattleAction_FindDescriptor(s32 id)
{
	struct ActionDescriptor *entry;
	s32						 i;
	s32						 group = 8;

	for (i = 0; i < 4; i++)
	{
		entry = gEventWork->tables[i];
		if (entry == NULL)
			continue;
		if (id <= 7)
		{
			for (; (s16)entry->sprite != -1; entry++)
			{
				if ((s16)entry->sprite == id)
					goto found;
			}
		}
		else
		{
			for (; (s16)entry->sprite != -1; entry++)
			{
				if ((s16)entry->sprite > 7)
				{
					if (group == id)
						goto found;
					group++;
				}
			}
		}
	}
found:
	if ((s16)entry->sprite == -1)
		return NULL;
	return entry;
}

#undef gEventWork

//--------------------------------------------------------------------------

// descript.c

// フィールド：シーンのイベント表を引き、話す・触れる・出口のイベントを動かす

//--------------------------------------------------------------------------

#undef BattleAction_RunDescriptor
#undef BattleFx_FindDescriptor
#undef BattleFx_RunDescriptorAction
#undef BattleFx_RunKind6DescriptorAction
#undef Battle_DispatchInputEvent
#undef ObjectTable_Get
#undef gOverlayArea

#define BattleAction_FindDescriptor ((int (*)())BattleAction_FindDescriptor)

#define gOverlayArea ((u8 *)&gOverlayArea)
#define ObjectTable_Get ((struct EffectObject * (*)(s32)) ObjectTable_Get)

#define ObjectDispatch_ApplyValueToChildrenFar \
	((void (*)(struct BattleActionObject *, s32))ObjectDispatch_ApplyValueToChildrenFar)
#define Field_TryJumpForward ((void (*)(void))Field_TryJumpForward)

extern const s32 ObjectMotion_StepAngleScript[];

// シーンのイベント表の 1 件（struct SceneEvent と同じ 12 バイト）
struct EffectDescriptor
{
	s32 flags; // 下位 4bit が種類（SceneEventKind、7 は止めた相手と話す）
	u8	value; // 相手（オブジェクト番号など）
	u8	unused_05;
	s16 condition; // GameFlag_IsConditionActive の条件
	s32 result;	   // 0x10000 未満はメッセージ、それ以上は呼ぶ関数
};

// イベントの作業領域（gEventWork）の一部
struct EffectDescriptorRuntime
{
	u8						 unused_00[16];
	struct EffectDescriptor *descriptors; // いまのシーンのイベント表（-1 で終わり）
	u8						 unused_14[0x15c];
	s16						 queued_result; // 出口の行き先
	u8						 unused_172[0x2a];
	s16						 limit; // 押し続けた数（EventWork の blocked_steps）
};

struct EffectObject
{
	u8	unused_00[6];
	u16 reference; // 向き
};
#define gEventWork (*(void *(*))&gEventWork)
typedef s32 (*WorkEntryFn)(void);

// 配置表の 1 件（struct ScenePlacement）
struct BattleEffectAction
{
	s16 id;
	u8	unused_02[20];
	s8	mode; // 話しかけたときの向き方（talk_facing）
	u8	unused_17;
};

struct BattleActionObject
{
	void **states;
	s16	   state_index;
	u16	   value;
	s32	   x;
	s32	   y;
	s32	   z;
	u8	   unused_14[0x10];
	s32	   velocity_x;
	s32	   velocity_y;
	s32	   velocity_z;
	u8	   unused_30[8];
	s32	   target_x;
	s32	   target_y;
	s32	   target_z;
	u8	   unused_44[0x16];
	u8	   flags_5a;
	u8	   busy_5b;
	u8	   unused_5c[8];
	u16	   saved_value;
	u8	   unused_66[2];
	void  *linked_object;
};
#define Object_GetById ((struct BattleActionObject *(*)(s32))Object_GetById)
extern s32						  BattleFx_GetFlags(s32);
extern void						  Battle_Reset(void);
extern void						  Event_SetMessageId(s32);
extern void						  BattleFx_FinishAction(void);
extern void						  Object_LinkPair(s32, s32, s32);

// イベント表の 1 件にも、イベントの作業領域にも重ねる（触れる・出口の処理で使う）
typedef struct
{
	s32 flags;
	u8	unk_04[4];
	s32 result;
	u8	unk_0c[0x170 - 0xc];
	s16 queued_result;
	u8	unk_172[0x19c - 0x172];
	s16 limit;
} EffectDescriptorWorkView;

void AudioCommand_PlayFar(s32);

void				BattleFx_RunRisingObjectSequence(s32, s32, s32);
void				FieldEffect_UpdateGridPlacement(void);
void				battle_owner_69(void);

//--------------------------------------------------------------------------
// イベントの条件を調べる
// -1 は常に成り立つ。0x1000 が立っていればフラグが立っているとき、
// そうでなければフラグが立っていないときに成り立つ
//--------------------------------------------------------------------------
s32 GameFlag_IsConditionActive(s32 condition)
{
	s32 on;

	if (condition == -1)
	{
		return 1;
	}
	if (0x1000 & condition)
	{
		return GameFlag_TestFar(condition);
	}
	on = GameFlag_TestFar(condition);
	return on == 0;
}

//--------------------------------------------------------------------------
// 調べるイベント（種類 3）で、条件を見ずに成り立たせるものか
// 対象つき（flags & 0x1ff が 3 でない）で、result の上位 12bit が 0x005 でないとき 1
//--------------------------------------------------------------------------
u32 BattleFx_CheckDescriptorKind3Result(s32 flags, s32 result)
{
	u32 tmp;
	s32 kind;
	u32 ret;

	if ((flags & 0xf) != 3)
		return 0;
	kind = flags & 0x1ff;
	ret = 0;
	if (kind != 3)
	{
		tmp = (result & 0xfff00000) ^ 0x500000;
		ret = tmp | (0U - tmp);
		ret >>= 31; // tmp != 0 なら 1
	}
	return ret;
}

//--------------------------------------------------------------------------
// いまのシーンのイベント表から、種類と相手が合い、条件の成り立つものを探す
// flags 0x200 : 押し続けた数が 12（0x800 なら 2）を超えたときだけ
// flags 0x400 : 先頭の向きが flags の上位 4bit の向きから 1/8 周以内のときだけ
// kind  : 種類
// value : 相手
//--------------------------------------------------------------------------
struct EffectDescriptor *BattleFx_FindDescriptor(s32 kind, s32 value)
{
	struct EffectDescriptorRuntime *work = gEventWork;
	struct EffectDescriptor		   *ev = work->descriptors;
	s32								flags;
	u32								facing = ObjectTable_Get(gGameState.selected_actor)->reference;

	flags = ev->flags;
	while (flags != -1)
	{
		if ((flags & 0xf) == kind && ev->value == value
			&& (BattleFx_CheckDescriptorKind3Result(flags, ev->result) != 0
				|| (GameFlag_IsConditionActive(ev->condition) != 0 && (flags = ev->flags, 1))))
		{
			s32 ok = 0;
			s32 steps = 12;
			s32 need;

			if ((flags & 0x800) != 0)
				steps = 2;
			need = flags & 0x600;

			switch (need)
			{
			case 0:
				ok = 1;
				break;
			case 0x200: // 押し続け
				if (work->limit > steps)
				{
					ok = 1;
				}
				break;
			case 0x400: // 向き
				flags &= 0xf000;
				ok = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
				break;
			case 0x600: // 両方
				if (work->limit > steps)
				{
					flags &= 0xf000;
					ok = (u16)((flags - facing) + 0x1fff) <= 0x3ffe;
				}
				break;
			}
			if (ok)
			{
				return ev;
			}
		}
		ev++;
		if (ev->flags == -1)
			break;
		flags = ev->flags;
	}
	return 0;
}

//--------------------------------------------------------------------------
// オーバーレイ領域に読み込んだシーンの初期化を呼ぶ
//--------------------------------------------------------------------------
void Runtime_CallWorkBlockEntry(void)
{
	u8 *base = gOverlayArea;

	(*(WorkEntryFn *)(base + 4))(); // +4 は SceneEntries の initialize
}

//--------------------------------------------------------------------------
// 話すイベントを探す（止めている相手なら、種類 7 のほうを先に見る）
//--------------------------------------------------------------------------
s32 BattleFx_FindDescriptorWithOverride(s32 id)
{
	struct EffectDescriptor *ev = BattleFx_FindDescriptor(0, id);
	s32						 paused = gGameState.paused_object;

	if (paused == id)
	{
		struct EffectDescriptor *alt = BattleFx_FindDescriptor(7, paused);

		if (alt != 0)
		{
			return (s32)alt;
		}
	}
	return (s32)ev;
}

//--------------------------------------------------------------------------
// オブジェクト id に話しかける
// 止めている相手は種類 7 を先に使い、なくて種類 0 が関数なら
// MsgPonderEllipsis（「・・・。」）からのメッセージで済ませる
// 戻り値 : 動かしたら 0、なければ -1
//--------------------------------------------------------------------------
s32 BattleFx_RunDescriptorAction(s32 id)
{
	struct BattleEffectAction *place = (struct BattleEffectAction *)BattleAction_FindDescriptor(id);
	s32						   ret = -1;
	struct BattleActionObject *obj = Object_GetById(id);
	struct EffectDescriptor	  *ev;
	s32						   paused = 0;
	u32						   saved_value;
	u32						   mode;
	s32						   talk = 0;

	if (gGameState.paused_object == id)
	{
		paused = 1;
		ev = BattleFx_FindDescriptor(7, id);
		if (ev == 0)
		{
			ev = BattleFx_FindDescriptor(0, id);
			talk = 1;
			if (ev == 0)
			{
				return -1;
			}
			if (ev->result >= 0x10000)
			{
				s32 no = BattleFx_GetFlags(id);
				u32 rnd = Random16();
				s32 mes = (s32)MsgPonderEllipsis + no * 2 + (rnd * 2 >> 16);
				Battle_Reset();
				Event_SetMessageId(mes);
				BattleEv_RunWait(id, 0);
				BattleFx_FinishAction();
				goto finish;
			}
		}
	}
	else
	{
		ev = BattleFx_FindDescriptor(0, id);
	}

	if (ev != 0 && ev->result != 0)
	{
		if (!paused)
		{
			// 相手を止め、向き方が 0・1・3 なら先頭も止めて相手とつなぐ
			u8 *busy = &obj->busy_5b;

			*busy = 1;
			ObjectDispatch_ApplyValueToChildrenFar(obj, 0);
			saved_value = obj->value;
			mode = (u8)place->mode << 24;
			if (mode <= (1 << 24) || mode == (3 << 24))
			{
				struct BattleActionObject *leader = Object_GetById(gGameState.selected_actor);

				leader->target_x = leader->x;
				leader->target_y = leader->y;
				leader->target_z = leader->z;
				leader->velocity_x = 0;
				leader->velocity_y = 0;
				leader->velocity_z = 0;
				Object_LinkPair(id, gGameState.selected_actor, 0);
			}
		}
		if (ev->result < 0x10000)
		{
			UiWork_SetBusyFlagsFar(talk);
			Battle_Reset();
			Event_SetMessageId(ev->result);
			BattleEv_RunWait(id, 0);
			BattleFx_FinishAction();
		}
		else
		{
			Battle_InitializeRenderObject();
			((void (*)(s32))ev->result)(id);
		}
		if (!paused)
		{
			// 相手の状態が 16 なら、向き方に合わせて動きを戻す
			if (obj->states[obj->state_index] == (void *)16)
			{
				if (place->mode == 3)
				{
					obj->linked_object = Object_GetById(gGameState.selected_actor);
					obj->flags_5a |= 1;
					ObjectMotion_SetActionCallback(obj, (void *)ObjectMotion_LinkedActionScript);
				}
				else if (place->mode == 1)
				{
					obj->saved_value = saved_value;
					ObjectDispatch_InitializeFar(
						(struct DispatchObject *)obj, ObjectMotion_StepAngleScript);
				}
			}
			obj->busy_5b = 0;
			ObjectDispatch_ApplyValueToChildrenFar(obj, 16);
		}
		ret = 0;
	}
finish:
	if (paused)
	{
		BattleFx_ResumeObject(gGameState.paused_object);
		gGameState.paused_object = -1;
	}
	return ret;
}

//--------------------------------------------------------------------------
// 種類 6 のイベントを動かす
// 戻り値 : 動かしたら 0、なければ -1
//--------------------------------------------------------------------------
s32 BattleFx_RunKind6DescriptorAction(s32 value)
{
	s32						 res;
	s32						 ret;
	struct EffectDescriptor *ev;

	ev = BattleFx_FindDescriptor(6, value);
	ret = -1;
	if (ev != NULL)
	{
		res = ev->result;
		if (res != 0)
		{
			if (res < 0x10000)
			{
				Battle_Reset();
				Event_SetMessageId(ev->result);
				BattleEv_RunWait(-1, 0);
				ret = 0;
				BattleFx_FinishAction();
			}
			else
			{
				((void (*)(s32))res)(value);
				ret = 0;
			}
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
// 触れたものの触れるイベント（種類 2）を動かし、なければ出口（種類 1）へ
// 出口は音を鳴らし、行き先を積んでおく
// 戻り値 : 動かしたら 0、なければ -1
//--------------------------------------------------------------------------
s32 BattleAction_RunDescriptor(s32 value)
{
	EffectDescriptorWorkView *touch;
	EffectDescriptorWorkView *exit;
	s32						  kind;
	s32						  ret;
	EffectDescriptorWorkView *work;

	touch = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(2, value);
	ret = -1;
	work = *(EffectDescriptorWorkView **)&gEventWork;
	if ((touch != 0) && (touch->result != 0))
	{
		if (touch->flags & 0x200)
		{
			work->limit = 0;
		}
		if (touch->result < 0x10000)
		{
			Battle_Reset();
			Event_SetMessageId(touch->result);
			BattleEv_RunWait(-1, 0);
			ret = 0;
			BattleFx_FinishAction();
		}
		else
		{
			((void (*)(s32))touch->result)(value);
			goto done;
		}
	}
	else
	{
		exit = (EffectDescriptorWorkView *)BattleFx_FindDescriptor(1, value);
		if (exit != 0)
		{
			kind = exit->flags & 0x30;
			switch (kind)
			{
			case 0:
				AudioCommand_PlayFar(123);
				break;
			case 32:
				AudioCommand_PlayFar(128);
				ObjectEffect_BeginContextEffect26();
				break;
			case 48:
				AudioCommand_PlayFar(129);
				ObjectEffect_BeginContextEffect25();
				break;
			}
			work->queued_result = exit->result;
		done:
			ret = 0;
		}
	}
	return ret;
}

//--------------------------------------------------------------------------
// 入力イベント（0xf9・0xfc〜0xfe）の処理
// 押し続けた数が 12 を超えたら動かし、数え直す
//--------------------------------------------------------------------------
s32 Battle_DispatchInputEvent(s32 event)
{
	struct EffectDescriptorRuntime *work = gEventWork;
	s32								leader = gGameState.selected_actor;

	switch (event)
	{
	case 0xfc: // 下を押していれば BattleFx_RunRisingObjectSequence
		if (work->limit > 12 && (gKeysHeld & KEY_DOWN))
		{
			BattleFx_RunRisingObjectSequence(leader, 6, 0);
			work->limit = 0;
		}
		break;
	case 0xf9:
	case 0xfe: // 前へ跳ぶ
		if (work->limit > 12)
		{
			Field_TryJumpForward();
			work->limit = 0;
		}
		break;
	case 0xfd: // 下なら FieldEffect_UpdateGridPlacement、上なら battle_owner_69
		if (work->limit > 12)
		{
			if (gKeysHeld & KEY_DOWN)
			{
				FieldEffect_UpdateGridPlacement();
			}
			else if (gKeysHeld & KEY_UP)
			{
				battle_owner_69();
			}
			work->limit = 0;
		}
		break;
	}
	return 0;
}

#undef BattleAction_FindDescriptor
#undef gEventWork
#undef Object_GetById

#undef Field_RunTileAction

#define EffectRuntime_GetCurrentObject ((void * (*)(s32))EffectRuntime_GetCurrentObject)
#define EffectRuntime_PrepareRisingObject ((void (*)(void*))EffectRuntime_PrepareRisingObject)

struct EffectDescriptor;
#define gEventWork (*(struct EventRuntime *(*))&gEventWork)
#define ObjectDispatch_ApplyArgumentToChildrenFar ((void (*)(void *object, s32 mode))ObjectDispatch_ApplyArgumentToChildrenFar)
#define ObjectDispatch_ReleaseFar ((void (*)(void *object))ObjectDispatch_ReleaseFar)
void UiWork_PushValueSlotFar(s32 quantity, s32 slot);
void Party_AdjustCoinsFar(s32 coins);
s32 BattleFx_GetWeightedResult(s32 weight, s32 group);
void BattleFx_SelectBattleCue(s32 weight, s32 group);
#define BattleParty_ApplyDrain ((void (*)(s32 amount, s32 mode))BattleParty_ApplyDrain)
void *BattleFx_StartRandomParticleEmitter(s32 actor, s32 item);
void Battle_WaitMode0(s32 frames);
void Battle_Reset(void);
void BattleFx_FinishAction(void);
void BattleFx_PlayQueuedSound(void);

s32 Field_RunTileAction(s32 tile)
{
    const struct SceneEvent *descriptor;
    s32 leader;
    u32 n;
    s16 condition;
    u32 action;
    s32 type;
    s32 allowed;

    leader = gGameState.selected_actor;
    n = tile - 242;
    if (n <= 5) {
        Battle_InitializeRenderObject();
        tile = Field_TileMessageOffsets[n];
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileChecked + tile, 1);
        UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + tile, 1);
    } else {
        descriptor = (const struct SceneEvent *)BattleFx_FindDescriptor(3, tile);
        if (descriptor != NULL) {
            s32 flags = (s32)descriptor->control;

            condition = descriptor->condition;
            n = (flags >> 4) & 31;
            if (((u16)descriptor->trigger & 0x400) == 0 && n != 0) {
                Battle_InitializeRenderObject();
                UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileChecked + n, 1);
                GameFlag_SetBitFar(0x142);
            } else {
                GameFlag_ClearBitFar(0x142);
            }
            action = descriptor->value;
            if ((action & 0x0f000000) != 0 || (action & 0xfff00000) == 0x400000) {
                if ((action & 0x0f000000) != 0) {
                    if (GameFlag_IsConditionActive(condition) != 0)
                        ((void (*)(s32))descriptor->value)(gGameState.selected_actor);
                    if (GameFlag_TestFar(0x142) != 0)
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + n, 1);
                } else {
                    if (GameFlag_IsConditionActive(condition) != 0)
                        UiText_ShowPositionedMessageAndWaitFar((*(const u16 *)&descriptor->value), 1);
                    else
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgButFoundNothing, 1);
                }
            } else {
                Battle_Reset();
                if (GameFlag_IsConditionActive(condition) != 0) {
                    action = descriptor->value;
                    allowed = 1;
                    if ((action & 0xf0000) == 0x10000 && leader <= 7)
                        allowed = 0;
                    if (allowed != 0) {
                        if (((s32)descriptor->control & 0x1ff) == 19)
                            EffectRuntime_SetMode4AndPlayCue(tile);
                        type = descriptor->value & 0xfff00000;
                        if (type == 0x300000) {
                            void *object;

                            if (((s32)descriptor->control & 0x1ff) == 19)
                                EffectRuntime_SetMode2(tile);
                            object = EffectRuntime_GetCurrentObject(tile);
                            EffectRuntime_PrepareRisingObject(object);
                            AudioCommand_PlayFar(83);
                            UiWork_PushValueSlotFar((*(const u16 *)&descriptor->value), 5);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone, 3);
                            BattleParty_ApplyDrain(999, 0);
                            UiWindow_CreateWithLayoutBoundsFar(1);
                            AudioCommand_PlayFar(126);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone + 1, 1);
                            UiWork_FinalizeAndReleaseBlock16Far();
                            ObjectDispatch_ApplyArgumentToChildrenFar(object, 2);
                            AudioCommand_PlayFar(246);
                            Battle_WaitMode0(30);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotPsynergyStone + 2, 1);
                            EffectRuntime_ClearCurrentFlags(tile);
                            if (condition != -1)
                                GameFlag_SetBitFar(condition);
                        } else if (type == 0x500000) {
                            struct EventRuntime *work = gEventWork;

                            if (((s32)descriptor->control & 0x1ff) == 19)
                                EffectRuntime_SetMode7AndLaunch(tile);
                            if (condition != -1) {
                                condition = gGameState.pending_djinn_event = condition | 0x1000;
                            }
                            work->value_17c = BattleFx_GetWeightedResult(99, (*(const u16 *)&descriptor->value));
                            Battle_SetStart(2);
                            BattleFx_SelectBattleCue(99, (*(const u16 *)&descriptor->value));
                            AudioCommand_PlayFar(gGameState.scene_cue);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgChestWasMimic, 1);
                        } else if (type == 0x200000) {
                            void *object;

                            object = BattleFx_StartRandomParticleEmitter(gGameState.selected_actor, 0);
                            WaitFrames(30);
                            if (((s32)descriptor->control & 0x1ff) == 19)
                                EffectRuntime_SetMode2(tile);
                            EffectRuntime_PrepareRisingObject(object);
                            AudioCommand_PlayFar(83);
                            UiWork_PushValueSlotFar((*(const u16 *)&descriptor->value), 5);
                            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGotCoins, 3);
                            Party_AdjustCoinsFar((*(const u16 *)&descriptor->value));
                            if (condition != -1)
                                GameFlag_SetBitFar(condition);
                            ObjectDispatch_ReleaseFar(object);
                        } else {
                            void *object;
                            s32 owner;

                            object = BattleFx_StartRandomParticleEmitter(gGameState.selected_actor, descriptor->value & 0xfff);
                            WaitFrames(30);
                            owner = PartyInventory_AddFar((*(const u16 *)&descriptor->value));
                            n = 0xffff;
                            if (owner == -1) {
                                UiWork_PushValueSlotFar(descriptor->value & 0xfff, 2);
                                UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem, 1);
                                UiText_ShowPositionedMessageAndWaitFar((s32)MsgFoundItem + 4, 1);
                                Object_DestroyIfPresent(object);
                                if (((s32)descriptor->control & 0x1ff) == 19)
                                    EffectRuntime_SetMode5AndPlayCue(tile);
                            } else {
                                if (((s32)descriptor->control & 0x1ff) == 19)
                                    EffectRuntime_SetMode2(tile);
                                EffectRuntime_PrepareRisingObject(object);
                                AudioCommand_PlayFar(83);
                                UiWork_PushValueSlotFar(descriptor->value & n, 2);
                                if (owner == gGameState.selected_actor) {
                                    UiText_ShowPositionedMessageAndWaitFar((s32)MsgKorosseoRobinGotItem, 3);
                                } else {
                                    UiWork_PushValueSlotFar(owner, 1);
                                    UiText_ShowPositionedMessageAndWaitFar((s32)MsgGaveItemToMember, 3);
                                }
                                if (condition != -1)
                                    GameFlag_SetBitFar(condition);
                                ObjectDispatch_ReleaseFar(object);
                            }
                        }
                    } else {
                        UiText_ShowPositionedMessageAndWaitFar((s32)MsgCantOpenIt, 1);
                    }
                } else {
                    UiText_ShowPositionedMessageAndWaitFar((s32)MsgTileResult + n, 1);
                }
                BattleFx_FinishAction();
                BattleFx_PlayQueuedSound();
            }
        } else {
            UiText_ShowPositionedMessageAndWaitFar((s32)MsgCheckedGround, 1);
            UiText_ShowPositionedMessageAndWaitFar((s32)MsgGroundDidntFindAnything, 1);
        }
        GameFlag_ClearBitFar(0x142);
    }
    return 0;
}

#undef gEventWork
#undef ObjectDispatch_ApplyArgumentToChildrenFar
#undef ObjectDispatch_ReleaseFar
#undef BattleParty_ApplyDrain
