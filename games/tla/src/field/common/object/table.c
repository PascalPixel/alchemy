#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "ram_buffer.h"
#include "eventwrk.h"
#include "object_runtime.h"
#include "field_sprite.h"
#include "dma.h"
#include "io_reg.h"


void ObjectTable_Clear(void)
{
    struct EventWork *work = Ram_HeapSlots->event_work;
    s32 i;

    for (i = 0; i < 80; i++)
        work->objects[i] = NULL;
}

struct ObjectRuntime *ObjectTable_Get(u32 object_id)
{
    struct EventWork *work = Ram_HeapSlots->event_work;

    if (object_id > 0xbf)
        return NULL;
    return work->objects[object_id];
}

void ObjectTable_Snapshot(void)
{
    struct ObjectRuntime *copy = gSnapshotObjects;
    u8 *indices = (u8 *)gSnapshotObjects - 32;
    u8 *frames = (u8 *)&gSnapshotObjects[32];
    u8 *next_frames = frames + 32;
    u8 *priorities = frames + 64;
    u32 count = 0;
    struct ObjectRuntime *object;
    struct FieldSprite *sprite;
    s32 i;
    u32 frame;
    u32 next;
    u32 priority;
    s32 limit = 80;

    for (i = 0; i < limit; i++) {
        object = ObjectTable_Get(i);
        if (object == NULL)
            continue;
        *indices++ = i;
        Dma_Set(object, copy, 0x84000000 | (sizeof(struct ObjectRuntime) / 4), REG_DMA3);
        if (object->animation_kind == 1) {
            sprite = object->animation;
            frame = sprite->frame;
            next = sprite->next_frame;
            priority = sprite->priority;
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
