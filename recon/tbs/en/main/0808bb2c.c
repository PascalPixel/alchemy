/* Draft of ObjectTable_Restore, the reverse of ObjectTable_Snapshot in
   games/THE BROKEN SEAL/SRC/FIELD/COMMON/OBJECT/OBJECT5.C; it belongs after
   it in that file.
   Remaining difference (54 instructions, all register choice and order):
   the listing reaches gEventWork, gMapWork and the selected actor through an
   address register of their own (ldr r2, =gEventWork; ldr r3, [r2]) and
   takes its reload registers in turn from r1, r2 and r3. That is what agscc
   gives when its loop pass moves the three address loads out of the loop:
   compiling this draft with the diagnostic -fmove-all-movables leaves only
   the order of a few moves. Here each address is loaded one insn before its
   use, so the pass counts its life as 1 and leaves it (10 * 1 * 1 against 60
   insns); the listing's source must put about six insns or notes between an
   address and its use, as an inlined helper taking the address would. An
   accessor taking the cell's address reaches a life of 2 only.
   What already agrees: count, the index cursor and the bank cursor in r9,
   sl and fp with the two frame cursors on the stack, the frame byte read
   once into r1, and the loop's shape. */
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
    u8 *indices = (u8 *)Data_02001124 - 32;
    u8 *frames = (u8 *)&Data_02001124[32];
    u8 *next_frames = frames + 32;
    u8 *banks = frames + 64;
    s32 count;
    struct RestoreObject *object;
    struct RestoreSprite *sprite;
    struct RestoreCamera *camera;
    struct RestoreView *view;
    s32 index;
    s32 frame;
    s32 bank;
    s32 y;

    index = *indices++;
    count = 0;
    while (index != 255) {
        object = ObjectTable_Get(index);
        if (object != NULL) {
            sprite = object->sprite;
            Dma_Set(copy, object, 0x84000000 | (sizeof(struct RestoreObject) / 4), (volatile u32 *)0x040000d4);
            frame = *frames;
            if (frame != 0)
                Object_SetMode(object, frame);
            ObjectDispatch_SetSingleChildField26Far(object, *next_frames);
            bank = *banks;
            sprite->bank = bank;
            sprite->shadow_bank = bank;
            object->sprite = sprite;
            if (index == gGameState.selected_actor) {
                camera = gEventWork->camera;
                view = gMapWork->view;
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
