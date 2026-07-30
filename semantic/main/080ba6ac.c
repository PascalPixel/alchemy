#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

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
    s16 members[24];
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
    u8 padding02[4];
    s16 kind;
    s16 inventory_slot;
    u8 padding0a[6];
} QueuedItem;

void Func_080030f8(s32);
void Func_080041d8(u32, s32);
void Func_08009080(Motion *, s32);
void Func_08009088(Motion *, s32);
u8 *Func_08077008(s16);
u8 *Func_08077018(u16);
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
    s32 *global = *(s32 **)0x03001f00;
    Motion *primary_object;
    u8 *state;
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
            FIELD(&work, u8, 0x34 + i * 4 + child) =
                record->children[child]->value;
        }
    }

    Func_080041d8(0x080bd899, 0x0c80);
    if (work.flags != 0) {
        state = *(u8 **)0x03001e74;
        for (i = 0; i < 20; i++) {
            s32 factor = 0x10000 - i * 0x444;
            FIELD(state, s32, 0x644) = factor;
            Func_080c1724(state + 0x544, 0x050000c0, factor, 0x80);
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
        u8 *character = Func_08077008(used_item->actor_id);
        s16 slot = used_item->inventory_slot;
        u16 item = FIELD(character, u16, 0xd8 + slot * 2);
        u8 item_type = FIELD(Func_08077018(item), u8, 0x0c);

        if (item_type == 1) {
            if (Func_08077058(used_item->actor_id, slot) == 2) {
                QueuedItem *queue =
                    (QueuedItem *)(*(u8 **)0x03001e74 + 0x2ec);

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
            FIELD(character, u16, 0xd8 + slot * 2) = item;
        }
    }

    return 0;
}
