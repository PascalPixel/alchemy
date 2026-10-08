#include "mem.h"
#include "types.h"
#include "iwram_call.h"
#include "inventory.h"
#include "btlwork.h"
#include "btltype.h"

#if defined(TBS_EDITION_EN)

struct EnemyRecord {
    u8 unknown_00[0x4c];
    u16 coins;
    s16 item;
    s16 item_chance;
    u16 experience;
};

struct EnemyRecord *Owner_GetRecordFar(s32);
s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(s32);
u32 Random16(void);
u32 BtlRndFar(void);
s32 Item_EncodeBankedId(s32);

void Battle_AwardSpoils(void)
{
    struct BattleSpoils *spoils;
    s32 count;
    u16 *list;
    s32 i;
    s32 unit_id;
    struct SpoilsUnit *unit;
    struct SpoilsUnit *backup;
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
    backup = Runtime_BumpAllocateAlternatePool(sizeof(struct SpoilsUnit));
    for (i = 0; i < count; i++) {
        unit_id = list[i];
        unit = (struct SpoilsUnit *)UnitGetFar(unit_id);
        unit->experience += spoils->experience;
        while (Iwram_CopyWords(backup, unit, sizeof(struct SpoilsUnit)),
            Func_080770b8(unit_id, gains) != 0) {
            Audio_PlayCue(0x59);
            UiWork_ClearValueNameTablesFar();
            UiWork_PushValueSlotFar(unit->class_name, 3);
            UiWork_PushValueSlotFar(list[i], 1);
            UiWork_PushValueSlotFar(unit->level, 5);
            UiText_ShowMessageAndWaitCoreFar((s32)MsgLevelUp);
            BattlePresentation_WaitForAdvance();
            for (cnt = 0; cnt < 32; cnt++) {
                learned = unit->psynergy[cnt].id;
                if ((learned & PSYNERGY_MASK) && (learned >> 15)) {
                    for (j = 0; j < 32; j++) {
                        if (learned == backup->psynergy[j].id)
                            break;
                    }
                    if (j == 32) {
                        UiWork_ClearValueNameTablesFar();
                        UiWork_PushValueSlotFar(unit->class_name, 3);
                        UiWork_PushValueSlotFar(unit_id, 1);
                        UiWork_PushValueSlotFar(learned & PSYNERGY_MASK, 4);
                        Audio_PlayCue(0x9a);
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
