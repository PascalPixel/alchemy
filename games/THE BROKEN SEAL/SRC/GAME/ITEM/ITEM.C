#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_EFX.H"
#include "BATTLE_TYPES.H"
#include "SCENE.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "INVENTORY_MENU.H"
#include "ITEM.H"

extern struct InventoryMenuState *gMenuWork;
struct BattleUnit *Owner_GetStateFar(s32);
struct ItemDefinition *Item_Get(s32);
s32 Inventory_RemoveFar(s32, s32);
s32 ItemMenu_Collect(struct BattleUnit *, u16 *, s32);
s32 BattleEffect_ApplyToTargets(s32, s32, s32, s32);
void BattleUnit_Recalculate(s32);
struct BattleAction *BattleAction_Get(s32);
void Owner_RecalculateRatiosFar(s32);
s32 Battle_CalcRestore(s32, s32, s32);
void UiWork_PushValueSlotFar(s32, s32);
s32 Audio_PlayCueReturnOne(s32 cue);

/* ability/play_use_animation.c */
#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

/* menu/core/get_modulo_of_sum.c */
void Ability_PlayUseAnimation(s32 action);

struct ItemMenuModeState {
    u8 reserved_000[0x20c];
    u8 mode;
};

struct ItemMenuDisplayState {
    u16 reserved_00[2];
    u16 busy;
};

/* The pointer at 0x2128 is cleared beside the variant; its pointer type
   keeps the two stores apart for the scheduler. */
struct ItemMenuSavedGraphics {
    u8 reserved_000[0xa8];
    u8 tiles[0x2000];
    u16 palette[64];
    void *reserved_2128;
    s32 variant;
};

struct ItemMenuWork {
    u8 reserved_000[0x1c];
    u8 selection;
    u8 target;
    u8 reserved_01e[0xee];
    s32 window;
    u8 reserved_110[0x64];
    u16 slot;
    u16 target_slot;
    u16 item;
    u8 reserved_17a[10];
    struct ItemMenuSavedGraphics *saved;
    u8 reserved_188[0x80];
    u16 owners[8];
    u8 reserved_218;
    u8 count;
};

extern struct ItemMenuModeState gGameState;
extern struct ItemMenuDisplayState *gMenuCtrlWork;
s32 GameFlag_TestFar(s32 flag);
void UiWindow_DrawFrameFar(s32 x, s32 y, s32 width, s32 height);
void UiWindow_InitializeWork(s32 mode);
void Func_080153e0(s32 value);
void Func_080152a8(void);
void Link_DrawShiftedTilePairFar(void *tiles);
s32 Party_ListActiveOwnersFar(u16 *owners);
void Resource_LoadPairedBlocksIfAvailable(void);
void ItemMenu_Init(s32 x, s32 y, s32 mode, s32 columns);
void Palette_LightenBankHighlight(s32 palette);
s32 UiWindow_CreateFar(s32 x, s32 y, s32 width, s32 height, s32 style);
void FourObjectMotion_InitializeBottomRow(s32 window, s32 mode);
void Func_080aa768(void);
void FourObjectMotion_ClearSlotsAndScheduleAlt(void);
void Menu_ResetTwoResourceEntries(void);
void ItemMenu_Close(void);
void UiWindow_EraseBorderRectFar(s32 x, s32 y, s32 width, s32 height);
void Event_ClearInvalidPackedValuesFar(void);

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0);

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
    struct InventoryMenuState *work;
    s32 result;
    s32 item_id;
    struct BattleUnit *owner;
    struct ItemDefinition *item;

    owner = Owner_GetStateFar(owner_id);
    work = gMenuWork;
    item_id = 0x1ff & owner->inventory[slot];
    item = Item_Get(item_id);
    result = BattleEffect_ApplyToTargets(
        0x3fff & item->action_id,
        owner_id,
        target_id,
        1);
    if (result != -1) {
        item = Item_Get(owner->inventory[slot]);
        if (item->use_type == 1) {
            Inventory_RemoveFar(owner_id, slot);
            work->item_count =
                ItemMenu_Collect(owner, work->items, 0);
        }
        if (item->use_type == 4) {
            if (item_id == 0xb8)
                item_id = 0xb9;
            owner->inventory[slot] = item_id;
        }
        result = 0;
    }
    return result;
}

s32 Item_ReturnOne(void)
{
    return 1;
}

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
    struct InventoryMenuState *runtime;
    struct BattleUnit *target;
    struct BattleUnit *source;
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

    /* FAKEMATCH: dead range test; gcse reuses its read at the loop top.
       2026-10-02: deleting this test delays the range load until after the
       count guard and changes the target-index and power-load registers. */
    if (effect->range == 0xff && later_target != 0)
        result_code = 3;

    for (index = 0; index < runtime->party_count; index++) {
        if (effect->range == 0xff) {
            target_id = runtime->owner_ids[index];
            target = Owner_GetStateFar(target_id);
        }

        amount = effect->power;
        switch (effect->target_flags & 0xf) {
        case BATTLE_DAMAGE_HP_HEAL:
            if (fixed_scale == 0) {
                if (effect->damage_class != 4) {
                    s32 stat_offset;

                    source = Owner_GetStateFar(source_id);
                    stat_offset = effect->damage_class;
                    scale = source->elements[stat_offset].power;
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
                *(u16 *)&target->base_hp += amount + random_adjust;
                result_code = 0x10;
                changed = 1;
                break;
            case 0x105:
                *(u16 *)&target->base_pp += amount + random_adjust;
                result_code = 0x11;
                changed = 1;
                break;
            case 0x108:
                target->base_agility += amount + random_adjust;
                result_code = 0x12;
                changed = 1;
                break;
            case 0x109:
                target->base_luck += amount;
                result_code = 0x13;
                changed = 1;
                break;
            case 0x106:
                target->base_attack += amount + random_adjust;
                UiWork_PushValueSlotFar(3, 5);
                result_code = 0x14;
                changed = 1;
                break;
            case 0x107:
                target->base_defense += amount + random_adjust;
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
                target->hp = target->max_hp * 7 / 10;
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
        runtime->message_offset = result_code;
        return -1;
    }

    for (index = 0; index < runtime->party_count; index++)
        BattleUnit_Recalculate(runtime->owner_ids[index]);
    runtime->message_offset = result_code;
    return 0;
}

void Item_PlayUseAnimation(u32 item)
{
    Ability_PlayUseAnimation(Item_Get(item)->action_id & 0x3fff);
}

/* menu/entry/set_first_object_row_coordinates.c */
void Ability_PlayUseAnimation(s32 action)
{
    s32 animation_type;
    u32 target_type;
    struct BattleAction *ability;

    ability = BattleAction_Get(action);
    animation_type = 0xf & ability->target_flags;
    switch (animation_type) {
    case 1:
        Audio_PlayCueReturnOne(0x7e);

    case 11:
        Audio_PlayCueReturnOne(0x7e);
        return;
    default:
        target_type = ability->effect - 1;
        switch (target_type) {
        case 4:
            Audio_PlayCueReturnOne(0x52);
            return;
        case 2:
            Audio_PlayCueReturnOne(0x54);
            return;
        default:
        case 3:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
        case 28:
        case 29:
            Audio_PlayCueReturnOne(0x5b);

        case 0:
        case 1:
        case 30:
        case 31:
            return;
        }
        break;
    }
}

/* menu/reserved_status_zero.c */
/* menu/status/reserved_status_zero.c */
s32 Menu_ReservedStatusZero(void)
{
    return 0;
}

s32 Menu_GetModuloOfSum(s32 arg0, s32 arg1)
{
    return (arg0 + arg1) % arg1;
}

s32 Menu_SetFirstObjectRowCoordinates(s32 arg0)
{
    u8 *current;
    s32 value;
    s32 count;
    current = ((u8 *)gMenuWork) + 0x134;
    arg0 += 0x3D;
    value = 0x20;
    count = 3;
    do {
        count--;
        *(s16 *)current = value;
        *(s16 *)(current + 0x10) = arg0;
        value += 0x38;
        current += 2;
    } while (count >= 0);
    return arg0;
}

/* Open the item menu as a modal view, selecting its story variant, then
   restore the saved graphics and the caller's menu mode. */
s32 ItemMenu_Run(void)
{
    struct ItemMenuWork *menu;
    u32 old_mode;
    s32 state;
    struct ItemMenuSavedGraphics *saved;
    s32 zero;

    menu = Runtime_AllocateHeapBlock(55, 0xa70);
    old_mode = gGameState.mode;
    gGameState.mode = 2;
    state = gMenuCtrlWork->busy = 1;
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    WaitFrames(1);
    UiWindow_InitializeWork(0);
    saved = (struct ItemMenuSavedGraphics *)Runtime_BumpAllocateAlternatePool(0x2130);
    menu->saved = saved;
    saved->reserved_2128 = 0;
    saved->variant = 0;
    if (GameFlag_TestFar(0x16e)) {
        if (!GameFlag_TestFar(0x16f)) {
            if (!GameFlag_TestFar(0x171))
                saved->variant = state;
            else
                saved->variant = 14;
        } else {
            if (!GameFlag_TestFar(0x171))
                saved->variant = 27;
            else
                saved->variant = 28;
        }
    }
    Scheduler_EnableOverlayCallbacksWithFlags();
    Func_080153e0(1);
    Link_DrawShiftedTilePairFar((void *)0x06002500);
    menu->count = Party_ListActiveOwnersFar(menu->owners);
    Resource_LoadPairedBlocksIfAvailable();
    ItemMenu_Init(0, 3, 0, 7);
    Menu_SetFirstObjectRowCoordinates(0);
    Palette_LightenBankHighlight(14);
    menu->window = UiWindow_CreateFar(13, 0, 17, 5, 2);
    menu->item = 255;
    zero = 0;
    menu->selection = zero;
    menu->target = zero;
    menu->slot = 0;
    menu->target_slot = 0;
    FourObjectMotion_InitializeBottomRow(menu->window, 0);
    Func_080aa768();
    FourObjectMotion_ClearSlotsAndScheduleAlt();
    Menu_ResetTwoResourceEntries();
    WaitFrames(1);
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    gMenuCtrlWork->busy = 0;
    Func_080152a8();
    Func_080153e0(0);
    Iwram_CopyWords((void *)0x06004000, saved->tiles, 0x2000);
    Iwram_CopyWords((void *)0x05000080, saved->palette, 128);
    WaitFrames(1);
    Scheduler_DisableOverlayCallbacksWithFlags();
    UiWindow_EraseBorderRectFar(0, 0, 30, 20);
    Runtime_BumpFree(menu->saved);
    Runtime_ReleaseHeapBlock(55);
    Event_ClearInvalidPackedValuesFar();
    gGameState.mode = old_mode;
    return 1;
}
