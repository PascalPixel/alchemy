#include "global_cells.h"
#include "types.h"
#include "event_runtime.h"
#include "dma.h"
#include "game_state.h"
#include "field_sprite.h"
#include "object_runtime.h"
#include "object_dispatch.h"
#include "fieldobj.h"
#include "fieldrun.h"
#include "map_scroll.h"
#include "animspr.h"
#include "field_event.h"
#include "io_reg.h"
void ObjectDispatch_ApplyArgumentToChildrenFar(struct FieldActor *object, s32 animation);

extern struct ObjectRuntime gObjectSnapshots[32];

void ObjectDispatch_SetSingleChildFlagsFar(struct DispatchObject *object, s32 value);
void ObjectDispatch_InitFromTable6Far(struct ObjectRuntime *object);

void ObjectTable_ClearBattleSlots(void)
{
    s32 *current;
    s32 zero;
    s32 count;
    zero = 0;
    count = 0x41;
    current = (s32 *)&((struct FieldStepWork *)gEventWork)->actors[65];
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}

void *ObjectTable_Get(u32 index)
{
    u8 *base = (u8 *)gEventWork;
    u32 offset;
    if (index > 0xbf)
        return 0;
    offset = (index * 4) + 0x14;
    return *(void **)(base + offset);
}

void ObjectTable_Snapshot(void)
{
    struct ObjectRuntime *copy = gObjectSnapshots;
    u8 *frames = (u8 *)&gObjectSnapshots[32];
    u8 *next_frames = frames + 32;
    u8 *priorities = frames + 64;
    u8 *indices = (u8 *)gObjectSnapshots - 32;
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
        Dma_Set(object, copy, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
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

void ObjectTable_Restore(void)
{
    struct ObjectRuntime *copy = gObjectSnapshots;
    u8 *indices = (u8 *)gObjectSnapshots - 32;
    u8 *frames = (u8 *)&gObjectSnapshots[32];
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
            Dma_Set(copy, object, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
            frame = *frames;
            if (frame != 0)
                ObjectDispatch_ApplyArgumentToChildrenFar(object, frame);
            ObjectDispatch_SetSingleChildFlagsFar((struct DispatchObject *)object, *next_frames);
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
                ObjectDispatch_InitFromTable6Far(object);
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
