/* Draft, not exact. 2026-10-05 sun deep finisher 6: alchemy diff 28 (was
   317). What changed, all measured against the listing:
   - the action slots are written through a plain u16 pointer, abil[i * 2]:
     a store to a u16 struct field (abil[i].encoded_action) goes through
     store_field's bit-field path on Thumb (direct_store[HImode] is 0), whose
     extra (mem & 0) | v insns push each free-slot loop's exit code past
     jump1's 20-insn duplicate_loop_exit_test limit. Under the pointer store
     jump1 copies the head, GCSE reuses the search head's abil[0] (r6), the
     free loop keeps index addressing, the stores sit in both paths, cls
     spills to [sp+4] in an 8-byte frame and the zero stores load the
     ROM's pool zero without any struct trick (score 255 from 317);
   - one counter j for the first two clears, the class loop and the item
     loop, as the ROM keeps all of them in r5 (150);
   - a u16 mask variable for the two clears ties the loaded value to the
     and (ldrh r3; ands r3, r4) and, reused as the class mark, gives the
     hoisted 0x8000 in r11 (118, 52, 38);
   - the class loop: ca (pointer, stepped with j) tests the ability and the
     level, cls->abilities[j] the search, and cg = &cls->abilities[j] the
     OR (30; stepping ca before j, 28). Every mix of ca / index / cg / a second stepped pointer over
     the four uses and four cg positions was scored: next best 38 (ca for
     test and OR, index for level and search), all others 38-230.
   Earlier spellings (struct field stores, Half zero struct, while/goto
   free loops, all-index or all-ca class loops) are 300+ and superseded.
   Remaining (28): the ROM keeps the test and the OR on the copied pointer
   (r7 = lr) and the level and the search head on lr; here the test and
   the level read lr and the search head reads r7. */
#include "TYPES.H"
#include "CHARACTER.H"
#include "BATTLE_UNIT.H"
#include "OWNER_STATE.H"
#include "ITEM.H"

enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

s32 Owner_RefreshClassActions(s32 owner)
{
    struct BattleUnit *st;
    u16 *abil;
    struct ClassDefinition *cls;
    struct ItemDefinition *def;
    s32 i;
    s32 j;
    u16 mask;
    struct ClassAbility *ca;
    struct ClassAbility *cg;

    st = Owner_GetState(owner);
    abil = (u16 *)st->action_slots;
    cls = Owner_GetRecordStride84(st->class_index);
    if (st->class_index == 0)
        return 0;
    mask = 0x8000;
    for (j = 0; j < 32; j++) {
        if (abil[j * 2] & mask)
            abil[j * 2] = 0;
    }
    mask = 0x4000;
    for (j = 0; j < 32; j++) {
        if (abil[j * 2] & mask)
            abil[j * 2] = 0;
    }
    for (i = 31, j = 31; i >= 0;) {
        if (abil[i * 2] == 0) {
            i--;
        } else {
            abil[j * 2] = abil[i * 2];
            i--;
            j--;
        }
    }
    for (; j >= 0; j--)
        abil[j * 2] = 0;

    mask = OWNER_ACTION_CLASS;
    for (j = 0, ca = cls->abilities; j < 16; ca++, j++) {
        cg = &cls->abilities[j];
        if (ca->ability == 0)
            continue;
        if (st->level < ca->level)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i * 2] == cls->abilities[j].ability)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i * 2] == 0) {
                abil[i * 2] = cg->ability | mask;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (j = 0; j < 15; j++) {
        if (st->inventory[j] == 0)
            continue;
        if (!(st->inventory[j] & INVENTORY_EQUIPPED))
            continue;
        def = Item_GetDirect(st->inventory[j]);
        if (def->use_type != 3)
            continue;
        for (i = 0; i < 32; i++) {
            if ((abil[i * 2] & OWNER_ACTION_ID_MASK) == def->action_id)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i * 2] == 0) {
                abil[i * 2] = def->action_id | OWNER_ACTION_ITEM;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (i = 0, j = 0; i < 32;) {
        if (abil[i * 2] == 0) {
            i++;
        } else {
            abil[j * 2] = abil[i * 2];
            i++;
            j++;
        }
    }
    for (; j < 32; j++)
        abil[j * 2] = 0;
    return 0;
}
