/* Ported from ☀️ games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/OBJECT3.C. */
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
#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

/* Registers a scene's object table in the first free of the work's four
   table slots, then makes an object for each of its rows whose condition
   holds: party rows (ids up to 7) take their own table index, the others the
   next slot from `slot` up to 65. A row whose object already stands is moved
   to its place instead. */
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
        /* FAKEMATCH: the ROM reads the row id once per step and extends it
           again in the body; without this second read the compiler shares
           the shifted id between the test and the body. */
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
