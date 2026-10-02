/* EXACT (score 0, 2026-10-02) but not adopted: the last loop is written with
 * its test at the bottom and a goto, by hand. Pascal's call.
 *
 * What the reference shows: the refresh loop is an ordinary rotated loop whose
 * index stays in fp and whose list address is built from it each time
 * (lsls, ldrsh [index, list]); loop.c never strength-reduced it, although it
 * reduced the same list walk in the loop before. Every for, while and
 * do/while spelling of it is reduced to a pointer walk (4804); compiling with
 * strength reduction off, for diagnosis only, gives the reference loop from a
 * plain for. BattleActor_RemoveFromLists (080bac6c.c) has the same unreduced
 * second scan.
 *
 * What was settled on the way and holds for any spelling:
 * - The slot's resource is read before the paired state is stored, so the
 *   pointer to the state byte cannot share r5 and is saved around the call.
 * - The paired table is one expression, not a pointer that is then advanced.
 * - The state test leaves no variable: the zero stores reuse the tested byte.
 * - The actor object needs its real s32 members: a byte member is stored with
 *   its structure's alias set, and without an s32 member the scheduler moves
 *   the scale load above the link store. */
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
    u8 padding0[8];
    s32 x;
    u8 padding0c[0x14];
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

extern volatile u8 gSchedulerStatus;
extern u8 *gMenuCtrlWork;

s32 BattlePlacement_ContainsId(s16 *list, s32 id);
void ReleaseBattleObjectRecords(s32 object_id);
void WaitFrames(s32 frames);
struct BattleObjectSlot *GetBattleObjectSlot(s32 id);
void BattleUnit_BuildStatusFlags(s32 id, struct BattleObjectSlot *slot);
struct ResourceObject *GetBattleEffectObject(s32 resource);
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
    s32 object_id;

    for (i = 0; i <= 13; i++) {
        if (!BattlePlacement_ContainsId(list, i)) {
            object_id = i + 120;
            if (i <= 7)
                object_id = i;
            ReleaseBattleObjectRecords(object_id);
        }
    }
    if (gSchedulerStatus == 0)
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
        if (object->state != 0)
            continue;
        if ((slot->resource & 0xfff) == 476 || (slot->resource & 0xfff) == 483) {
            table = gMenuCtrlWork + ((struct PairedObjectTable *)gMenuCtrlWork)->count * 4;
            objects = ((struct PairedObjectList *)table)->objects;
            resource = slot->resource;
            object->state = 2;
            object->link = objects;
            Iwram_ClearWords(objects, 16);
            res = GetBattleEffectObject(resource);
            if (res != 0) {
                res->scale = Iwram_MulQ16(res->scale, slot->scale);
                object->height = Resource_GetMetadataRecordFar(resource)[9] >> 1;
                *objects = res;
                objects = &((struct PairedObjectList *)table)->objects[1];
            }
            res->layer = 0;
            res = GetBattleEffectObject(resource + 0x2001);
            if (res != 0) {
                res->scale = Iwram_MulQ16(res->scale, slot->scale);
                *objects = res;
            }
            res->layer = 0;
        } else {
            res = GetBattleEffectObject(slot->resource);
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
                    entry->mode = 0;
                    res->layer = 0;
                }
            }
        }
        BattlePres_SetActorModeAndAction(id);
    }
    if (refresh) {
        i = 0;
        if ((resource = list[0]) != 255) {
again:
            if (list[i] != 254 && (slot = GetBattleObjectSlot(resource)) != 0 && (object = slot->object) != 0)
                BattlePres_SetActorModeAndAction(resource);
            i++;
            if (i <= 13 && (resource = list[i]) != 255)
                goto again;
        }
    }
}
