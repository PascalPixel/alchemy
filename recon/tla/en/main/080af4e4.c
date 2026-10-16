/* Near miss, differ 4 of 247 (two reordered pairs), 2026-10-15: The Broken
 * Seal's Unit_RefreshPsynergy on this game's tables: one clearing pass
 * with both marks (0xc000), class abilities as halfwords with their level
 * at +2, and the class mark ored as a constant. Remaining: in both fill
 * loops' preheaders the native loads the pooled halfword zero before the
 * slot address is finished (ldr r2 before lsls r3, r6, #2; ldr r1 before
 * adds r2, r3, r0); this finishes the address first. From the sched2 dump:
 * the zero (cost 2, no dependents) ties the address on priority and loses
 * on dependents. The zero pinned to r2 behind an empty asm that holds r3
 * fixes the first loop, but any asm there lengthens j's live range so j and
 * the class pointer trade r6 and r7 (differ 64); pinning j as well breaks
 * the frame (differ 272). The pin alone, while and do forms, a pointer
 * form and a named zero change nothing. Four minutes of alchemy permute
 * found nothing lower. */
#include "types.h"
#include "owner_state.h"
#include "item.h"

struct ClassAbility {
    u16 ability;
    u8 level;
    u8 unk03;
};

struct ClassAbilities {
    u8 unk00[0x10];
    struct ClassAbility abilities[16];
};

enum {
    OWNER_ACTION_CLASS = 0x8000,
    OWNER_ACTION_ITEM = 0x4000
};

s32 Owner_UpdateClassActions(s32 owner)
{
    struct BattleUnit *st;
    u16 *ability;
    struct ClassAbilities *cls;
    struct ItemDefinition *def;
    s32 i;
    s32 j;
    u16 mask;
    struct ClassAbility *cg;

    st = UnitGet(owner);
    ability = (u16 *)st->actionSlots;
    cls = (struct ClassAbilities *)Class_GetDefinition(st->classIndex);
    if (st->classIndex == 0)
        return 0;
    mask = OWNER_ACTION_CLASS | OWNER_ACTION_ITEM;
    for (j = 0; j < 32; j++) {
        if (ability[j * 2] & mask)
            ability[j * 2] = 0;
    }
    for (i = 31, j = 31; i >= 0;) {
        if (ability[i * 2] == 0) {
            i--;
        } else {
            ability[j * 2] = ability[i * 2];
            i--;
            j--;
        }
    }
    for (; j >= 0; j--)
        ability[j * 2] = 0;

    mask = OWNER_ACTION_CLASS;
    for (j = 0; j < 16; j++) {
        cg = &cls->abilities[j];
        if (cls->abilities[j].ability == 0)
            continue;
        if (st->level < cls->abilities[j].level)
            continue;
        i = 0;
        while (i < 32 && ability[i * 2] != cls->abilities[j].ability)
            i++;
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (ability[i * 2] == 0) {
                ability[i * 2] = cg->ability | OWNER_ACTION_CLASS;
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
        if (def->useType != 3)
            continue;
        for (i = 0; i < 32; i++) {
            if ((ability[i * 2] & OWNER_ACTION_ID_MASK) == def->actionId)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (ability[i * 2] == 0) {
                ability[i * 2] = def->actionId | OWNER_ACTION_ITEM;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (i = 0, j = 0; i < 32;) {
        if (ability[i * 2] == 0) {
            i++;
        } else {
            ability[j * 2] = ability[i * 2];
            i++;
            j++;
        }
    }
    for (; j < 32; j++)
        ability[j * 2] = 0;
    return 0;
}
