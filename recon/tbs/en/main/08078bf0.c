/* Draft, not exact. 2026-10-04 sun w2 finisher 3: rewritten; alchemy diff
   317 (was 329), and 87 differing instructions with registers ignored (was
   109). What lines up now, all measured against the listing:
   - the zero as a one-halfword struct: its "ldrh rX, .L" assembles to the
     ROM's pc-relative ldr of a zero word (a 4-byte struct gives an and-mask,
     a u32 field gives movs);
   - the class loop's three ability givs: the ROM tests and ORs through one
     pointer (r7), checks level and the first compare through another (lr)
     and re-reads cls->abilities[k] in the search loop from the spilled cls
     plus an offset giv (r10). That is ca->ability for the test and the OR,
     cls->abilities[k] for the level and the search; all-index or all-ca
     forms give two givs;
   - the 0x8000 mark hoisted out of the class loop (movs/lsls into r11): the
     plain expression narrows the OR to HImode and loads 0x8000 from the
     pool inside the loop; a mark variable keeps it SImode. The item loop's
     0x4000 really is a pool load in each store path.
   Remaining: (1) allocation: the ROM has st in r9, abil in r8, cls spilled
   to [sp+4] (8-byte frame) and k in r5; register pins on st/abil make it
   worse (377). (2) Both free-slot loops: the ROM reuses the search head's
   abil[0] load (r6) for the free-slot head and addresses abil[i] by index
   (lsls/add, no pointer giv), storing in each found path before the
   i == 32 test. The body-store for loop here gets its head from loop.c
   (after GCSE), so the load is repeated and the address strength-reduced.
   A while (abil[i] != 0) { if (++i > 31) goto full; } form makes jump1
   copy the head (the reuse appears) but stores once, after the loop (307
   raw, 97 ops). The search-loop forms (for with break, for with the
   compare in the condition) compile identically. */
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
    s32 mark;
    struct ClassAbility *ca;

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

    mark = OWNER_ACTION_CLASS;
    for (k = 0, ca = cls->abilities; k < 16; k++, ca++) {
        if (ca->ability == 0)
            continue;
        if (st->level < cls->abilities[k].level)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].encoded_action == cls->abilities[k].ability)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].encoded_action == 0) {
                abil[i].encoded_action = ca->ability | mark;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (k = 0; k < 15; k++) {
        if (st->inventory[k] == 0)
            continue;
        if (!(st->inventory[k] & INVENTORY_EQUIPPED))
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
