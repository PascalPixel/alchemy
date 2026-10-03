#include "ANIMSPR.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "MAP_SCROLL.H"

/* One entry of a scene's region table, which ends at id -1. */
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

extern struct EventWork *gEventWork;
void Object_Destroy(struct FieldActor *object);




void Func_080090d0(struct FieldActor *object);
s32 ObjectTable_FindLastActiveId(void);
void Event_SpawnObjectTable(s32 event_id, s32 last_id);

/* Give every entry without a flag of its own one of two fixed flags: 0x164,
   kept clear, when its position lies inside the camera's bounds, and 0x165,
   kept set, when it lies outside. */
void Scene_AssignViewFlags(struct SceneRegionEntry *entry)
{
    struct MapScrollWork *view = gMapWork[0];

    GameFlag_ClearBitFar(0x164);
    GameFlag_SetBitFar(0x165);
    while (entry->id != -1) {
        /* FAKEMATCH: reading the flag through a u16 adds one loop
           instruction, which keeps loop-invariant motion to the reference's
           single hoisted bound address. */
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

/* Destroys every object in table slots 8 to 65 that has left the area around
 * the anchor object, 160 units either side of it in x and from 200 less to
 * 100 more in z; objects still at x and z zero are kept. Each one is marked
 * removed, bit 0 of its sprite's byte 29 is cleared, and its slot emptied. */
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
                    Object_Destroy(object);
                    sprite = NULL;
                    *slot = (struct FieldActor *)sprite;
                }
            }
        }

        slot++;
    }
}

/* Hides and releases every live effect object, then clears the pending
   event and respawns its object table if one was set. */
void BattleEffect_ClearAllObjects(void)
{
    struct EventWork *runtime = gEventWork;
    s32 event_id;
    s32 i;

    for (i = 0; i < 58; i++) {
        struct FieldActor *object = runtime->placed_actors[i];

        if (object != 0) {
            object->active = 1;
            ((struct AnimationObject *)object->sprite)->display_flags &= ~1;
            Func_080090d0(object);
            runtime->placed_actors[i] = 0;
        }
    }
    event_id = *(s32 *)&runtime->unknown_000[4];
    *(s32 *)&runtime->unknown_000[4] = 0;
    *(s32 *)&runtime->unknown_000[8] = 0;
    *(s32 *)&runtime->unknown_000[12] = 0;
    if (event_id != 0)
        Event_SpawnObjectTable(event_id, ObjectTable_FindLastActiveId());
}
