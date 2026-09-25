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

struct OwnerAbility {
    u16 id;
    u16 unk_02;
};

struct OwnerRecord {
    u8 name[0x0f];
    u8 level;
    u8 unk_10[0x48];
    struct OwnerAbility abilities[32];
    u16 inventory[15];
    u8 unk_0f6[0x33];
    u8 class_index;
};

struct ClassAbility {
    u8 ability;
    u8 level;
    u8 unk_02[2];
};

struct ClassRecord {
    u8 unk_00[0x10];
    struct ClassAbility abilities[16];
};

struct ItemDefinition {
    u8 unk_00[0x0c];
    u8 use_kind;
    u8 unk_0d[0x1b];
    u16 ability;
};

extern u8 Data_00000000[];
extern u8 Value_00000200;
extern u8 Value_00004000;

struct Half {
    u16 v;
};

struct OwnerRecord *Owner_GetState(s32 owner);
struct ClassRecord *Owner_GetRecordStride84(s32 class_index);
struct ItemDefinition *Item_GetDirect(s32 item);

s32 Owner_RefreshClassActions(s32 owner)
{
    struct OwnerRecord *st;
    struct OwnerAbility *abil;
    struct ClassRecord *cls;
    struct ItemDefinition *def;
    struct Half zero;
    s32 i;
    s32 j;
    s32 k;
    u16 item;

    st = Owner_GetState(owner);
    abil = st->abilities;
    cls = Owner_GetRecordStride84(st->class_index);
    if (st->class_index == 0)
        return 0;
    zero.v = 0;
    for (i = 0; i < 32; i++) {
        if (abil[i].id & 0x8000)
            abil[i].id = zero.v;
    }
    zero.v = 0;
    for (i = 0; i < 32; i++) {
        if (abil[i].id & 0x4000)
            abil[i].id = zero.v;
    }
    for (i = 31, j = 31; i >= 0;) {
        if (abil[i].id == 0) {
            i--;
        } else {
            abil[j].id = abil[i].id;
            i--;
            j--;
        }
    }
    zero.v = 0;
    for (; j >= 0; j--)
        abil[j].id = zero.v;

    for (k = 0; k < 16; k++) {
        if (cls->abilities[k].ability == 0 || st->level < cls->abilities[k].level)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].id == cls->abilities[k].ability)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].id == 0) {
                abil[i].id = cls->abilities[k].ability | 0x8000;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (k = 0; k < 15; k++) {
        item = st->inventory[k];
        if (item == 0 || !(item & (u16)(s32)&Value_00000200))
            continue;
        def = Item_GetDirect(st->inventory[k]);
        if (def->use_kind != 3)
            continue;
        for (i = 0; i < 32; i++) {
            if ((abil[i].id & 0x3fff) == def->ability)
                break;
        }
        if (i != 32)
            continue;
        for (i = 0; i < 32; i++) {
            if (abil[i].id == 0) {
                abil[i].id = def->ability | (u16)(s32)&Value_00004000;
                break;
            }
        }
        if (i == 32)
            break;
    }

    for (i = 0, j = 0; i < 32;) {
        if (abil[i].id == 0) {
            i++;
        } else {
            abil[j].id = abil[i].id;
            i++;
            j++;
        }
    }
    zero.v = 0;
    for (; j < 32; j++)
        abil[j].id = zero.v;
    return 0;
}
