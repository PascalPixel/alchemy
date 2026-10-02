#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "EVENT_RUNTIME.H"
#include "DMA.H"
#include "GAME_STATE.H"

struct SnapshotSprite {
    u32 unknown_00[2];
    u8 unknown_08;
    u32 unknown_09_0 : 2;
    u32 bank : 2;                   /* 0x09, bits 2-3 */
    u32 unknown_09_4 : 4;
    u8 unknown_0a[0x15 - 0xa];
    u32 unknown_15_0 : 2;
    u32 shadow_bank : 2;            /* 0x15, bits 2-3 */
    u32 unknown_15_4 : 4;
    u8 unknown_16[0x24 - 0x16];
    u8 frame;                       /* 0x24 */
    u8 unknown_25;
    u8 frame_next;                  /* 0x26 */
};

struct SnapshotObject {
    u8 unknown_00[0xc];
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x40];
    struct SnapshotSprite *sprite;  /* 0x50 */
    u8 visible;                     /* 0x54 */
    u8 unknown_55[0x70 - 0x55];
};

struct SnapshotCamera {
    u8 unknown_00[0xc];
    s32 y;                          /* 0x0c */
    u8 unknown_10[4];
    s32 target_y;                   /* 0x14 */
};

struct SnapshotWork {
    u8 unknown_000[0x19e];
    s16 mode;                       /* 0x19e */
    u8 unknown_1a0[0x40];
    struct SnapshotCamera *camera;  /* 0x1e0 */
};

struct SnapshotView {
    u8 unknown_00[4];
    s32 y;                          /* 0x04 */
};

struct SnapshotMapWork {
    struct SnapshotView *view;      /* 0x00 */
};

extern struct SnapshotObject Data_02001124[32];
extern struct SnapshotWork *gEventWork;

void Object_SetMode(struct SnapshotObject *object, s32 mode);
void ObjectDispatch_SetSingleChildField26Far(struct SnapshotObject *object, s32 value);
void Object_ResetMotion(struct SnapshotObject *object);

/* object/table/ObjectTable_ClearBattleSlots.c */
void ObjectTable_ClearBattleSlots(void)
{
    s32 *current;
    s32 offset;
    s32 zero;
    s32 count;
    offset = 0x8C;
    zero = 0;
    count = 0x41;
    current = (s32 *)((u8 *)gWork + offset * 2);
    do {
        count--;
        *current = zero;
        current--;
    } while (count >= 0);
}

/* The object pointer in slot index of the 192-entry table at gWork + 0x14,
   or NULL for an index past the table. */
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
 * and palette bank, and marks the unused index slots with 255.
 */
void ObjectTable_Snapshot(void)
{
    struct SnapshotObject *copy = Data_02001124;
    u8 *frames = (u8 *)&Data_02001124[32];
    u8 *next_frames = frames + 32;
    u8 *banks = frames + 64;
    u8 *indices = (u8 *)Data_02001124 - 32;
    u32 count = 0;
    s32 limit = 66;
    struct SnapshotObject *object;
    s32 i;
    u32 frame;
    u32 next;
    u32 bank;

    if (gEventWork->mode == 3)
        limit = 8;
    for (i = 0; i < limit; i++) {
        object = (struct SnapshotObject *)ObjectTable_Get(i);
        if (object == NULL)
            continue;
        *indices++ = i;
        Dma_Set(object, copy, 0x84000000 | (sizeof(struct SnapshotObject) / 4), (volatile u32 *)0x040000d4);
        if (object->visible == 1) {
            frame = object->sprite->frame;
            next = object->sprite->frame_next;
            bank = object->sprite->bank;
        } else {
            frame = 0;
            next = 0;
            bank = 0;
        }
        *frames++ = frame;
        *next_frames++ = next;
        *banks++ = bank;
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
 * restores its mode, frame and palette bank, and re-centres the camera on
 * the selected actor.
 */
void ObjectTable_Restore(void)
{
    struct SnapshotObject *copy = Data_02001124;
    u8 *indices = (u8 *)Data_02001124 - 32;
    u8 *frames = (u8 *)&Data_02001124[32];
    u8 *next_frames = frames + 32;
    u8 *banks = frames + 64;
    s32 count;
    struct SnapshotObject *object;
    struct SnapshotSprite *sprite;
    struct SnapshotCamera *camera;
    struct SnapshotView *view;
    s32 index;
    s32 frame;
    s32 bank;
    s32 y;
    s32 *selected = &gGameState.selected_actor;
    struct SnapshotWork **work = &gEventWork;
    struct SnapshotMapWork **map = ((struct SnapshotMapWork * *)gMapWork);

    count = 0;
    index = *indices++;
    while (index != 255) {
        object = ObjectTable_Get(index);
        if (object != NULL) {
            sprite = object->sprite;
            Dma_Set(copy, object, 0x84000000 | (sizeof(struct SnapshotObject) / 4),
                    (volatile u32 *)0x040000d4);
            frame = *frames;
            if (frame != 0)
                Object_SetMode(object, frame);
            ObjectDispatch_SetSingleChildField26Far(object, *next_frames);
            bank = *banks;
            sprite->bank = bank;
            sprite->shadow_bank = bank;
            object->sprite = sprite;
            if (index == *selected) {
                camera = (*work)->camera;
                view = (*map)->view;
                y = object->y;
                camera->target_y = y;
                camera->y = y;
                view->y = y;
                Object_ResetMotion(object);
            }
        }
        copy++;
        frames++;
        next_frames++;
        banks++;
        count++;
        if (count > 31)
            break;
        index = *indices++;
    }
}
