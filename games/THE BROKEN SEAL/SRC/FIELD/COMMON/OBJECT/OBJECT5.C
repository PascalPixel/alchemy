#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "EVENT_RUNTIME.H"
#include "DMA.H"
#include "GAME_STATE.H"
#include "FIELD_SPRITE.H"
#include "OBJECT_RUNTIME.H"
#include "OBJECT_DISPATCH.H"
#include "FIELDOBJ.H"
#include "FIELDRUN.H"
#include "MAP_SCROLL.H"
#include "ANIMSPR.H"

extern struct ObjectRuntime Data_02001124[32];

void ObjectDispatch_SetSingleChildField26Far(struct DispatchObject *object, s32 value);
void Object_ResetMotion(struct ObjectRuntime *object);
void Object_SetMode(struct ObjectRuntime *object, s32 mode);

/* object/table/ObjectTable_ClearBattleSlots.c */
void ObjectTable_ClearBattleSlots(void)
{
    s32 *current;
    s32 zero;
    s32 count;
    zero = 0;
    count = 0x41;
    current = (s32 *)&((struct FieldStepWork *)gWork)->actors[65];
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}

/* Generic word lookup at +0x14. Field mode uses 66 actor slots; the
   0xbf limit is this accessor's contract, not a field allocation extent. */
void *ObjectTable_Get(u32 index)
{
    u8 *base = (u8 *)gWork;
    u32 offset;
    if (index > 0xbf)
        return 0;
    offset = (index * 4) + 0x14;
    return *(void **)(base + offset);
}

/*
 * Copies every live object of the table (66 on the field, 8 in battle) into
 * the 32-entry snapshot beside its index, with its sprite's two frame bytes
 * and drawing priority, and marks the unused index slots with 255.
 */
void ObjectTable_Snapshot(void)
{
    struct ObjectRuntime *copy = Data_02001124;
    u8 *frames = (u8 *)&Data_02001124[32];
    u8 *next_frames = frames + 32;
    u8 *priorities = frames + 64;
    u8 *indices = (u8 *)Data_02001124 - 32;
    u32 count = 0;
    s32 limit = 66;
    struct ObjectRuntime *object;
    s32 i;
    u32 frame;
    u32 next;
    u32 priority;

    if (((struct EventRuntime *)gEventWork)->mode_19e == 3)
        limit = 8;
    for (i = 0; i < limit; i++) {
        object = (struct ObjectRuntime *)ObjectTable_Get(i);
        if (object == NULL)
            continue;
        *indices++ = i;
        Dma_Set(object, copy, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), (volatile u32 *)0x040000d4);
        if (object->animation_kind == 1) {
            frame = ((struct AnimationObject *)object->animation)->last_no;
            next = ((struct FieldSprite *)object->animation)->flags;
            priority = ((struct FieldSprite *)object->animation)->priority;
        } else {
            frame = 0;
            next = 0;
            priority = 0;
        }
        *frames++ = frame;
        *next_frames++ = next;
        *priorities++ = priority;
        count++;
        copy++;
        if (count > 31)
            break;
    }
    for (i = count; i < 32; i++)
        *indices++ = 255;
}

/*
 * Puts back the objects ObjectTable_Snapshot saved: copies each saved
 * object over the live one at its index while keeping the live sprite,
 * restores its mode, frame and drawing priority, and re-centres the camera on
 * the selected actor.
 */
void ObjectTable_Restore(void)
{
    struct ObjectRuntime *copy = Data_02001124;
    u8 *indices = (u8 *)Data_02001124 - 32;
    u8 *frames = (u8 *)&Data_02001124[32];
    u8 *next_frames = frames + 32;
    u8 *priorities = frames + 64;
    s32 count;
    struct ObjectRuntime *object;
    struct FieldSprite *sprite;
    struct ObjectRuntime *camera;
    s32 *view;
    s32 index;
    s32 frame;
    s32 priority;
    s32 y;
    s32 *selected = &gGameState.selected_actor;
    struct EventWork **work = &gEventWork;
    struct MapScrollWork **map = (struct MapScrollWork **)gMapWork;

    count = 0;
    index = *indices++;
    while (index != 255) {
        object = ObjectTable_Get(index);
        if (object != NULL) {
            sprite = object->animation;
            Dma_Set(copy, object, 0x84000000 | (sizeof(struct ObjectRuntime) / 4),
                    (volatile u32 *)0x040000d4);
            frame = *frames;
            if (frame != 0)
                Object_SetMode(object, frame);
            ObjectDispatch_SetSingleChildField26Far((struct DispatchObject *)object, *next_frames);
            priority = *priorities;
            sprite->priority = priority;
            sprite->second_priority = priority;
            object->animation = sprite;
            if (index == *selected) {
                camera = (struct ObjectRuntime *)(*work)->view_center;
                view = (*map)->origin;
                y = object->y;
                camera->terrain_height = y;
                camera->y = y;
                view[1] = y;
                Object_ResetMotion(object);
            }
        }
        copy++;
        frames++;
        next_frames++;
        priorities++;
        count++;
        if (count > 31)
            break;
        index = *indices++;
    }
}
