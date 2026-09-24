#include "TYPES.H"
#include "BATTLE_MSG.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_INTRO.H"

extern u8 *Data_03001e74;

void UiWork_ClearValueNameTablesFar(void);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWait(s32);
void UiWork_FinalizeSharedSlotFar(void);
void BattlePresentation_WaitForAdvance(void);

void BattleIntro_AnnounceEncounter(s32 enemy_count)
{
    s16 enemies[8];
    u8 *battle_state;
    s16 *enemy;
    s32 announced;

    battle_state = Data_03001e74;
    UiWork_ClearValueNameTablesFar();
    BattleParty_ListPresentEnemies(enemies);

    announced = 0;
    if (enemy_count != 0) {
        enemy = enemies;
        do {
            UiText_DrawQuantity((u16)*enemy++, 1);
            if (announced == enemy_count - 1)
                UiText_ShowMessageAndWait((s32)&Value_00000811);
            else
                UiText_ShowMessageAndWait((s32)&Value_00000810);
            announced++;
            BattlePresentation_WaitForAdvance();
        } while (announced != enemy_count);
    }

    UiWork_FinalizeSharedSlotFar();
    if (battle_state[69] == BATTLE_ENCOUNTER_PARTY_FIRST) {
        UiWork_ClearValueNameTablesFar();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000812);
        BattlePresentation_WaitForAdvance();
    } else if (battle_state[69] == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        UiWork_ClearValueNameTablesFar();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000813);
        BattlePresentation_WaitForAdvance();
    }
}
