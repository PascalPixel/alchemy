#include "PROTO.H"
#include "ANIMSPR.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELDOBJ.H"
#include "MAP_SCROLL.H"

struct SceneRegionEntry {
    s16 id;
    s16 flag;
    u8 unknown_04[4];
    s32 x;
    u8 unknown_0c[4];
    s32 z;
    u8 unknown_14[4];
};

void GameFlag_SetBitFar(s32 flag);
void GameFlag_ClearBitFar(s32 flag);

void ObjectDispatch_ReleaseFar(struct FieldActor *object);

struct EffectSprite {
    u8 unknown_00[29];
    u8 enabled : 1;
    u8 flags : 7;
};

void ObjectDispatch_ReleaseFar(struct FieldActor *object);

void Scene_AssignViewFlags(struct SceneRegionEntry *entry)
{
    struct MapScrollWork *view = gMapWork[0];

    GameFlag_ClearBitFar(0x164);
    GameFlag_SetBitFar(0x165);
    while (entry->id != -1) {
        /* FAKEMATCH: reading the flag through a u16 adds one loop instruction, which keeps loop-invariant motion to the reference's single hoisted bound address. */
        u16 raw = entry->flag;
        s16 flag = raw;

        if (flag == 0) {
            s32 x = entry->x;
            s32 z = entry->z;

            if (view->min_x <= x && x <= view->max_x
                && view->min_y <= z && z <= view->max_y)
                entry->flag = 0x164;
            else
                entry->flag = 0x165;
        }
        entry++;
    }
}

void BattleEffect_ClearOutOfBoundsObjects(void)
{
    struct EventWork *work = gEventWork;
    struct FieldActor *anchor;
    s32 x;
    s32 z;
    s32 left;
    s32 right;
    s32 top;
    s32 bottom;
    struct FieldActor **slot;
    s32 mask;
    s32 i;

    anchor = work->view_center;
    x = anchor->x.fixed;
    left = x - 0xa00000;
    right = x + 0xa00000;
    z = anchor->z.fixed;
    top = z - 0xc80000;
    bottom = z + 0x640000;
    slot = work->placed_actors;
    mask = ~1;

    for (i = 57; i >= 0; i--) {
        struct FieldActor *object = *slot;

        if (object != NULL) {
            s32 object_x = object->x.fixed;
            s32 object_z = object->z.fixed;

            if (object_x != 0 || object_z != 0) {
                if (object_x < left || object_x > right ||
                    object_z < top || object_z > bottom) {
                    u8 *sprite;
                    u8 *removed = &object->active;
                    u32 flags;

                    *removed = 1;
                    sprite = (u8 *)object->sprite;
                    flags = sprite[29];
                    sprite[29] = flags & mask;
                    ObjectDispatch_ReleaseFar(object);
                    sprite = NULL;
                    *slot = (struct FieldActor *)sprite;
                }
            }
        }

        slot++;
    }
}

void BattleEffect_ClearAllObjects(void)
{
    /* FAKEMATCH: the original one-bit display flag clear keeps the signed mask and saved loop registers; a full-byte owner mask changes the frame. */
    struct EventWork *runtime = gEventWork;
    struct ScenePlacement *table;
    s32 i;

    for (i = 0; i < 58; i++) {
        struct FieldActor *object = runtime->placed_actors[i];

        if (object != 0) {
            object->active = 1;
            ((struct EffectSprite *)object->sprite)->enabled = 0;
            ObjectDispatch_ReleaseFar(object);
            runtime->placed_actors[i] = 0;
        }
    }
    table = runtime->tables[1];
    runtime->tables[1] = 0;
    runtime->tables[2] = 0;
    runtime->tables[3] = 0;
    if (table != 0)
        Event_SpawnObjectTable(table, ObjectTable_FindLastActiveId());
}
