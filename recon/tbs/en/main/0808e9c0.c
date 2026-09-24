/* Draft, not exact (2026-09-25): 596 of 596 bytes, 29 differing halfwords.
   Written from the listing. What made it line up: the col/row/id list
   walked as a while loop that reads the next pair at its end, the slot
   column and row stored inside each kind branch so their tails cross-jump,
   and slot++ before count++. Remaining: the list cursor and slot pointer
   take r8/sl where the ROM has sl/r8 (global priorities 35 refs over 170
   insns against 32 over 165; declaration and assignment order moved
   nothing), and the kind 3 mode mask loads after the mode word. */
#include "TYPES.H"
#include "DMA.H"

/*
 * Places the battle markers listed by the current map: each (column, row,
 * id) triplet names a map event; the first matching event of kind 19, or
 * of kind 3 with a clear flag, becomes an object at the centre of that
 * tile and is recorded in the event work's ten marker slots. The list ends
 * with column and row 255.
 */

struct MarkerMap {
    u8 unknown_00[0x10];
    u8 *markers;                    /* 0x10 */
};

struct MarkerEvent {
    u32 kind;                       /* 0x00; low nine bits, -1 ends the table */
    s16 id;                         /* 0x04 */
    s16 flag;                       /* 0x06 */
    u32 mode;                       /* 0x08; top twelve bits */
};

struct MarkerObject {
    u8 unknown_00[8];
    s32 x;                          /* 0x08 */
    u8 unknown_0c[4];
    s32 z;                          /* 0x10 */
    u8 unknown_14[0x23 - 0x14];
    u8 unknown_23;                  /* 0x23 */
    u8 unknown_24[0x59 - 0x24];
    u8 unknown_59;                  /* 0x59 */
    u8 unknown_5a[0x64 - 0x5a];
    s16 home_x;                     /* 0x64 */
    s16 home_z;                     /* 0x66 */
};

struct MarkerSlot {
    struct MarkerObject *object;    /* 0x00 */
    u8 id;                          /* 0x04 */
    u8 unknown_05;
    u8 column;                      /* 0x06 */
    u8 row;                         /* 0x07 */
};

struct MarkerWork {
    u8 unknown_000[0x11c];
    struct MarkerSlot slots[10];    /* 0x11c */
};

struct MarkerGlobals {
    struct MarkerMap *map;          /* 0x03001e70 */
    u8 unknown_04[0x48];
    struct MarkerWork *work;        /* 0x03001ebc */
};

struct MarkerServices {
    u8 unknown_00[0x24];
    struct MarkerEvent *(*events)(void);
};

extern struct MarkerGlobals Data_03001e70;
extern struct MarkerServices Data_02008000;

struct MarkerObject *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void ObjectMotion_SnapToTerrain(struct MarkerObject *object);
void ObjectDispatch_SetSingleChildField26Far(struct MarkerObject *object, s32 value);
s32 GameFlag_TestFar(s32 flag);
void Func_080090d0(struct MarkerObject *object);
void Object_SetMode(struct MarkerObject *object, s32 mode);
void Func_08009140(struct MarkerObject *object);

void Battle_PlaceMapMarkers(void)
{
    u32 id;
    struct MarkerMap *map = Data_03001e70.map;
    struct MarkerWork *work = Data_03001e70.work;
    s32 count = 0;
    u8 *list = map->markers;
    struct MarkerSlot *slot = work->slots;
    volatile u32 fill;
    struct MarkerEvent *event;
    struct MarkerObject *object;
    u32 column;
    u32 row;

    fill = 0;
    Dma_Set((void *)&fill, slot, 0x85000000 | (sizeof(struct MarkerSlot) * 10 / 4), (volatile u32 *)0x040000d4);
    if (list == NULL)
        return;

    column = *list++;
    row = *list++;
    while (column != 255 || row != 255) {
        id = *list++;
        if (id < 100 || id > 239)
            goto next;
        event = Data_02008000.events();
        for (; event->kind != -1; event++) {
            if (event->id != id)
                continue;
            if ((event->kind & 0x1ff) == 19) {
                object = Object_CreateFar(20, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
                if (object == NULL)
                    continue;
                ObjectMotion_SnapToTerrain(object);
                ObjectDispatch_SetSingleChildField26Far(object, 0);
                if (GameFlag_TestFar(event->flag)) {
                    if ((event->mode & 0xfff00000) == 0x500000) {
                        Func_080090d0(object);
                        continue;
                    }
                    Object_SetMode(object, 2);
                }
                Func_08009140(object);
                object->home_x = object->x / 0x10000;
                object->home_z = object->z / 0x10000;
                object->unknown_23 = 1;
                object->unknown_59 = 1;
                slot->id = event->id;
                slot->object = object;
                slot->column = object->x / 0x100000;
                slot->row = object->z / 0x100000;
            } else if ((event->kind & 0x1ff) == 3) {
                if ((event->mode & 0xfff00000) != 0x300000)
                    continue;
                if (GameFlag_TestFar(event->flag))
                    continue;
                object = Object_CreateFar(28, (column << 20) + 0x80000, 0, (row << 20) + 0x80000);
                if (object == NULL)
                    continue;
                ObjectMotion_SnapToTerrain(object);
                ObjectDispatch_SetSingleChildField26Far(object, 0);
                Func_08009140(object);
                Object_SetMode(object, 1);
                object->home_x = object->x / 0x10000;
                object->home_z = object->z / 0x10000;
                object->unknown_59 = 1;
                object->unknown_23 = 1;
                slot->object = object;
                slot->id = event->id;
                slot->column = object->x / 0x100000;
                slot->row = object->z / 0x100000;
            } else {
                continue;
            }
            slot++;
            count++;
            if (count > 9)
                return;
            break;
        }
    next:
        column = *list++;
        row = *list++;
    }
}
