#include "TYPES.H"
#include "LAYOUT_GUARD.H"
#include "OBJECT_FACTORY.H"
#include "GLOBAL_CELLS.H"

struct Object_080a9cbc {
    u8 padding[6];
    s16 value1;
    s16 value2;
};

struct State_080a9cbc {
    u8 padding[72];
    struct Object_080a9cbc *objects[32];
};

LAYOUT_OFFSET_GUARD(
    Object_080a9cbc_value1_offset, struct Object_080a9cbc, value1, 6);
LAYOUT_OFFSET_GUARD(
    State_080a9cbc_objects_offset, struct State_080a9cbc, objects, 72);
LAYOUT_SIZE_GUARD(State_080a9cbc_size, struct State_080a9cbc, 200);
extern struct State_080a9cbc *gMenuWork;
void UiIcon_PrepareObject(void *obj);

extern u8 Data_03001f2c[];
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

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

struct Entry_080a9dc4 {
    u8 padding[14];
    u8 value;
};

struct State_080a9dc4 {
    u8 padding[200];
    struct Entry_080a9dc4 *entries[5];
};

extern void Resource_LoadByModeIntoSlotFar(s32, s32, s32, s32);

void Palette_CopyObjectBankToBackground14(void);
void Palette_LightenBankHighlight(s32);

void ItemMenu_PosCategory(void)
{
    struct State_080a9cbc *state = gMenuWork;
    s32 value1 = 248;
    struct Object_080a9cbc **entry = state->objects;
    s32 value2 = 168;
    s32 remaining = 31;

    do {
        struct Object_080a9cbc *object = *entry++;

        if (object != 0) {
            object->value1 = value1;
            object->value2 = value2;
            UiIcon_PrepareObject(object);
        }
        remaining--;
    } while (remaining >= 0);
}

s32 Menu_CreateEightEntryObjects(s32 resource)
{
    void **slot;
    void *obj;
    s32 i;
    u8 *state;
    s32 param;

    state = *(u8 **)((u32)&Data_03001f2c);
    i = 0;
    param = 0xA8;
    slot = (void **)(state + 0xC8);
    do {
        obj = RenderOutput_CreateFromResourceFar(2, i, resource, 0xF8, param);
        i += 1;
        *slot = obj;
        slot += 1;
    } while (i <= 7);
    return 1;
}

/* Item menu: after resetting the category, place the flagged entries of
   the five category sprites in a column 16 pixels apart, starting at 88.

   FAKEMATCH: the x store goes through its own pointer, which is what keeps
   the flag index as a counter over the flags base (loop.c otherwise turns
   flags[index] into a walking pointer). */
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

s32 CharacterMenu_UpdateSelectionIcons(const u8 *enabled)
{
    struct State_080a9dc4 *state = gMenuWork;
    s32 index = 0;

    do {
        if (enabled[index] != 0) {
            s32 kind;
            switch (index) {
            case 0: kind = 16; break;
            case 1: kind = 1; break;
            case 2: kind = 2; break;
            case 3: kind = 15; break;
            case 4: kind = 7; break;
            default: kind = 0; break;
            }
            Resource_LoadByModeIntoSlotFar(8, kind, state->entries[index]->value, 0);
        }
        index++;
    } while (index <= 4);
    return 1;
}

void Item_PrepareUsePalette(void)
{
    Palette_CopyObjectBankToBackground14();
    Palette_LightenBankHighlight(13);
}

void Item_UseNoOpCallback(void)
{
}
