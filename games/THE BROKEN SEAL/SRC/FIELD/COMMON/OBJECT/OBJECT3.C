#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"

struct ObjectTableWork {
    u8 unknown_00[0x14];
    void *objects[192];
};

extern struct ObjectTableWork *gEventWork;
void *ObjectTable_Get(s32);
void Object_Destroy(void *);

/* One row of a scene's object table; a row whose id is -1 ends it. */
struct EventObjectEntry {
    s16 id;                     /* 0x00 */
    s16 condition;              /* 0x02 */
    s32 action;                 /* 0x04 */
    s32 x;                      /* 0x08 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 */
    u16 facing;                 /* 0x14 */
    u8 unknown_16;
    u8 flags;                   /* 0x17 */
};

struct EventSprite {
    u8 unknown_00[0x18];
    s32 scale;                  /* 0x18 */
    u8 resource;                /* 0x1c */
    u8 flags;                   /* 0x1d */
    u8 unknown_1e[6];
    u8 phase;                   /* 0x24 */
};

struct EventObject {
    u8 unknown_00[6];
    u16 facing;                 /* 0x06 */
    s32 x;                      /* 0x08 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 */
    s32 ground;                 /* 0x14 */
    u8 unknown_18[0x0a];
    u8 terrain_id;              /* 0x22 */
    u8 visible;                 /* 0x23 */
    u8 unknown_24[0x2c];
    void *sprite;               /* 0x50 */
    u8 kind;                    /* 0x54 */
    u8 mode;                    /* 0x55 */
    u8 unknown_56[3];
    u8 active;                  /* 0x59 */
    u8 unknown_5a[0x0a];
    s16 cell_x;                 /* 0x64 */
    s16 cell_z;                 /* 0x66 */
};

struct ObjectWork {
    s32 header[4];                          /* 0x000 */
    u8 unknown_010[4];
    struct EventObject *objects[0x62];      /* 0x014 */
    u8 unknown_19c[2];
    s16 scene_mode;                         /* 0x19e */
    u8 unknown_1a0[0x40];
    struct EventObject *camera_object;      /* 0x1e0 */
    u8 unknown_1e4[0x1c];
    struct EventObjectEntry player[2];      /* 0x200 */
};

struct PlayerState {
    u8 unknown_000[0x1dc];
    s32 x;                      /* 0x1dc */
    s32 y;                      /* 0x1e0 */
    s32 z;                      /* 0x1e4 */
    s32 facing;                 /* 0x1e8 */

    u16 terrain_id;             /* 0x1ec */
    u8 unknown_1ee[4];
    u8 on_ladder;               /* 0x1f2 */
    u8 unknown_1f3;
    s32 leader;                 /* 0x1f4 */
};

/* One cell of the field map's 128-cell-wide collision grid in EWRAM. */
struct MapCell {
    u8 unknown_0[2];
    u8 kind;
    u8 unknown_3;
};

#define MAP_CELLS ((struct MapCell *)Ram_MapCellBuffer)

struct ResourceMetadata {
    u8 unknown_0[5];
    u8 width;
    u8 height;
};

extern struct ObjectWork *Data_03001ebc;
extern struct PlayerState Data_02000240;
extern const struct EventObjectEntry Data_0809f810[2];
extern void **gCam;
void ObjectTable_ClearBattleSlots(void);
void Event_SpawnObjectTable(struct EventObjectEntry *entry, s32 slot);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void ObjectDispatch_SetSingleChildField26Far(struct EventObject *object, s32 value);
void Object_SetMode(struct EventObject *object, s32 mode);
struct EventObject *Object_CreateFar(s32 character, s32 x, s32 y, s32 z);
void ObjectDispatch_InitFromTable4WithArgumentFar(struct EventObject *object, struct EventObject *source);
struct ResourceMetadata *ResourceMetadata_RegisterFar(void *sprite, s32 kind);
s32 GameFlag_IsConditionActive(s32 condition);
s32 Party_RemapCharacterIdByFlags(s32 id);
void Resource_ResetEntry(s32 entry);
s32 GameFlag_TestFar(s32 flag);
void ObjectDispatch_RegisterChildMetadataFar(struct EventObject *object, s32 value);
void Object_SetPositionAndResetMotionFar(struct EventObject *object, s32 x, s32 y, s32 z);
u32 Random16(void);
u32 __umodsi3(u32 numerator, u32 denominator);
void ObjectMotion_SetActionCallback(struct EventObject *object, s32 action);

/* object/table/ObjectTable_FindLastActiveId.c */
struct State_0808b824 {
    u8 padding[0x34];
    s32 values[58];
};

extern struct State_0808b824 *gWork;

/* Registers a scene's object table in the first free of the work's four
   table slots, then makes an object for each of its rows whose condition
   holds: party rows (ids up to 7) take their own table index, the others the
   next slot from `slot` up to 65. A row whose object already stands is moved
   to its place instead. */
void Event_SpawnObjectTable(struct EventObjectEntry *entry, s32 slot)
{
    struct ObjectWork *work;
    struct EventObject *object;
    struct EventObject *previous;
    struct EventSprite *sprite;
    s32 i;
    s32 index;
    u32 offset;
    s32 character;
    s32 condition;
    u8 resource;
    u16 id;

    work = Data_03001ebc;
    for (i = 0; i < 4; i++) {
        if (work->header[i] == (s32)entry)
            break;
        if (work->header[i] == 0) {
            work->header[i] = (s32)entry;
            break;
        }
    }
    for (id = entry->id; (s16)id != -1 && slot <= 65; entry++, id = entry->id) {
        /* FAKEMATCH: the ROM reads the row id once per step and extends it
           again in the body; without this second read the compiler shares
           the shifted id between the test and the body. */
        id = entry->id;
        if ((s16)id <= 7)
            index = (s16)id;
        else if ((s16)id <= 0x2705)
            index = slot++;
        condition = entry->condition;
        if (!GameFlag_IsConditionActive(condition))
            continue;
        if ((u32)(condition - 48) <= 79 && work->scene_mode != 3
            && !GameFlag_IsConditionActive(condition + 80))
            continue;
        character = Party_RemapCharacterIdByFlags(entry->id);
        object = ObjectTable_Get(index);
        if (object == 0) {
            object = Object_CreateFar(character, entry->x, entry->y, entry->z);
            if (entry->flags & 1) {
                previous = ObjectTable_Get(index - 1);
                if (previous->kind == 1 && object->kind == 1) {
                    sprite = (struct EventSprite *)previous->sprite;
                    sprite->flags |= 1;
                    resource = sprite->resource;
                    sprite = (struct EventSprite *)object->sprite;
                    sprite->flags |= 1;
                    Resource_ResetEntry(sprite->resource);
                    sprite->resource = resource;
                }
            }
            if (GameFlag_TestFar(33) && (u32)(character - 18) <= 1)
                ObjectDispatch_RegisterChildMetadataFar(object, 226);
        } else if (!GameFlag_TestFar(0x109)) {
            Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
        }
        if (object) {
            Object_SetMode(object, 1);
            if (object->kind == 1 && (sprite = (struct EventSprite *)object->sprite) != 0)
                sprite->phase = __umodsi3(Random16(), 30);
            object->facing = entry->facing;
            object->active = 1;
            ObjectMotion_SetActionCallback(object, entry->action);
            Object_SetMode(object, 1);
            object->cell_x = object->x / 0x10000;
            object->cell_z = object->z / 0x10000;
            if (object->y != 0) {
                object->mode = 4;
                object->y += 0x8000;
            }
            if (work->scene_mode == 3) {
                object->mode &= 0xfe;
                if (!GameFlag_TestFar(33))
                    sprite->scale = Iwram_MulQ16(sprite->scale, 0xc000);
            } else {
                object->ground = Map_GetTerrainHeightFar(0, object->x, object->z);
                object->y += object->ground;
            }
            object->visible = 1;
        }
        offset = index * 4 + 0x14;
        *(struct EventObject **)((u8 *)work + offset) = object;
    }
}

void ObjectTable_DestroyAtIndex(s32 index)
{
    struct ObjectTableWork *state = gEventWork;
    void *object = ObjectTable_Get(index);

    if (object != NULL) {
        Object_Destroy(object);
        state->objects[index] = NULL;
    }
}

/* Rebuild the object table for a new scene: copy the player template, spawn
 * the leader and the scene's own objects, put the leader on a ladder when
 * the map cell and the one north of it are both ladder cells (kind 0xfd),
 * and create the camera object that follows it. */
void ObjectTable_ResetForObject(struct EventObjectEntry *table)
{
    struct ObjectWork *work;
    struct EventObjectEntry *entry;
    struct EventObject *object;
    struct EventObject *camera;
    struct MapCell *cell;
    struct MapCell *above;
    s32 leader;
    s32 pos;
    s32 hgt;

    work = Data_03001ebc;
    leader = Data_02000240.leader;
    entry = &work->player[0];
    work->player[0] = Data_0809f810[0];
    work->player[1] = Data_0809f810[1];
    for (pos = 0; pos < 4; pos++)
        work->header[pos] = 0;
    ObjectTable_ClearBattleSlots();
    entry->condition = -1;
    entry->id = leader;
    entry->x = Data_02000240.x;
    entry->y = 0;
    entry->z = Data_02000240.z;
    entry->facing = Data_02000240.facing;
    Event_SpawnObjectTable(entry, leader);
    Event_SpawnObjectTable(table, 8);

    object = work->objects[leader];
    object->terrain_id = Data_02000240.terrain_id;
    pos = (object->x / 0x100000) + (object->z / 0x100000) * 128;
    cell = &MAP_CELLS[pos];
    above = &MAP_CELLS[pos - 128];
    if (Data_02000240.y != 0 && cell->kind == 0xfd && above->kind == 0xfd) {
        Data_02000240.on_ladder = 1;
        hgt = Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) - 0x200000;
        object->y += hgt;
        object->ground = object->y;
        object->mode = 0;
        ObjectDispatch_SetSingleChildField26Far(object, 0);
        Object_SetMode(object, 12);
    } else {
        Data_02000240.on_ladder = 0;
    }

    camera = Object_CreateFar(0x8000, object->x, object->y, object->z);
    camera->ground = object->ground;
    ObjectDispatch_InitFromTable4WithArgumentFar(camera, object);
    if (work->scene_mode == 3) {
        struct ResourceMetadata *meta = ResourceMetadata_RegisterFar(object->sprite, 23);
        meta->width = 15;
        meta->height = 9;
    }
    *gCam = &camera->x;
    work->camera_object = camera;
}

s32 ObjectTable_FindLastActiveId(void)
{
    struct State_0808b824 *state = gWork;
    s32 result = 7;
    s32 index = 8;
    s32 *value = state->values;

    do {
        s32 current = *value++;

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

/* object/table/ObjectTable_GetSlotAddress.c */
void *ObjectTable_GetSlotAddress(u32 index)
{
    return *(u8 **)((u32)&Data_03001ebc) + index * 4 + 20;
}
