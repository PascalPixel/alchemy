/* DRAFT: 604 of 612 bytes. The first loop and the paired-object branch
 * match in shape; the ROM keeps the list index in fp across all three loops
 * and spills the current id (sp+16) and the mode byte (sp+12), and keeps
 * &object->state live across the create call in branch B (sp+4); this
 * compile gives the id fp and spills the index instead. */
#include "TYPES.H"
#include "IWRAM_CALL.H"

/* battle/actor/spawn_objects_for_list.c */
struct SpriteEntry {
    u8 padding0[5];
    u8 palette;
    u8 mode;
};

struct ResourceObject {
    u8 padding0[0x18];
    s32 scale;
    u8 padding1c[4];
    u8 kind;
    u8 padding21[5];
    u8 layer;
    u8 padding27;
    struct SpriteEntry *sprite;
};

struct ActorObject {
    u8 padding0[0x20];
    u16 height;
    u8 padding22[0x50 - 0x22];
    void *link;
    u8 state;
};

struct BattleObjectSlot {
    struct ActorObject *object;
    u16 resource;
    u16 overlay;
    u16 animation;
    u16 effect;
    u8 padding0c[8];
    s32 palette;
    s32 scale;
    u8 padding1c[4];
    struct SpriteEntry *animation_entry;
    struct SpriteEntry *effect_entry;
};

struct PairedObjectList {
    u8 padding0[8];
    struct ResourceObject *objects[1];
};

struct PairedObjectTable {
    u8 padding0[0x18];
    s32 count;
};

typedef s32 (*ClearFn)(void *dst, s32 size);

s32 BattlePlacement_ContainsId(s16 *list, s32 id);
void ReleaseBattleObjectRecords(s32 object_id);
void WaitFrames(s32 frames);
struct BattleObjectSlot *GetBattleObjectSlot(s32 id);
void BattleUnit_BuildStatusFlags(s32 id, struct BattleObjectSlot *slot);
struct ResourceObject *ResourceObject_CreateFar(s32 resource);
u8 *Resource_GetMetadataRecordFar(s32 resource);
struct SpriteEntry *ResourceMetadata_RegisterFar(struct ResourceObject *object, s32 resource);
void Animation_SetWorkEntryFar(struct SpriteEntry *entry, s32 index);
void BattlePres_SetActorModeAndAction(s32 id);

void BattleActor_SpawnObjectsForList(s16 *list, s32 refresh)
{
    s32 i;
    s32 id;
    struct BattleObjectSlot *slot;
    struct ActorObject *object;
    struct ResourceObject *res;
    struct SpriteEntry *entry;
    struct ResourceObject **objects;
    u8 *table;
    s32 resource;
    u8 mode;
    s32 object_id;

    for (i = 0; i <= 13; i++) {
        if (!BattlePlacement_ContainsId(list, i)) {
            object_id = i + 120;
            if (i <= 7)
                object_id = i;
            ReleaseBattleObjectRecords(object_id);
        }
    }
    if (*(u8 *)0x03001a10 == 0)
        WaitFrames(1);
    for (i = 0; i <= 13 && (id = list[i]) != 255; i++) {
        if (id == 254)
            continue;
        slot = GetBattleObjectSlot(id);
        if (slot == 0)
            continue;
        BattleUnit_BuildStatusFlags(id, slot);
        object = slot->object;
        if (object == 0)
            continue;
        mode = object->state;
        if (mode != 0)
            continue;
        if ((slot->resource & 0xfff) == 476 || (slot->resource & 0xfff) == 483) {
            table = *(u8 **)0x03001e68;
            table += ((struct PairedObjectTable *)table)->count * 4;
            objects = ((struct PairedObjectList *)table)->objects;
            object->state = 2;
            resource = slot->resource;
            object->link = objects;
            ((ClearFn)0x03000164)(objects, 16);
            res = ResourceObject_CreateFar(resource);
            if (res != 0) {
                res->scale = Iwram_MulQ16(res->scale, slot->scale);
                object->height = Resource_GetMetadataRecordFar(resource)[9] >> 1;
                *objects = res;
                objects = &((struct PairedObjectList *)table)->objects[1];
            }
            res->layer = mode;
            res = ResourceObject_CreateFar(resource + 0x2001);
            if (res != 0) {
                res->scale = Iwram_MulQ16(res->scale, slot->scale);
                *objects = res;
            }
            res->layer = mode;
        } else {
            res = ResourceObject_CreateFar(slot->resource);
            if (res != 0) {
                object->state = 1;
                object->link = res;
                res->scale = Iwram_MulQ16(res->scale, slot->scale);
                entry = res->sprite;
                entry->mode = 1;
                entry->palette = slot->palette;
                if ((resource = slot->overlay) != 0) {
                    entry = ResourceMetadata_RegisterFar(res, resource);
                    entry->mode = 1;
                }
                if ((resource = slot->animation) != 0) {
                    entry = ResourceMetadata_RegisterFar(res, resource);
                    slot->animation_entry = entry;
                    Animation_SetWorkEntryFar(entry, 0);
                    entry->mode = 3;
                }
                if ((resource = slot->effect) != 0) {
                    if (res->kind == 32)
                        resource = 0x1ff;
                    entry = ResourceMetadata_RegisterFar(res, resource);
                    slot->effect_entry = entry;
                    entry->mode = mode;
                    res->layer = mode;
                }
            }
        }
        BattlePres_SetActorModeAndAction(id);
    }
    if (refresh) {
        for (i = 0; i <= 13 && (id = list[i]) != 255; i++) {
            if (list[i] != 254 && (slot = GetBattleObjectSlot(id)) != 0 && slot->object != 0)
                BattlePres_SetActorModeAndAction(id);
        }
    }
}
