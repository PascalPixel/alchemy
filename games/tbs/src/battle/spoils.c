#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "btlpty.h"
#include "mem.h"
#include "iwram_call.h"
#include "inventory.h"
#include "btlwork.h"
#include "btltype.h"
#include "btlmes.h"
#include "character.h"
#include "btlsys.h"

#define Item_EncodeBankedId ((s32 (*)(s32))Item_EncodeBankedId)
#define Item_EncodeBankedId ((s32 (*)(s32))Item_EncodeBankedId)

u32 Random16(void);

static __inline__ s32 BattleEnemy_CalculateCoinReward(
    struct BtlUnit *unit, u16 *reward)
{
    s32 i;
    s32 bonus = 0;
    s32 base;
    s32 floor;

    for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
        bonus += ((Random16() * 6) >> 16) + 1;
    base = *reward;
    floor = base * 3 / 10;
    if (bonus < floor)
        bonus = floor;
    return bonus + base;
}

static __inline__ s32 BattleEnemy_CalculateExperienceReward(
    struct BtlUnit *unit, u16 *reward)
{
    s32 i;
    s32 bonus = 0;
    s32 base;
    s32 floor;

    for (i = 0; i < (u8)(unit->level / 10U) + 1; i++)
        bonus += ((Random16() * 4) >> 16) + 1;
    base = *reward;
    floor = base * 3 / 10;
    if (bonus < floor)
        bonus = floor;
    return bonus + base;
}

void UiWork_PushValueSlotFar(s32 value, s32 slot);
void AudioCommand_PlayFar(s32 cue);

s32 BattleEnemy_RecordDefeat(s32 unit_id, s32 earned)
{
    struct BtlUnit *unit;
    struct EnemyDefinition *rec;
    struct BtlWork *formation;
    struct BattleSpoils *spoils;
    s32 i;
    s32 chance;
    s32 slot;
    s32 lowest;
    s32 lowest_slot;
    s32 value;

    unit = UnitGetFar(unit_id);
    formation = BtlWk;
    spoils = &formation->spoils;
    if (unit_id >= 0 && unit_id < 8)
        return -1;
    if (unit->class_index != 0)
        return -2;
    slot = 0;
    i = 0;
    if (formation->enemy_classes[0] != unit->class_id) {
        do {
            if (++i > 5)
                break;
        } while (formation->enemy_classes[i] != unit->class_id);
    }
    if (i != 6)
        slot = i;
    if (formation->defeat_state != 2) {
        if (slot < formation->first_defeated)
            formation->first_defeated = slot;
        if (spoils->defeated)
            formation->defeat_state = 1;
    }
    spoils->defeated++;
    if (GameFlag_TestFar(0x173))
        return 0;
    GameFlag_SetBitFar(unit->class_id + 0x600);
    rec = Owner_GetRecordFar(unit->class_id);
    if (earned != 0) {
        if (rec->coins != 0)
            spoils->coins += BattleEnemy_CalculateCoinReward(unit, &rec->coins);
        if (rec->experience != 0)
            spoils->experience += BattleEnemy_CalculateExperienceReward(unit, &rec->experience);
    } else {
        spoils->coins += rec->coins;
        spoils->experience += rec->experience;
    }
    if (rec->item == 0 || rec->item_chance == 0)
        return 0;
    i = 0;
    if (spoils->items[0] != rec->item) {
        do {
            if (++i > 3)
                break;
        } while (spoils->items[i] != rec->item);
    }
    if (i != 4)
        return 0;
    chance = rec->item_chance;
    if (earned != 0)
        chance -= 2;
    if (chance < 0)
        chance = 0;
    if ((0x20000 >> chance) > (BtlRndFar() & 0xffff)) {
        lowest_slot = -1;
        lowest = 0x40000000;
        for (i = 0; i <= 3; i++) {
            value = Item_EncodeBankedId(spoils->items[i]);
            if (value < lowest) {
                lowest = value;
                lowest_slot = i;
            }
        }
        if (Item_EncodeBankedId(rec->item) > lowest)
            spoils->items[lowest_slot] = rec->item;
    }
    return 0;
}

void Battle_AwardSpoils(void)
{
    struct BattleSpoils *spoils;
    s32 count;
    u16 *list;
    s32 i;
    s32 unit_id;
    struct BtlUnit *unit;
    struct BtlUnit *backup;
    s32 cnt;
    u32 learned;
    s32 j;
    s32 best;
    s32 best_slot;
    s32 slot;
    u16 *found;
    s32 value;
    s16 gains[8];
    u16 units[8];

    spoils = &BtlWk->spoils;
    if (spoils->experience != 0) {
        UiWork_PushValueSlotFar(spoils->experience, 5);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgExpGained);
        BattlePresentation_WaitForAdvance();
    }
    list = units;
    count = BattleParty_ListLivingUnits(1, list);
    backup = (struct BtlUnit *)Runtime_BumpAllocateAlternatePool(sizeof(struct BtlUnit));
    for (i = 0; i < count; i++) {
        unit_id = list[i];
        unit = (struct BtlUnit *)UnitGetFar(unit_id);
        unit->experience += spoils->experience;
        while (Iwram_CopyWords(backup, unit, sizeof(struct BtlUnit)),
            Owner_GetValueIfLevelThresholdReachedFar(unit_id, gains) != 0) {
            AudioCommand_PlayFar(0x59);
            UiWork_ClearValueNameTablesFar();
            UiWork_PushValueSlotFar(unit->class_index, 3);
            UiWork_PushValueSlotFar(list[i], 1);
            UiWork_PushValueSlotFar(unit->level, 5);
            UiText_ShowMessageAndWaitCoreFar((s32)MsgLevelUp);
            BattlePresentation_WaitForAdvance();
            for (cnt = 0; cnt < 32; cnt++) {
                learned = unit->action_slots[cnt].encoded_action;
                if ((learned & OWNER_ACTION_ID_MASK) && (learned >> 15)) {
                    for (j = 0; j < 32; j++) {
                        if (learned == backup->action_slots[j].encoded_action)
                            break;
                    }
                    if (j == 32) {
                        UiWork_ClearValueNameTablesFar();
                        UiWork_PushValueSlotFar(unit->class_index, 3);
                        UiWork_PushValueSlotFar(unit_id, 1);
                        UiWork_PushValueSlotFar(learned & OWNER_ACTION_ID_MASK, 4);
                        AudioCommand_PlayFar(0x9a);
                        UiText_ShowMessageAndWaitCoreFar((s32)MsgAbilityMastered);
                        BattlePresentation_WaitForAdvance();
                    }
                }
            }
            if (gains[2] != 0) {
                UiWork_PushValueSlotFar(gains[2], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxHpRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[3] != 0) {
                UiWork_PushValueSlotFar(gains[3], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgMaxPpRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[4] != 0) {
                UiWork_PushValueSlotFar(gains[4], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgAttackRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[5] != 0) {
                UiWork_PushValueSlotFar(gains[5], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgDefenseRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[6] != 0) {
                UiWork_PushValueSlotFar(gains[6], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgAgilityRises);
                BattlePresentation_WaitForAdvance();
            }
            if (gains[7] != 0) {
                UiWork_PushValueSlotFar(gains[7], 5);
                UiText_ShowMessageAndWaitCoreFar((s32)MsgLuckRises);
                BattlePresentation_WaitForAdvance();
            }
        }
    }
    MemFree(backup);
    if (spoils->coins != 0) {
        UiWork_PushValueSlotFar(spoils->coins, 5);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgCoinsGained);
        Party_AdjustCoinsFar(spoils->coins);
        BattlePresentation_WaitForAdvance();
    }
    found = &gOverflowItem;
    for (;;) {
        best_slot = -1;
        best = -1;
        for (slot = 0; slot <= 3; slot++) {
            if (spoils->items[slot] != 0) {
                value = Item_EncodeBankedId(spoils->items[slot]);
                if (value >= best) {
                    best = value;
                    best_slot = slot;
                }
            }
        }
        if (best_slot == -1)
            break;
        UiWork_PushValueSlotFar(spoils->items[best_slot], 2);
        UiText_ShowMessageAndWaitCoreFar((s32)MsgItemGained);
        BattlePresentation_WaitForAdvance();
        if (PartyInventory_AddFar(spoils->items[best_slot]) == -1) {
            *found = spoils->items[best_slot];
            break;
        }
        spoils->items[best_slot] = 0;
    }
}

void Battle_ReservedNoOp2A08(void)
{
}
