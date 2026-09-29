/* Draft, not exact (2026-09-25): 280 of 280 bytes, 77 differing halfwords.
   Written from the listing after splitting it from ObjectTable_Snapshot.
   Every call, store and loop is in place; the bank byte is read once for
   both bitfields. Remaining: global allocation puts count on the stack and
   the frame cursor in sl where the ROM keeps count in r9, the bank cursor
   in fp and the two frame cursors on the stack; declaration order was
   swept (77 is the best of 120 orders).
   2026-09-29 alchemy permute (8 minutes): 1760 -> 1496, minimized to two
   natural changes: count is declared after copy, and the cursors advance
   next_frames, frames, copy, banks. The selected actor is now
   gGameState.current_owner and the callee and cells carry the build's
   names; that costs one more moved instruction (1521: 39 register-only,
   1 stack-only, 1 operand, 16 reordered, 2 inserted, 1 deleted). The
   allocation above is unchanged in kind. The index and frame tables are
   still reached relative to Data_02001124 (32 bytes before it, and past
   its 32 objects), which needs one named snapshot record before adoption. */
#include "TYPES.H"
#include "DMA.H"
#include "PARTY_STATE.H"

/*
 * Puts back the objects ObjectTable_Snapshot saved: copies each saved
 * object over the live one at its index while keeping the live sprite,
 * restores its mode, frame and palette bank, and re-centres the camera on
 * the selected actor.
 */

struct RestoreSprite {
    u8 unknown_00[9];
    u8 unknown_09_0 : 2;
    u8 bank : 2;                    /* 0x09, bits 2-3 */
    u8 unknown_09_4 : 4;
    u8 unknown_0a[0x15 - 0xa];
    u8 unknown_15_0 : 2;
    u8 shadow_bank : 2;             /* 0x15, bits 2-3 */
    u8 unknown_15_4 : 4;
};

struct RestoreObject {
    u8 unknown_00[0xc];
    s32 y;                          /* 0x0c */
    u8 unknown_10[0x40];
    struct RestoreSprite *sprite;   /* 0x50 */
    u8 unknown_54[0x70 - 0x54];
};

struct RestoreCamera {
    u8 unknown_00[0xc];
    s32 y;                          /* 0x0c */
    u8 unknown_10[4];
    s32 target_y;                   /* 0x14 */
};

struct RestoreWork {
    u8 unknown_000[0x1e0];
    struct RestoreCamera *camera;   /* 0x1e0 */
};

struct RestoreView {
    u8 unknown_00[4];
    s32 y;                          /* 0x04 */
};

struct RestoreMapWork {
    struct RestoreView *view;       /* 0x00 */
};

extern struct RestoreObject Data_02001124[32];

extern struct RestoreWork *gEventWork;
extern struct RestoreMapWork *gMapWork;

struct RestoreObject *ObjectTable_Get(s32 index);
void Object_SetMode(struct RestoreObject *object, s32 mode);
void ObjectDispatch_SetSingleChildField26Far(struct RestoreObject *object, s32 value);
void Object_ResetMotion(struct RestoreObject *object);

void ObjectTable_Restore(void)
{
    struct RestoreObject *copy = Data_02001124;
    s32 count = 0;
    u8 *indices = (u8 *)Data_02001124 - 32;
    u8 *frames = (u8 *)&Data_02001124[32];
    u8 *next_frames = frames + 32;
    u8 *banks = frames + 64;
    struct RestoreObject *object;
    struct RestoreSprite *sprite;
    struct RestoreCamera *camera;
    struct RestoreView *view;
    u32 index;
    s32 y;
    u32 bank;

    index = *indices++;
    while (index != 255) {
        object = ObjectTable_Get(index);
        if (object != NULL) {
            sprite = object->sprite;
            Dma_Set(copy, object, 0x84000000 | (sizeof(struct RestoreObject) / 4), (volatile u32 *)0x040000d4);
            if (*frames != 0)
                Object_SetMode(object, *frames);
            ObjectDispatch_SetSingleChildField26Far(object, *next_frames);
            bank = *banks;
            sprite->bank = bank;
            sprite->shadow_bank = bank;
            object->sprite = sprite;
            if (index == gGameState.current_owner) {
                camera = gEventWork->camera;
                view = gMapWork->view;
                y = object->y;
                camera->target_y = y;
                camera->y = y;
                view->y = y;
                Object_ResetMotion(object);
            }
        }
        next_frames++;
        frames++;
        copy++;
        banks++;
        if (++count > 31)
            break;
        index = *indices++;
    }
}
