#include "layout_guard.h"
#include "types.h"

typedef struct BattleInput {
    u8 primary_id;
    u8 unknown01;
    u8 secondary_id;
    u8 padding03[0x55];
    s32 flags;
} BattleInput;

typedef struct BattleWork {
    s32 flags;
    s32 secondary_is_low_id;
    s32 primary_id;
    s32 secondary_id;
    s32 one;
    s32 count;
    s32 mode;
    s32 unknown_1c;
    u8 padding20[4];
    s16 members[8];
    u8 child_values[8][4];
} BattleWork;

typedef struct Motion {
    u8 padding00[8];
    s32 x;
    u8 padding0c[4];
    s32 z;
} Motion;

typedef struct Slot {
    Motion *object;
} Slot;

typedef struct Child {
    u8 padding00[5];
    u8 value;
} Child;

typedef struct Record {
    u8 padding00[0x27];
    u8 child_count;
    Child **children;
} Record;

typedef struct UsedItem {
    s16 actor_id;
    u8 padding02[6];
    s16 inventory_slot;
} UsedItem;

typedef struct QueuedItem {
    s16 actor_id;
    u8 padding02[6];
    s16 inventory_slot;
    s16 kind;
    u8 padding0c[4];
} QueuedItem;

typedef struct Character_080ba6ac {
    u8 padding000[0xd8];
    u16 inventory[32];
} Character_080ba6ac;

typedef struct ItemDefinition_080ba6ac {
    u8 padding00[0x0c];
    u8 type;
} ItemDefinition_080ba6ac;

typedef struct WorldState_080ba6ac {
    u8 padding000[0x2ec];
    QueuedItem queued_items[20];
    u8 padding42c[0x118];
    u8 palette[0x100];
    s32 palette_factor;
} WorldState_080ba6ac;

LAYOUT_OFFSET_GUARD(
    BattleInput080ba6ac_Flags,
    BattleInput,
    flags,
    0x58);
LAYOUT_OFFSET_GUARD(
    BattleWork080ba6ac_Members,
    BattleWork,
    members,
    0x24);
LAYOUT_OFFSET_GUARD(
    BattleWork080ba6ac_ChildValues,
    BattleWork,
    child_values,
    0x34);
LAYOUT_SIZE_GUARD(
    BattleWork080ba6ac_Size,
    BattleWork,
    0x54);
LAYOUT_OFFSET_GUARD(
    Record080ba6ac_ChildCount,
    Record,
    child_count,
    0x27);
LAYOUT_OFFSET_GUARD(
    Record080ba6ac_Children,
    Record,
    children,
    0x28);
LAYOUT_OFFSET_GUARD(
    UsedItem080ba6ac_InventorySlot,
    UsedItem,
    inventory_slot,
    8);
LAYOUT_OFFSET_GUARD(
    QueuedItem080ba6ac_InventorySlot,
    QueuedItem,
    inventory_slot,
    8);
LAYOUT_OFFSET_GUARD(
    QueuedItem080ba6ac_Kind,
    QueuedItem,
    kind,
    0x0a);
LAYOUT_SIZE_GUARD(
    QueuedItem080ba6ac_Size,
    QueuedItem,
    0x10);
LAYOUT_OFFSET_GUARD(
    Character080ba6ac_Inventory,
    Character_080ba6ac,
    inventory,
    0xd8);
LAYOUT_OFFSET_GUARD(
    ItemDefinition080ba6ac_Type,
    ItemDefinition_080ba6ac,
    type,
    0x0c);
LAYOUT_OFFSET_GUARD(
    WorldState080ba6ac_QueuedItems,
    WorldState_080ba6ac,
    queued_items,
    0x2ec);
LAYOUT_OFFSET_GUARD(
    WorldState080ba6ac_Palette,
    WorldState_080ba6ac,
    palette,
    0x544);
LAYOUT_OFFSET_GUARD(
    WorldState080ba6ac_PaletteFactor,
    WorldState_080ba6ac,
    palette_factor,
    0x644);

extern s32 *Data_03001f00;
extern WorldState_080ba6ac *Data_03001e74;

void Func_080030f8(s32);
void Func_080041d8(u32, s32);
void Func_08009080(Motion *, s32);
void Func_08009088(Motion *, s32);
Character_080ba6ac *Func_08077008(s16);
ItemDefinition_080ba6ac *Func_08077018(u16);
s32 Func_08077058(s16, s16);
void Func_08077060(s16, s16);
s32 Func_080771a0(void);
Slot *Func_080b7dd0(s32);
Record *Func_080b7f70(Motion *, s32);
void Func_080b8000(s16);
void Func_080b9d34(void *, BattleWork *);
void Func_080bb938(void);
void Func_080bbabc(s32, u16);
void Func_080be02c(void);
void Func_080c10e8(s32, s32);
void Func_080c1724(void *, u32, s32, s32);
void Func_080c9008(BattleWork *);
void Func_080c9018(BattleWork *);

/*
 * Execute a battle action, then reconcile the used inventory slot with any
 * queued actions that still refer to it.
 */
s32 Func_080ba6ac(
    BattleInput *input,
    void *unused,
    UsedItem *used_item)
{
    BattleWork work;
    s32 *global = Data_03001f00;
    Motion *primary_object;
    s32 i;

    (void)unused;
    global[0] = input->primary_id <= 4 ? 0x2000 : -0x2000;
    Func_080b9d34(input, &work);
    Func_080c10e8(0, 0);

    primary_object = Func_080b7dd0(work.primary_id)->object;
    Func_08009080(primary_object, 3);
    Func_08009088(primary_object, 0x10);
    work.secondary_is_low_id = input->secondary_id <= 7;

    for (i = 0; i < work.count; i++) {
        Record *record =
            Func_080b7f70(Func_080b7dd0(work.members[i])->object, 0);
        s32 child_count = record->child_count - 1;
        s32 child;

        for (child = 0; child < child_count; child++) {
            work.child_values[i][child] =
                record->children[child]->value;
        }
    }

    Func_080041d8(0x080bd899, 0x0c80);
    if (work.flags != 0) {
        WorldState_080ba6ac *world = Data_03001e74;

        for (i = 0; i < 20; i++) {
            s32 factor = 0x10000 - i * 0x444;

            world->palette_factor = factor;
            Func_080c1724(
                world->palette, 0x050000c0, factor, 0x80);
            Func_080030f8(1);
        }
        if ((input->flags & 0x4000) != 0)
            Func_080c9008(&work);
        else
            Func_080c9018(&work);
    } else {
        Func_080030f8(0x3c);
    }

    Func_080be02c();
    Func_08009080(primary_object, 1);
    for (i = 0; i < work.count; i++)
        Func_080b8000(work.members[i]);

    {
        Character_080ba6ac *character =
            Func_08077008(used_item->actor_id);
        s16 slot = used_item->inventory_slot;
        u16 item = character->inventory[slot];
        u8 item_type = Func_08077018(item)->type;

        if (item_type == 1) {
            if (Func_08077058(used_item->actor_id, slot) == 2) {
                QueuedItem *queue = Data_03001e74->queued_items;

                for (i = 0; i < 20; i++) {
                    if (queue[i].kind == 2 &&
                        queue[i].actor_id == used_item->actor_id) {
                        if (queue[i].inventory_slot == slot)
                            queue[i].inventory_slot = -1;
                        else if (queue[i].inventory_slot > slot)
                            queue[i].inventory_slot--;
                    }
                }
            }
        } else if (item_type == 2) {
            if ((Func_080771a0() & 7) == 0) {
                Func_080bbabc(2, item);
                Func_080bbabc(4, 0x81c);
                Func_08077060(used_item->actor_id, slot);
                Func_080bb938();
            }
        } else if (item_type == 4) {
            if ((item & 0x1ff) == 0xb8)
                item = 0xb9;
            character->inventory[slot] = item;
        }
    }

    return 0;
}
