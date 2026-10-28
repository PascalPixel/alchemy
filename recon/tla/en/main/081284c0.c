// Draft, differ 21 of 308 (draft 307): the battle's spoils. Reports the
// experience, gives it to each living member (and half of it to the members
// Func_0811a24c adds, who did not fight), reports each level gained with its
// new psynergy and stat rises, then the coins and the dropped items, best
// first, keeping one that does not fit in Data_0200049c. ☀️'s Battle_AwardSpoils
// in games/tbs/src/battle/battle.c, rewritten from the listing with ⚓️'s calls
// (Unit_Get, MessageArgument_PushFar, Func_080ad0b0 for the level-up check) and
// numbered messages. Left (differ 21 of 308, all from one row): the native
// builds the gains' address before the member loop as movs r2, #48; add r2,
// sp, where this draft takes add r2, sp, #48, one instruction fewer, so every
// pool offset after it is 4 short. The unit list is ten halfwords (the
// frame's 64 bytes), and list declared before count gives their spill slots.
// Tried: gains first, gains inside the member loop (23).
#include "types.h"
#include "heap_release.h"
#include "iwram_call.h"
#include "battle_unit.h"
#include "battle_action.h"
#include "ram_buffer.h"

#define ACTION_ID_MASK OWNER_ACTION_ID_MASK

extern u8 Msg03206[]; // "You got N experience points." (MsgExpGained)
extern u8 Msg03207[]; // "You got N coins." (MsgCoinsGained)
extern u8 Msg03208[]; // "You got <item>." (MsgItemGained)
extern u8 Msg03322[]; // "<name> is now a level N ..." (MsgLevelUp)
extern u8 Msg03323[]; // "Mastered the <class>'s <ability>" (MsgAbilityMastered)
extern u8 Msg03324[]; // "Maximum HP rises by N!" (MsgMaxHpRises)
extern u8 Msg03325[]; // MsgMaxPpRises
extern u8 Msg03326[]; // MsgAttackRises
extern u8 Msg03327[]; // MsgDefenseRises
extern u8 Msg03328[]; // MsgAgilityRises
extern u8 Msg03329[]; // "Luck rises by N!" (MsgLuckRises)
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
    Message_ShowAndWaitCoreFar((s32)Msg03206);
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
      Message_ShowAndWaitCoreFar((s32)Msg03322);
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
            Message_ShowAndWaitCoreFar((s32)Msg03323);
            BattleMessage_WaitForAdvance();
          }
        }
      }
      if (gains[2] != 0) {
        MessageArgument_PushFar(gains[2], 5);
        Message_ShowAndWaitCoreFar((s32)Msg03324);
        BattleMessage_WaitForAdvance();
      }
      if (gains[3] != 0) {
        MessageArgument_PushFar(gains[3], 5);
        Message_ShowAndWaitCoreFar((s32)Msg03325);
        BattleMessage_WaitForAdvance();
      }
      if (gains[4] != 0) {
        MessageArgument_PushFar(gains[4], 5);
        Message_ShowAndWaitCoreFar((s32)Msg03326);
        BattleMessage_WaitForAdvance();
      }
      if (gains[5] != 0) {
        MessageArgument_PushFar(gains[5], 5);
        Message_ShowAndWaitCoreFar((s32)Msg03327);
        BattleMessage_WaitForAdvance();
      }
      if (gains[6] != 0) {
        MessageArgument_PushFar(gains[6], 5);
        Message_ShowAndWaitCoreFar((s32)Msg03328);
        BattleMessage_WaitForAdvance();
      }
      if (gains[7] != 0) {
        MessageArgument_PushFar(gains[7], 5);
        Message_ShowAndWaitCoreFar((s32)Msg03329);
        BattleMessage_WaitForAdvance();
      }
    }
  }
  Heap_Release(backup);
  if (spoils->coins != 0) {
    MessageArgument_PushFar(spoils->coins, 5);
    Message_ShowAndWaitCoreFar((s32)Msg03207);
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
    Message_ShowAndWaitCoreFar((s32)Msg03208);
    BattleMessage_WaitForAdvance();
    if (PartyInventory_AddFar(spoils->items[bestSlot]) == -1) {
      *found = spoils->items[bestSlot];
      break;
    }
    spoils->items[bestSlot] = 0;
  }
}
