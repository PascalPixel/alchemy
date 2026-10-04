/* Draft, not exact (2026-09-25): 546 of 568 bytes, 273 differing halfwords.
   Written from the listing. What lined up: the zero as a one-halfword struct
   set before each clearing loop (its pool lands mid-function as in the ROM),
   the compaction loops stepping i in both arms, and the class abilities
   indexed through cls->abilities[k] rather than an entry pointer.
   Remaining: the ROM keeps the ability array in r8, the owner in r9 and the
   class record in a stack slot with three induction variables over the class
   entries; it re-reads inventory[k] for Item_GetDirect and ORs the 0x8000 /
   0x4000 marks separately in both free-slot paths, where this hoists them. */
#include "TYPES.H"
#include "CHARACTER.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"
#include "ITEM.H"

enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

struct Half {
    u16 v;
};


s32 Owner_RefreshClassActions(s32 owner)
{
    struct BattleUnit *st;
    struct OwnerActionSlot *abil;
    struct ClassDefinition *cls;
    struct ItemDefinition *def;
    struct Half zero;
    s32 i;
    s32 j;
    s32 k;
    u16 item;

    st = Owner_GetState(owner);
    abil = st->action_slots;
    cls = Owner_GetRecordStride84(st->class_index);
    if (st->class_index == 0)
        return 0;
    zero.v = 0;
    for (i = 0; i < 32; i++) {
        if (abil[i].encoded_action & OWNER_ACTION_CLASS)
            abil[i].encoded_action = zero.v;
    }
    zero.v = 0;
    for (i = 0; i < 32; i++) {
        if (abil[i].encoded_action & OWNER_ACTION_ITEM)
            abil[i].encoded_action = zero.v;
    }
    for (i = 31, j = 31; i >= 0;) {
        if (abil[i].encoded_action == 0) {
            i--;
        } else {
            abil[j].encoded_action = abil[i].encoded_action;
            i--;
            j--;
        }
    }
    zero.v = 0;
    for (; j >= 0; j--)
        abil[j].encoded_action = zero.v;

    for (k = 0; k < 16; k++) {
        if (cls->abilities[k].ability == 0 || st->level < cls->abilities[k].level)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].encoded_action == cls->abilities[k].ability)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].encoded_action == 0) {
                abil[i].encoded_action = cls->abilities[k].ability | OWNER_ACTION_CLASS;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (k = 0; k < 15; k++) {
        item = st->inventory[k];
        if (item == 0 || !(item & INVENTORY_EQUIPPED))
            continue;
        def = Item_GetDirect(st->inventory[k]);
        if (def->use_type != 3)
            continue;
        for (i = 0; i < 32; i++) {
            if ((abil[i].encoded_action & OWNER_ACTION_ID_MASK) == def->action_id)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].encoded_action == 0) {
                abil[i].encoded_action = def->action_id | OWNER_ACTION_ITEM;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (i = 0, j = 0; i < 32;) {
        if (abil[i].encoded_action == 0) {
            i++;
        } else {
            abil[j].encoded_action = abil[i].encoded_action;
            i++;
            j++;
        }
    }
    zero.v = 0;
    for (; j < 32; j++)
        abil[j].encoded_action = zero.v;
    return 0;
}
