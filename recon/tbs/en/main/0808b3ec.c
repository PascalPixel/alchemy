/* Draft, not exact (2026-09-24): 596 bytes for the 608-byte owner, 288
   halfwords differ, nearly all of them branch offsets. The spawn body
   matches instruction for instruction (the object slot store needs the
   offset local, as ObjectTable_Get's does). Residuals: the reference keeps
   the table-search index unreduced (mov r0, r9; ldr r3, [r0, r2]) and
   jumps into a loop header that loads the row id twice (ldrsh for the -1
   test, ldrh carried into the body and re-extended); here the search is
   strength-reduced and the id is one ldrsh. u16 id locals and fields, and
   goto and while forms of the search, move nothing. */

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

extern u8 *Data_03001ebc;

s32 GameFlag_IsConditionActive(s32 condition);
s32 Party_RemapCharacterIdByFlags(s32 id);
struct EventObject *ObjectTable_Get(s32 index);
struct EventObject *Object_CreateFar(s32 character, s32 x, s32 y, s32 z);
void Resource_ResetEntry(s32 entry);
s32 GameFlag_TestFar(s32 flag);
void Func_08009228(struct EventObject *object, s32 value);
void Object_SetPositionAndResetMotionFar(struct EventObject *object, s32 x, s32 y, s32 z);
void Object_SetMode(struct EventObject *object, s32 mode);
u32 Random16(void);
u32 Math_ModU(u32 numerator, u32 denominator);
void ObjectMotion_SetActionCallback(struct EventObject *object, s32 action);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);

#define EVENT_STATE_MODE(state) (*(s16 *)((u8 *)(state) + 0x19e))

void Event_SpawnObjectTable(struct EventObjectEntry *entry, s32 slot)
{
    struct EventObjectEntry **tables;
    struct EventObject *object;
    struct EventObject *previous;
    struct EventSprite *sprite;
    s32 i;
    s32 index;
    u32 offset;
    s16 id;
    u16 uid;
    s32 character;
    s32 condition;
    u8 resource;

    tables = (struct EventObjectEntry **)Data_03001ebc;
    for (i = 0; i < 4; i++) {
        if (tables[i] == entry)
            break;
        if (tables[i] == 0) {
            tables[i] = entry;
            break;
        }
    }
    for (; (s16)(uid = entry->id) != -1 && slot <= 65; entry++) {
        if ((s16)uid <= 7)
            index = (s16)uid;
        else if ((s16)uid <= 0x2705)
            index = slot++;
        condition = entry->condition;
        if (!GameFlag_IsConditionActive(condition))
            continue;
        if ((u32)(condition - 48) <= 79 && EVENT_STATE_MODE(tables) != 3
            && !GameFlag_IsConditionActive(condition + 80))
            continue;
        character = Party_RemapCharacterIdByFlags(entry->id);
        object = ObjectTable_Get(index);
        if (object == 0) {
            object = Object_CreateFar(character, entry->x, entry->y, entry->z);
            if (entry->flags & 1) {
                previous = ObjectTable_Get(index - 1);
                if (previous->kind == 1 && object->kind == 1) {
                    previous->sprite->flags |= 1;
                    resource = previous->sprite->resource;
                    sprite = object->sprite;
                    sprite->flags |= 1;
                    Resource_ResetEntry(sprite->resource);
                    sprite->resource = resource;
                }
            }
            if (GameFlag_TestFar(33) && (u32)(character - 18) <= 1)
                Func_08009228(object, 226);
        } else if (!GameFlag_TestFar(0x109)) {
            Object_SetPositionAndResetMotionFar(object, entry->x, entry->y, entry->z);
        }
        if (object) {
            Object_SetMode(object, 1);
            if (object->kind == 1 && (sprite = object->sprite) != 0)
                sprite->phase = Math_ModU(Random16(), 30);
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
            if (EVENT_STATE_MODE(tables) == 3) {
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
        *(struct EventObject **)((u8 *)tables + offset) = object;
    }
}
