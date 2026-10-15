#include "types.h"
#include "btlwork.h"
#include "text/msg_ids.h"

s32 BtlEnmList(s16 *unit_ids);
void UiWork_ClearValueNameTablesFar(void);
void UiWork_FinalizeSharedSlotFar(void);
void UiText_DrawQuantity(s32 value, s32 slot);
void UiText_ShowMessageAndWaitCoreFar(s32 message);
void BattlePresentation_WaitForAdvance(void);

// Announces each enemy that appears, then who strikes first. The Broken
// Seal's BattleIntro_AnnounceEncounter.
void Func_08118bcc(s32 enemy_count)
{
    s16 enemies[8];
    struct BtlWork *battle;
    s16 *enemy;
    s32 announced;

    battle = Ram_HeapSlots->battle_work;
    UiWork_ClearValueNameTablesFar();
    BtlEnmList(enemies);

    announced = 0;
    if (enemy_count != 0) {
        enemy = enemies;
        do {
            UiText_DrawQuantity((u16)*enemy++, 1);
            if (announced == enemy_count - 1)
                UiText_ShowMessageAndWaitCoreFar((s32)MsgLastEnemyAppeared);
            else
                UiText_ShowMessageAndWaitCoreFar((s32)MsgEnemyAppeared);
            announced++;
            BattlePresentation_WaitForAdvance();
        } while (announced != enemy_count);
    }

    UiWork_FinalizeSharedSlotFar();
    if (battle->encounter_mode == 1) {
        UiWork_ClearValueNameTablesFar();
        UiText_ShowMessageAndWaitCoreFar((s32)MsgPartyStrikesFirst);
        BattlePresentation_WaitForAdvance();
    } else if (battle->encounter_mode == 2) {
        UiWork_ClearValueNameTablesFar();
        UiText_ShowMessageAndWaitCoreFar((s32)MsgPartySurprised);
        BattlePresentation_WaitForAdvance();
    }
}
