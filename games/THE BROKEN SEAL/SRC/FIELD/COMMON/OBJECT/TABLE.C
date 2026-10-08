#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "TYPES.H"
#include "GAME_STATE.H"
#include "FIELD_SCENE.H"
#include "MAP.H"
#include "MAP_SCROLL.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "OBJECT_RUNTIME.H"
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "ANIMSPR.H"
#include "FIELDRUN.H"
#include "FIELDOBJ.H"
#include "OBJECT_DISPATCH.H"
#include "FIELD_EVENT.H"
#include "BTLSYS.H"

#define Object_Destroy ((void (*)(void*))Object_Destroy)

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

extern const struct ScenePlacement Field_DefaultPlacements[2];
void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *object, u32 value);
struct ObjectRuntime *Object_CreateFar(s32 character, s32 x, s32 y, s32 z);
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
            Object_SetMode(object, 1);
            if (object->animation_kind == 1 && (sprite = (struct AnimationObject *)object->animation) != 0)
                sprite->last_no = __umodsi3(Random16(), 30);
            object->angle = entry->facing;
            object->unknown_59 = 1;
            ObjectMotion_SetActionCallback(object, entry->behavior);
            Object_SetMode(object, 1);
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
        Object_Destroy(object);
        state->actors[index] = NULL;
    }
}

void ObjectTable_ResetForObject(struct ScenePlacement *table)
{
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
        Object_SetMode(object, 12);
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
