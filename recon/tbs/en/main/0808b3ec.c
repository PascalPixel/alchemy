/* Draft, not exact: 616 bytes for the 608-byte listing, 18 instructions
   differ, all in the row loop's test. The listing's bottom test loads the
   row id once (ldrh), copies it to the register the body re-extends, and
   compares the extension, so the entry copy of the test branches into it;
   here both copies load the id twice (ldrsh and ldrh) and do not merge.
   The compiler's around-loop CSE replaces the body's first id read with the
   test's own register; in the listing global CSE did that instead. A
   do-while with the test repeated before it gives the listing's bottom test
   but moves slot to the stack. Everything else matches. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

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
    u8 unknown_18[0x0b];
    u8 visible;                 /* 0x23 */
    u8 unknown_24[0x2c];
    struct EventSprite *sprite; /* 0x50 */
    u8 kind;                    /* 0x54 */
    u8 mode;                    /* 0x55 */
    u8 unknown_56[3];
    u8 active;                  /* 0x59 */
    u8 unknown_5a[0x0a];
    s16 cell_x;                 /* 0x64 */
    s16 cell_z;                 /* 0x66 */
};

struct ObjectWork {
    struct EventObjectEntry *tables[4];     /* 0x000 */
    u8 unknown_010[4];
    struct EventObject *objects[0x62];      /* 0x014 */
    u8 unknown_19c[2];
    s16 scene_mode;                         /* 0x19e */
};

extern struct ObjectWork *gEventWork;

s32 GameFlag_IsConditionActive(s32 condition);
s32 Party_RemapCharacterIdByFlags(s32 id);
struct EventObject *ObjectTable_Get(s32 index);
struct EventObject *Object_CreateFar(s32 character, s32 x, s32 y, s32 z);
void Resource_ResetEntry(s32 entry);
s32 GameFlag_TestFar(s32 flag);
void ObjectDispatch_RegisterChildMetadataFar(struct EventObject *object, s32 value);
void Object_SetPositionAndResetMotionFar(struct EventObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct EventObject *object, s32 mode);
u32 Random16(void);
u32 __umodsi3(u32 numerator, u32 denominator);
void ObjectMotion_SetActionCallback(struct EventObject *object, s32 action);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

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

    work = gEventWork;
    for (i = 0; i < 4; i++) {
        if (work->tables[i] == entry)
            break;
        if (work->tables[i] == 0) {
            work->tables[i] = entry;
            break;
        }
    }
    for (; entry->id != -1 && slot <= 65; entry++) {
        if (entry->id <= 7)
            index = entry->id;
        else if (entry->id <= 0x2705)
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
                    sprite = previous->sprite;
                    sprite->flags |= 1;
                    resource = sprite->resource;
                    sprite = object->sprite;
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
            if (object->kind == 1 && (sprite = object->sprite) != 0)
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
