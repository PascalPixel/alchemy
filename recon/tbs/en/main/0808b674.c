/* Draft, not exact (2026-09-24): 432 of 432 bytes, 63 halfwords differ.
 * The whole owner is hand-written; the residual is register allocation:
 * the reference keeps work in r8 and leader in r7 and compares the header
 * clearing loop against a copy of work in ip (a biv-eliminated counter
 * loop), while every counter, pointer and do/while spelling here either
 * keeps the counter or compares against work itself, which promotes work
 * to r7. Declaring the row above as its own extern (Data_0200fe00) takes
 * it to 55; the height add (y + (h - 0x200000)) and the Object_CreateFar
 * argument load order follow from the same allocation. */

#include "TYPES.H"

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

struct EventObject {
    u8 unknown_00[8];
    s32 x;                      /* 0x08 */
    s32 y;                      /* 0x0c */
    s32 z;                      /* 0x10 */
    s32 ground;                 /* 0x14 */
    u8 unknown_18[0x0a];
    u8 terrain_id;              /* 0x22 */
    u8 unknown_23[0x2d];
    void *sprite;               /* 0x50 */
    u8 kind;                    /* 0x54 */
    u8 mode;                    /* 0x55 */
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

struct MapCell {
    u8 unknown_0[2];
    u8 kind;
    u8 unknown_3;
};

struct ResourceMetadata {
    u8 unknown_0[5];
    u8 width;
    u8 height;
};

extern struct ObjectWork *Data_03001ebc;
extern struct PlayerState Data_02000240;
extern const struct EventObjectEntry Data_0809f810[2];
extern struct MapCell Data_02010000[];
extern void **Data_03001e70;

void ObjectTable_ClearBattleSlots(void);
void Event_SpawnObjectTable(struct EventObjectEntry *entry, s32 slot);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
void ObjectDispatch_SetSingleChildField26Far(struct EventObject *object, s32 value);
void Object_SetMode(struct EventObject *object, s32 mode);
struct EventObject *Object_CreateFar(s32 character, s32 x, s32 y, s32 z);
void ObjectDispatch_InitFromTable4WithArgumentFar(struct EventObject *object, struct EventObject *source);
struct ResourceMetadata *ResourceMetadata_RegisterFar(void *sprite, s32 kind);

void ObjectTable_ResetForObject(struct EventObjectEntry *table)
{
    struct ObjectWork *work;
    struct EventObjectEntry *entry;
    struct EventObject *object;
    struct EventObject *camera;
    struct MapCell *cell;
    struct MapCell *above;
    s32 leader;
    s32 *header;
    s32 pos;

    work = Data_03001ebc;
    leader = Data_02000240.leader;
    entry = &work->player[0];
    work->player[0] = Data_0809f810[0];
    work->player[1] = Data_0809f810[1];
    header = &work->header[3];
    do {
        *header = 0;
        header--;
    } while ((s32)header >= (s32)work->header);
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
    cell = &Data_02010000[pos];
    above = cell - 128;
    if (Data_02000240.y != 0 && cell->kind == 0xfd && above->kind == 0xfd) {
        Data_02000240.on_ladder = 1;
        object->y += Map_GetTerrainHeightFar(0, object->x, object->z - 0x100000) - 0x200000;
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
    *Data_03001e70 = &camera->x;
    work->camera_object = camera;
}
