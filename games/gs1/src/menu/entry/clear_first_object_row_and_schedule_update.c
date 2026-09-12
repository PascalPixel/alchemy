#include "types.h"
#include "scene.h"
#include "text_render_runtime.h"

/* menu/entry/clear_first_object_row_and_schedule_update.c */
/* menu/entry/clear_first_object_row_and_schedule_update.c */
extern struct PlacementState *gIw;
void ScheduleCallback(s32);

void Menu_ClearFirstObjectRowAndScheduleUpdate(void)
{
    u8 *base = gIw;
    s32 offset = 138;
    s32 zero;
    s32 count;

    ScheduleCallback((s32)FunctionHead_0801ff58);
    zero = 0;
    offset *= 2;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            Menu_Do(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}

/* menu/entry/update_first_object_row_positions.c */

void Menu_UpdateFirstObjectRowPositions(void)
{
    u8 *base = gIw;
    s16 *offsets = (s16 *)(base + 0x134);
    s32 *entries = (s32 *)(base + 0x114);
    s32 source[2];
    s32 request[4];
    s32 index = 0;
    s32 handle;

    while (1) {
        handle = entries[index];
        if (handle != 0) {
            source[0] = entries[index + 0x10];
            source[1] = entries[index + 0x10];
            request[0] = offsets[index] << 16;
            request[1] = 0x01F40000;
            request[2] = (offsets[index + 8] << 16) + 0x01F40000;
            request[3] = 0;
            Menu_SetMode(handle, request, source, 0x4000);
        }
        index += 1;
        if (index > 3) {
            break;
        }
    }
}

/* menu/entry/spawn_four_objects_at_origin.c */
struct PlacementOrigin {
    u8 pad_00[0x0c];
    u16 x;
    u16 y;
};

struct PlacementState {
    u8 pad_000[0x224];
    void *objects[4];
    s16 x[4];
    s16 y[4];
};

struct RuntimeObject {
    u8 pad_00[9];
    s8 flags;
    u8 pad_0a[0x1c];
    s8 field_26;
};

extern const s32 gRom[4];

struct RuntimeObject *FunctionHead_08009030(s32);
void Object_InitializeMode(struct RuntimeObject *, s32);
void ScheduleCallbackAfterFrames(s32, s32);

void Menu_SpawnFourObjectsAtOrigin(struct PlacementOrigin *origin, s32 x, s32 y)
{
    struct PlacementState *state = gIw;
    s32 i;

    if (origin != 0) {
        for (i = 0; i < 4; i++) {
            struct RuntimeObject *object = FunctionHead_08009030(gRom[i]);

            if (object != 0) {
                Object_InitializeMode(object, 2);
                object->field_26 = 0;
                object->flags = (u8)(object->flags & ~0x0c);
            }

            state->objects[i] = object;
            state->x[i] = (origin->x + x + i * 3) * 8 + 0x10;
            state->y[i] = (origin->y + y) * 8 + 0x10;
        }

        ScheduleCallbackAfterFrames((s32)Menu_UpdateSecondObjectRowPositions, 200 << 4);
    }
}

/* menu/entry/clear_second_object_row_and_schedule_update.c */

void Menu_ClearSecondObjectRowAndScheduleUpdate(void)
{
    u8 *base = gIw;
    s32 offset = 137;
    s32 zero;
    s32 count;

    ScheduleCallback((s32)FunctionHead_080200cc);
    zero = 0;
    offset *= 4;
    count = 3;
    do {
        void *entry = *(void **)(offset + (unsigned int)base);

        if (entry != 0) {
            Menu_Do(entry);
            *(s32 *)(offset + (unsigned int)base) = zero;
        }
        count--;
        offset += 4;
    } while (count >= 0);
}

/* menu/entry/update_second_object_row_positions.c */
struct Root_080200cc {
    u8 pad_000[548];
    void *objects[4];
    s16 coords[8];
};


void Menu_UpdateSecondObjectRowPositions(void)
{
    struct Root_080200cc *root = gIw;
    s32 scale[2];
    s32 position[4];
    s32 index;

    index = 0;
    do {
        void *object = root->objects[index];

        if (object != 0) {
            s32 unit = 0x10000;
            s32 bias = 0x1f40000;

            scale[0] = unit;
            scale[1] = unit;
            position[0] = root->coords[index] << 16;
            position[1] = bias;
            position[2] = (root->coords[index + 4] << 16) + bias;
            position[3] = 0;
            Menu_SetMode(object, position, scale, 0x4000);
        }
        index++;
    } while (index <= 3);
}

/* ui/text/draw/draw_four_numbers_in_row.c */
void UiText_DrawFourNumbersInRow(struct TextRenderWork *work, s32 base)
{
    s32 pos;
    s32 cnt;
    volatile u8 *src;
    s32 size;

    if (work != 0) {
        pos = 0;
        size = 0x10;
        cnt = 3;
        src = (volatile u8 *) (base + 0x28);
        do {
            UiText_DrawNumberAtOffset((s8)*src, 2, work, pos, size);
            src += 1;
            cnt -= 1;
            pos = pos + 0x18;
        } while (cnt >= 0);
    }
}
