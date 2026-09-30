#include "TYPES.H"
#include "BATTLE_EFX.H"
#include "BATTLE_TYPES.H"

struct ItemOwner {
    u8 padding_000[0x10];
    u16 stat_10;
    u16 stat_12;
    u8 padding_014[4];
    u16 stat_18;
    u16 stat_1a;
    u16 stat_1c;
    u8 stat_1e;
    u8 padding_01f[0x15];
    s16 max_hp;                     /* 0x034 */
    s16 max_pp;
    s16 hp;
    s16 pp;
    u8 padding_03c[0x9c];
    u16 items[15];                  /* 0x0d8 */
    u8 padding_0f6[0x3b];
    s8 poison;                      /* 0x131 */
};

struct ItemData {
    u8 padding_00[0x0c];
    u8 kind;
    u8 padding_0d[0x1b];
    u16 use_ability;
};

struct ItemUseWork {
    u8 padding_000[0x1c8];
    u16 entries[32];
    u16 targets[8];                 /* 0x208, the party list */
    u8 entry_count;                 /* 0x218 */
    u8 target_count;                /* 0x219 */
    u8 padding_21a[0x40];
    s16 result_code;                /* 0x25a */
};

extern struct ItemUseWork *gMenuWork;

struct ItemOwner *Owner_GetStateFar(s32);
struct ItemData *Item_Get(s32);
u8 Inventory_RemoveFar(s32, s32);
u32 ItemMenu_Collect(struct ItemOwner *, u16 *, s32);
s32 BattleEffect_ApplyToTargets(s32, s32, s32, s32);

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
    struct ItemUseWork *work;
    s32 result;
    s32 item_id;
    struct ItemOwner *owner;
    struct ItemData *item;

    owner = Owner_GetStateFar(owner_id);
    work = gMenuWork;
    item_id = 0x1ff & owner->items[slot];
    item = Item_Get(item_id);
    result = BattleEffect_ApplyToTargets(
        0x3fff & item->use_ability,
        owner_id,
        target_id,
        1);
    if (result != -1) {
        item = Item_Get(owner->items[slot]);
        if (item->kind == 1) {
            Inventory_RemoveFar(owner_id, slot);
            work->entry_count =
                ItemMenu_Collect(owner, work->entries, 0);
        }
        if (item->kind == 4) {
            if (item_id == 0xb8)
                item_id = 0xb9;
            owner->items[slot] = item_id;
        }
        result = 0;
    }
    return result;
}

s32 Item_ReturnOne(void)
{
    return 1;
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

void BattleUnit_Recalculate(s32);
struct BattleAction *BattleAction_Get(s32);
void Owner_RecalculateRatiosFar(s32);
s32 Battle_CalcRestore(s32, s32, s32);
s32 Random16(void);
void UiWork_PushValueSlotFar(s32, s32);
s32 Math_Div(s32, s32);

/* Apply an item or ability effect to its target, or with range 0xff to
   every member of the menu's party list: restore HP or PP, raise a stat,
   revive or cure. Returns -1 and leaves the result message code when
   nothing changed. The entry block reads the range before the list's first
   count test, as the ROM does. */
s32 BattleEffect_ApplyToTargets(
    s32 effect_id,
    s32 source_id,
    s32 target_id,
    s32 fixed_scale)
{
    struct BattleAction *effect;
    struct ItemUseWork *runtime;
    struct ItemOwner *target;
    struct ItemOwner *source;
    s32 later_target;
    s32 changed;
    u8 index;
    s16 scale;
    s32 random_adjust;
    s32 random_nonone;
    s32 amount;
    s32 result_code;

    effect = BattleAction_Get(effect_id);
    runtime = gMenuWork;
    changed = 0;
    result_code = 0;
    later_target = 0;

    if (target_id != 9)
        target = Owner_GetStateFar(target_id);
    else
        target = Owner_GetStateFar(0);

    /* FAKEMATCH: dead range test; gcse reuses its read at the loop top */
    if (effect->range == 0xff && later_target != 0)
        result_code = 3;

    for (index = 0; index < runtime->target_count; index++) {
        if (effect->range == 0xff) {
            target_id = runtime->targets[index];
            target = Owner_GetStateFar(target_id);
        }

        amount = effect->power;
        switch (effect->target_flags & 0xf) {
        case BATTLE_DAMAGE_HP_HEAL:
            if (fixed_scale == 0) {
                if (effect->damage_class != 4) {
                    s32 stat_offset;

                    source = Owner_GetStateFar(source_id);
                    stat_offset = effect->damage_class * 4 + 0x48;
                    scale = *(s16 *)((u8 *)source + stat_offset);
                } else
                    scale = 100;
                amount = Battle_CalcRestore(amount, scale, 0x100);
            }

            if (target->hp <= 0) {
                if (later_target == 0)
                    result_code = 2;
            } else {
                if (target->hp == target->max_hp) {
                    if (later_target == 0)
                        result_code = 4;
                } else {
                    target->hp += amount;
                    if (target->hp > target->max_hp) {
                        amount -= target->hp - target->max_hp;
                        target->hp = target->max_hp;
                        if (later_target == 0)
                            result_code = 0;
                    } else if (later_target == 0) {
                        result_code = 1;
                    }
                    Owner_RecalculateRatiosFar(target_id);
                    changed = 1;
                    if (effect->range == 0xff) {
                        later_target = 1;
                        result_code = 3;
                    }
                }
            }
            break;

        case 9:
            random_adjust = ((u32)Random16() * 4) >> 16;
            if (random_adjust == 0) {
                random_adjust = -1;
            } else {
                random_nonone = 1 ^ random_adjust;
                random_adjust =
                    (u32)((-random_nonone) | random_nonone) >> 31;
                random_adjust = 1 - random_adjust;
            }

            switch (effect_id & 0x3fff) {
            case 0x104:
                target->stat_10 += amount + random_adjust;
                result_code = 0x10;
                changed = 1;
                break;
            case 0x105:
                target->stat_12 += amount + random_adjust;
                result_code = 0x11;
                changed = 1;
                break;
            case 0x108:
                target->stat_1c += amount + random_adjust;
                result_code = 0x12;
                changed = 1;
                break;
            case 0x109:
                target->stat_1e += amount;
                result_code = 0x13;
                changed = 1;
                break;
            case 0x106:
                target->stat_18 += amount + random_adjust;
                UiWork_PushValueSlotFar(3, 5);
                result_code = 0x14;
                changed = 1;
                break;
            case 0x107:
                target->stat_1a += amount + random_adjust;
                UiWork_PushValueSlotFar(4, 5);
                result_code = 0x15;
                changed = 1;
                break;
            }
            break;

        case BATTLE_DAMAGE_PP_HEAL:
            if (target->pp == target->max_pp) {
                if (later_target == 0)
                    result_code = 7;
            } else {
                target->pp += amount;
                if (target->pp > target->max_pp) {
                    amount -= target->pp - target->max_pp;
                    target->pp = target->max_pp;
                    if (later_target == 0)
                        result_code = 5;
                } else if (later_target == 0) {
                    result_code = 6;
                }
                Owner_RecalculateRatiosFar(target_id);
                changed = 1;
                if (effect->range == 0xff) {
                    later_target = 1;
                    result_code = 8;
                }
            }
            break;

        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 10:
            break;
        }

        switch (effect->effect) {
        case 1:
            if (target->hp <= 0 || target->hp == target->max_hp) {
                if (later_target == 0)
                    result_code = 2;
            } else {
                target->hp += amount;
                if (target->hp > target->max_hp) {
                    target->hp = target->max_hp;
                    if (later_target == 0)
                        result_code = 0;
                } else if (later_target == 0) {
                    result_code = 1;
                }
                Owner_RecalculateRatiosFar(target_id);
                changed = 1;
            }
            break;

        case 2:
            if (target->pp == target->max_pp) {
                if (later_target == 0)
                    result_code = 7;
            } else {
                target->pp += amount;
                if (target->pp > target->max_pp) {
                    target->pp = target->max_pp;
                    if (later_target == 0)
                        result_code = 5;
                } else if (later_target == 0) {
                    result_code = 6;
                }
                Owner_RecalculateRatiosFar(target_id);
                changed = 1;
            }
            break;

        case EFX_REVIVE_FULL:
            if (target->hp == 0) {
                target->hp = target->max_hp;
                Owner_RecalculateRatiosFar(target_id);
                changed = 1;
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_REVIVE_HALF:
            if (target->hp == 0) {
                target->hp = target->max_hp / 2;
                Owner_RecalculateRatiosFar(target_id);
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_REVIVE_80:
            if (target->hp == 0) {
                target->hp = Math_Div(
                    target->max_hp * 7, 10);
                Owner_RecalculateRatiosFar(target_id);
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_CURE_POISON:
            if (target->poison != 0) {
                target->poison = 0;
                changed = 1;
                if (later_target == 0)
                    result_code = 0xa;
            } else if (later_target == 0) {
                result_code = 0xb;
            }
            break;
        }

        if (effect->range != 0xff)
            break;
    }

    if (changed == 0) {
        runtime->result_code = result_code;
        return -1;
    }

    for (index = 0; index < runtime->target_count; index++)
        BattleUnit_Recalculate(runtime->targets[index]);
    runtime->result_code = result_code;
    return 0;
}
#endif
