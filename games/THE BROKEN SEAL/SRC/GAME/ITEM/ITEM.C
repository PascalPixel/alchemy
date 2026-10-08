#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "TYPES.H"
#include "MEM.H"
#include "CALLBACK_SCHEDULER.H"
#include "BATTLE_EFX.H"
#include "BTLTYPE.H"
#include "SCENE.H"
#include "SYSTEM.H"
#include "IWRAM_CALL.H"
#include "INVENTORY_MENU.H"
#include "ITEM.H"
#include "DJINN_MENU.H"
#include "BTLSYS.H"
#include "BTLCALC.H"
#include "UI.H"

#define RenderOutput_ClearListFar ((void (*)(s32))RenderOutput_ClearListFar)

#define Link_DrawShiftedTilePairFar ((void (*)(void*))Link_DrawShiftedTilePairFar)
#define FourObjectMotion_InitializeBottomRow ((void (*)(s32,s32))FourObjectMotion_InitializeBottomRow)
#define Djinn_DeactivateFar ((s32 (*)(s32,s32,s32))Djinn_DeactivateFar)
#define Trade_AddOfferFar ((s32 (*)(s32,s32,s32))Trade_AddOfferFar)

#define gMenuCtrlWork (*(struct ItemMenuDisplayState * *)&gMenuCtrlWork)

extern struct InventoryMenuState *gMenuWork;
void UiWork_PushValueSlotFar(s32, s32);

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

struct ItemMenuModeState {
    u8 reserved_000[0x20c];
    u8 mode;
};

struct ItemMenuDisplayState {
    u16 reserved_00[2];
    u16 busy;
};

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

s32 Item_Use(s32 slot, s32 owner_id, s32 target_id)
{
    struct InventoryMenuState *work;
    s32 result;
    s32 item_id;
    struct BtlUnit *owner;
    struct ItemDefinition *item;

    owner = UnitGetFar(owner_id);
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

s32 BattleEffect_ApplyToTargets(
    s32 effect_id,
    s32 source_id,
    s32 target_id,
    s32 fixed_scale)
{
    struct BtlAct *effect;
    struct InventoryMenuState *runtime;
    struct BtlUnit *target;
    struct BtlUnit *source;
    s32 later_target;
    s32 changed;
    u8 index;
    s16 scale;
    s32 random_adjust;
    s32 random_nonone;
    s32 amount;
    s32 result_code;

    effect = BtlActGet(effect_id);
    runtime = gMenuWork;
    changed = 0;
    result_code = 0;
    later_target = 0;

    if (target_id != 9)
        target = UnitGetFar(target_id);
    else
        target = UnitGetFar(0);

    /* FAKEMATCH: the dead test keeps the range load before the count guard and preserves target-index and power-load registers. */
    if (effect->rng == 0xff && later_target != 0)
        result_code = 3;

    for (index = 0; index < runtime->party_count; index++) {
        if (effect->rng == 0xff) {
            target_id = runtime->owner_ids[index];
            target = UnitGetFar(target_id);
        }

        amount = effect->pow;
        switch (effect->tgt_flg & 0xf) {
        case BATTLE_DAMAGE_HP_HEAL:
            if (fixed_scale == 0) {
                if (effect->damage_class != 4) {
                    s32 stat_offset;

                    source = UnitGetFar(source_id);
                    stat_offset = effect->damage_class;
                    scale = source->elm[stat_offset].pow;
                } else
                    scale = 100;
                amount = BtlRcvCalc(amount, scale, 0x100);
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
                    UnitRatioFar(target_id);
                    changed = 1;
                    if (effect->rng == 0xff) {
                        later_target = 1;
                        result_code = 3;
                    }
                }
            }
            break;

        case 9:
            random_adjust = (Random16() * 4) >> 16;
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
                target->base_hp += amount + random_adjust;
                result_code = 0x10;
                changed = 1;
                break;
            case 0x105:
                target->base_ep += amount + random_adjust;
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
            if (target->ep == target->max_ep) {
                if (later_target == 0)
                    result_code = 7;
            } else {
                target->ep += amount;
                if (target->ep > target->max_ep) {
                    amount -= target->ep - target->max_ep;
                    target->ep = target->max_ep;
                    if (later_target == 0)
                        result_code = 5;
                } else if (later_target == 0) {
                    result_code = 6;
                }
                UnitRatioFar(target_id);
                changed = 1;
                if (effect->rng == 0xff) {
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

        switch (effect->efx) {
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
                UnitRatioFar(target_id);
                changed = 1;
            }
            break;

        case 2:
            if (target->ep == target->max_ep) {
                if (later_target == 0)
                    result_code = 7;
            } else {
                target->ep += amount;
                if (target->ep > target->max_ep) {
                    target->ep = target->max_ep;
                    if (later_target == 0)
                        result_code = 5;
                } else if (later_target == 0) {
                    result_code = 6;
                }
                UnitRatioFar(target_id);
                changed = 1;
            }
            break;

        case EFX_REVIVE_FULL:
            if (target->hp == 0) {
                target->hp = target->max_hp;
                UnitRatioFar(target_id);
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
                UnitRatioFar(target_id);
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_REVIVE_80:
            if (target->hp == 0) {
                target->hp = target->max_hp * 7 / 10;
                UnitRatioFar(target_id);
                if (later_target == 0)
                    result_code = 0xc;
            } else if (later_target == 0) {
                result_code = 0xd;
            }
            break;

        case EFX_CURE_POISON:
            if (target->doku != 0) {
                target->doku = 0;
                changed = 1;
                if (later_target == 0)
                    result_code = 0xa;
            } else if (later_target == 0) {
                result_code = 0xb;
            }
            break;
        }

        if (effect->rng != 0xff)
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

void Ability_PlayUseAnimation(s32 action)
{
    s32 animation_type;
    u32 target_type;
    struct BtlAct *ability;

    ability = BtlActGet(action);
    animation_type = 0xf & ability->tgt_flg;
    switch (animation_type) {
    case 1:
        Audio_PlayCueReturnOne(0x7e);

    case 11:
        Audio_PlayCueReturnOne(0x7e);
        return;
    default:
        target_type = ability->efx - 1;
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
    UiWork_SetAltFlagAndClearTableFar(1);
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
    DjinnMenu_RunScreen();
    FourObjectMotion_ClearSlotsAndScheduleAlt();
    Menu_ResetTwoResourceEntries();
    WaitFrames(1);
    ItemMenu_Close();
    UiWindow_DrawFrameFar(0, 0, 30, 20);
    gMenuCtrlWork->busy = 0;
    UiWindow_MarkVisibleTileAttributesFar();
    UiWork_SetAltFlagAndClearTableFar(0);
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

void AudioCommand_PlayFar(s32 cue);

s32 DjinnMenu_RunScreen(void)
{
    struct DjinnMenuWork *menu;
    struct RenderOutput *cursor;
    s32 result;
    s32 state;
    s32 ret;
    s32 done;
    s32 i;

    done = 0;
    result = 0;
    ret = 0;
    menu = (struct DjinnMenuWork *)gMenuWork;
    cursor = menu->cursor;
    cursor->active = 13;
    cursor->anim_step = 0;
    Menu_OpenBackdropScreen();
    WaitFrames(1);
    state = 2;
    do {
        switch (state) {
        case 0:
            if (result < 0) {
                ret = -1;
                done = 1;
            }
            state = 2;
            break;
        case 1:
            break;
        case 2:
            Menu_SetFirstObjectRowCoordinates(0);
            FourObjectMotion_SetSlotPosition(1, 0, 200, 0);
            result = DjinnMenu_SelectDjinn(0);
            state = 15;
            if (result == 10)
                break;
            state = 0;
            if (result < 0)
                break;
            menu->selection[1] = menu->selected_column[0];
            state = 10;
            if (result == 7)
                break;
            state = 3;
            break;
        case 10:
            menu->owner = menu->owners[menu->selected_column[0]];
            menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
            result = DjinnMenu_ShowOwnerStatus();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 15:
            result = DjinnMenu_ShowHelp();
            if (result == -2)
                done = 1;
            state = 2;
            break;
        case 8:
            state = 0;
            if (menu->pair_count == 0)
                break;
            result = DjinnMenu_SelectDjinn(1);
            if (result == -2)
                done = 1;
            state = 4;
            if (result < 0)
                break;
            state = 9;
            break;
        case 3:
            DjinnMenu_DrawElementList(&menu->backdrop->list);
            Menu_SetFirstObjectRowCoordinates(-8);
            menu->owner = menu->owners[menu->selected_column[0]];
            menu->shown_owner[0] = menu->owners[menu->selected_column[0]];
            FourObjectMotion_SetSlotPosition(0, menu->selected_column[0] * 56 + 48, 54, 0);
            result = DjinnMenu_SelectDjinn(1);
            for (i = 0; i < menu->owner_count; i++)
                menu->owner_y[i] += 8;
            if (result == -2)
                done = 1;
            if (result < 0) {
                state = 2;
                break;
            }
            if (result == 3 || result == 4 || result == 8 || result == 9)
                menu->shown_owner[1] = menu->owners[menu->selected_column[1]];
            if (result < 0)
                state = 2;
            else if (result == 1)
                state = 5;
            else if (result == 2)
                state = 6;
            else if (result == 3) {
                menu->pending = 2;
                state = 7;
            } else if (result == 4) {
                menu->pending = 2;
                state = 9;
            } else if (result == 5)
                state = 11;
            else if (result == 6)
                state = 12;
            else if (result == 8) {
                menu->pending = 2;
                state = 13;
            } else if (result == 9) {
                menu->pending = 2;
                state = 14;
            }
            break;
        case 7:
            result = OwnerAction_RunCompareLoop(1);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 13:
            AudioCommand_PlayFar(126);
            result = Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0], menu->shown_owner[1]);
            UnitCalcFar(menu->shown_owner[0]);
            UnitCalcFar(menu->shown_owner[1]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            Menu_ComputeEntryValues(menu->backdrop);
            {
                s32 column;
                s32 found;
                u8 entry;

                column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
                found = 0;
                entry = menu->pair_entries[0];
                for (i = 0; i < menu->backdrop->list.counts[column]; i++) {
                    if (entry == (u8)menu->backdrop->list.ids[column][i]) {
                        found = i;
                        break;
                    }
                }
                /* FAKEMATCH: needed to match: a one-pass block builds the selection address before found * 10 */
                do {
                    menu->selection[0] = column + found * DJINN_LIST_ROWS;
                } while (0);
                menu->cursor->active = 1;
            }
            state = 0;
            break;
        case 6:
            result = OwnerAction_RunCompareLoop(2);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 12:
            AudioCommand_PlayFar(175);
            Djinn_DeactivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            result = Trade_AddOfferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            UnitCalcFar(menu->shown_owner[0]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            menu->cursor->active = 1;
            state = 2;
            break;
        case 9:
            result = OwnerAction_RunCompareLoop(0);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 14:
            AudioCommand_PlayFar(126);
            Djinn_TransferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0], menu->shown_owner[1]);
            result = Djinn_TransferFar(menu->shown_owner[1], menu->pair_element[1], menu->pair_index[1], menu->shown_owner[0]);
            UnitCalcFar(menu->shown_owner[0]);
            UnitCalcFar(menu->shown_owner[1]);
            Menu_ComputeEntryValues(menu->backdrop);
            {
                s32 column;
                s32 found;
                u8 entry;

                column = (u16)((u32)menu->selection[1] % DJINN_LIST_ROWS);
                found = 0;
                entry = menu->pair_entries[0];
                for (i = 0; i < menu->backdrop->list.counts[column]; i++) {
                    if (entry == (u8)menu->backdrop->list.ids[column][i]) {
                        found = i;
                        break;
                    }
                }
                /* FAKEMATCH: needed to match: a one-pass block builds the selection address before found * 10 */
                do {
                    menu->selection[0] = column + found * DJINN_LIST_ROWS;
                } while (0);
                menu->cursor->active = 1;
            }
            state = 2;
            break;
        case 4:
            if (result == -1) {
                ret = result;
                state = 2;
            } else if (menu->pending & 1)
                state = 8;
            else if (menu->pending & 2)
                state = 7;
            break;
        case 5:
            result = OwnerAction_RunCompareLoop(3);
            if (result == -2)
                done = 1;
            state = 3;
            if (result < 0)
                break;
        case 11:
            AudioCommand_PlayFar(139);
            Djinn_ActivateFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            result = Trade_RemoveOfferFar(menu->shown_owner[0], menu->pair_element[0], menu->pair_index[0]);
            UnitCalcFar(menu->shown_owner[0]);
            menu->cursor->active = 13;
            RenderOutput_ClearListFar(menu->list_window);
            menu->cursor->active = 1;
            state = 2;
            break;
        default:
            done = 1;
            break;
        }
    } while (done == 0);
    return ret;
}
