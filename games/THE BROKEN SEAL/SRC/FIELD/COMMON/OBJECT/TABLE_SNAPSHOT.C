#include "TYPES.H"
#include "DMA.H"

/*
 * Copies every live object of the table (66 on the field, 8 in battle) into
 * the 32-entry snapshot beside its index, with its sprite's two frame bytes
 * and palette bank, and marks the unused index slots with 255.
 */

struct SnapshotSprite {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 bank : 2;                    /* 0x09, bits 2-3 */
    u8 unknown_09_4 : 4;
    u8 unknown_0a[0x24 - 0xa];
    u8 frame;                       /* 0x24 */
    u8 unknown_25;
    u8 frame_next;                  /* 0x26 */
};

struct SnapshotObject {
    u8 unknown_00[0x50];
    struct SnapshotSprite *sprite;  /* 0x50 */
    u8 visible;                     /* 0x54 */
    u8 unknown_55[0x70 - 0x55];
};

struct SnapshotWork {
    u8 unknown_000[0x19e];
    s16 mode;                       /* 0x19e */
};

extern struct SnapshotObject Data_02001124[32];
extern struct SnapshotWork *gEventWork;

struct SnapshotObject *ObjectTable_Get(s32 index);

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
        object = ObjectTable_Get(i);
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
