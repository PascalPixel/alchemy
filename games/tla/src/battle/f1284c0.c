#include "types.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "battle_unit.h"
#include "battle_action.h"
#include "ram_buffer.h"

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK

extern u8 MsgGotExperience[];   // "You got N experience points." (MsgExpGained)
extern u8 MsgGotCoins[];        // "You got N coins." (MsgCoinsGained)
extern u8 MsgGotItem[];         // "You got <item>." (MsgItemGained)
extern u8 MsgLevelUp[];         // "<name> is now a level N ..." (MsgLevelUp)
extern u8 MsgMasteredAbility[]; // "Mastered the <class>'s <ability>"
                                // (MsgAbilityMastered)
extern u8 MsgMaxHpRises[];      // "Maximum HP rises by N!" (MsgMaxHpRises)
extern u8 MsgMaxPpRises[];      // MsgMaxPpRises
extern u8 MsgAttackRises[];     // MsgAttackRises
extern u8 MsgDefenseRises[];    // MsgDefenseRises
extern u8 MsgAgilityRises[];    // MsgAgilityRises
extern u8 MsgLuckRises[];       // "Luck rises by N!" (MsgLuckRises)
extern u16 Data_0200049c;

void MessageArgument_PushFar(s32 value, s32 slot);
void Message_ShowAndWaitCoreFar(s32 message);
void BattleMessage_WaitForAdvance(void);
s32 BattleParty_ListLivingUnits(s32 side, u16 *units);
s32 Func_0811a24c(s32 side, u16 *units);
void *Heap_AllocEwram(s32 size);
struct BattleUnit *Unit_Get(s32 unitId);
void Sound_PlayFar(s32 cue);
void MessageArgument_ClearAllFar(void);
s32 Func_080ad0b0(s32 unitId, s16 *gains);
void Party_AdjustCoinsFar(s32 amount);
s32 BattleSpoils_RankItem(s32 item);
s32 PartyInventory_AddFar(s32 item);

void Battle_AwardSpoils(void) {
  struct BattleSpoils *spoils;
  s32 living;
  u16 *list;
  s32 count;
  s32 i;
  s32 unitId;
  struct BattleUnit *unit;
  struct BattleUnit *backup;
  s32 cnt;
  u32 learned;
  s32 j;
  s32 best;
  s32 bestSlot;
  s32 slot;
  u16 *found;
  s32 value;
  s16 gains[8];
  u16 units[10];

  spoils = (struct BattleSpoils *)((u8 *)Ram_HeapSlots->battleWork + 0x550);
  if (spoils->experience != 0) {
    MessageArgument_PushFar(spoils->experience, 5);
    Message_ShowAndWaitCoreFar((s32)MsgGotExperience);
    BattleMessage_WaitForAdvance();
  }
  list = units;
  living = BattleParty_ListLivingUnits(1, list);
  count = Func_0811a24c(1, list);
  backup = Heap_AllocEwram(sizeof(struct BattleUnit));
  for (i = 0; i < count; i++) {
    unitId = list[i];
    unit = Unit_Get(unitId);
    if (i >= living)
      unit->experience += (u32)spoils->experience >> 1;
    else
      unit->experience += spoils->experience;
    while (Iwram_CopyWords(backup, unit, sizeof(struct BattleUnit)),
           Func_080ad0b0(unitId, gains) != 0) {
      Sound_PlayFar(89);
      MessageArgument_ClearAllFar();
      MessageArgument_PushFar(unit->classIndex, 3);
      MessageArgument_PushFar(list[i], 1);
      MessageArgument_PushFar(unit->level, 5);
      Message_ShowAndWaitCoreFar((s32)MsgLevelUp);
      BattleMessage_WaitForAdvance();
      for (cnt = 0; cnt < 32; cnt++) {
        learned = unit->actionSlots[cnt].encodedAction;
        if ((learned & ACTION_ID_MASK) && (learned >> 15)) {
          for (j = 0; j < 32; j++) {
            if (learned == backup->actionSlots[j].encodedAction)
              break;
          }
          if (j == 32) {
            MessageArgument_ClearAllFar();
            MessageArgument_PushFar(unit->classIndex, 3);
            MessageArgument_PushFar(unitId, 1);
            MessageArgument_PushFar(learned & ACTION_ID_MASK, 4);
            Sound_PlayFar(154);
            Message_ShowAndWaitCoreFar((s32)MsgMasteredAbility);
            BattleMessage_WaitForAdvance();
          }
        }
      }
      if (gains[2] != 0) {
        MessageArgument_PushFar(gains[2], 5);
        Message_ShowAndWaitCoreFar((s32)MsgMaxHpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[3] != 0) {
        MessageArgument_PushFar(gains[3], 5);
        Message_ShowAndWaitCoreFar((s32)MsgMaxPpRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[4] != 0) {
        MessageArgument_PushFar(gains[4], 5);
        Message_ShowAndWaitCoreFar((s32)MsgAttackRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[5] != 0) {
        MessageArgument_PushFar(gains[5], 5);
        Message_ShowAndWaitCoreFar((s32)MsgDefenseRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[6] != 0) {
        MessageArgument_PushFar(gains[6], 5);
        Message_ShowAndWaitCoreFar((s32)MsgAgilityRises);
        BattleMessage_WaitForAdvance();
      }
      if (gains[7] != 0) {
        MessageArgument_PushFar(gains[7], 5);
        Message_ShowAndWaitCoreFar((s32)MsgLuckRises);
        BattleMessage_WaitForAdvance();
      }
    }
  }
  Heap_Release(backup);
  if (spoils->coins != 0) {
    MessageArgument_PushFar(spoils->coins, 5);
    Message_ShowAndWaitCoreFar((s32)MsgGotCoins);
    Party_AdjustCoinsFar(spoils->coins);
    BattleMessage_WaitForAdvance();
  }
  found = &Data_0200049c;
  for (;;) {
    bestSlot = -1;
    best = -1;
    for (slot = 0; slot <= 3; slot++) {
      if (spoils->items[slot] != 0) {
        value = BattleSpoils_RankItem(spoils->items[slot]);
        if (value >= best) {
          best = value;
          bestSlot = slot;
        }
      }
    }
    if (bestSlot == -1)
      break;
    MessageArgument_PushFar(spoils->items[bestSlot], 2);
    Message_ShowAndWaitCoreFar((s32)MsgGotItem);
    BattleMessage_WaitForAdvance();
    if (PartyInventory_AddFar(spoils->items[bestSlot]) == -1) {
      *found = spoils->items[bestSlot];
      break;
    }
    spoils->items[bestSlot] = 0;
  }
}
