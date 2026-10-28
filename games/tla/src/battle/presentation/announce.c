#include "types.h"
#include "battle_work.h"
#include "text/msg_ids.h"

s32 BattleEnemy_List(s16 *unit_ids);
void MessageArgument_ClearAllFar(void);
void Window_CloseSharedFar(void);
void MessageArgument_PushFar(s32 value, s32 slot);
void Message_ShowAndWaitCoreFar(s32 message);
void BattlePresentation_WaitForAdvance(void);

/// Announces each enemy that appears, then who strikes first. The Broken
/// Seal's BattleIntro_AnnounceEncounter.
void Func_08118bcc(s32 enemyCount) {
  s16 enemies[8];
  struct BattleWork *battle;
  s16 *enemy;
  s32 announced;

  battle = Ram_HeapSlots->battleWork;
  MessageArgument_ClearAllFar();
  BattleEnemy_List(enemies);

  announced = 0;
  if (enemyCount != 0) {
    enemy = enemies;
    do {
      MessageArgument_PushFar((u16)*enemy++, 1);
      if (announced == enemyCount - 1)
        Message_ShowAndWaitCoreFar((s32)MsgLastEnemyAppeared);
      else
        Message_ShowAndWaitCoreFar((s32)MsgEnemyAppeared);
      announced++;
      BattlePresentation_WaitForAdvance();
    } while (announced != enemyCount);
  }

  Window_CloseSharedFar();
  if (battle->encounterMode == 1) {
    MessageArgument_ClearAllFar();
    Message_ShowAndWaitCoreFar((s32)MsgPartyStrikesFirst);
    BattlePresentation_WaitForAdvance();
  } else if (battle->encounterMode == 2) {
    MessageArgument_ClearAllFar();
    Message_ShowAndWaitCoreFar((s32)MsgPartySurprised);
    BattlePresentation_WaitForAdvance();
  }
}
