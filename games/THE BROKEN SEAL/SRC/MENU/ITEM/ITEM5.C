/* Item menu: after resetting the category, place the flagged entries of
   the five category sprites in a column 16 pixels apart, starting at 88.

   FAKEMATCH: the x store goes through its own pointer, which is what keeps
   the flag index as a counter over the flags base (loop.c otherwise turns
   flags[index] into a walking pointer). */
#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "LAYOUT_GUARD.H"

extern u8 Data_03001f2c[];
void ItemMenu_ResetCategory(void);
void UiIcon_PrepareObject(void *);

struct Object_080a9d84 {
    u8 padding[6];
    s16 value1;
    s16 value2;
    u8 padding2[5];
    u8 flag;
};

struct State_080a9d84 {
    u8 padding[200];
    struct Object_080a9d84 *objects[32];
};

LAYOUT_OFFSET_GUARD(
    Object_080a9d84_flag_offset, struct Object_080a9d84, flag, 15);
LAYOUT_OFFSET_GUARD(
    State_080a9d84_objects_offset, struct State_080a9d84, objects, 200);
LAYOUT_SIZE_GUARD(State_080a9d84_size, struct State_080a9d84, 328);
extern struct State_080a9d84 *gMenuWork;
void UiIcon_PrepareObject(void *obj);

void ItemMenu_ApplyFlags(const u8 *flags)
{
    u8 *base;
    void **slot;
    void *entry;
    s32 index;
    s32 value;
    u16 kind;

    base = *(u8 **)((u32)&Data_03001f2c);
    ItemMenu_ResetCategory();
    index = 0;
    slot = (void **)(base + 200);
    value = 88;
    do {
        entry = *slot++;
        if (entry != 0 && flags[index] != 0) {
            kind = 8;
            *(u16 *)((u8 *)entry + 6) = kind;
            {
                u16 *x = (u16 *)((u8 *)entry + 8);

                *x = value;
            }
            *(u8 *)((u8 *)entry + 15) = 240;
            UiIcon_PrepareObject(entry);
            value += 16;
        }
        index++;
    } while (index <= 4);
}

void ItemMenu_ResetCategory(void)
{
    struct State_080a9d84 *state = gMenuWork;
    s32 index;

    for (index = 0; index < 5; index++) {
        struct Object_080a9d84 *object = state->objects[index];

        if (object != 0) {
            object->value1 = 248;
            object->value2 = 168;
            object->flag = 240;
            UiIcon_PrepareObject(object);
        }
    }
}
