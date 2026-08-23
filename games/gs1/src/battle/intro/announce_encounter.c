#include "types.h"
#include "battle_msg.h"
#include "battle_party.h"
#include "battle_intro.h"

extern u8 *Data_03001e74;

void Func_08015118(void);
void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWait(s32);
void Func_08015218(void);
s32 Func_080bb65c(void);

void BattleIntro_AnnounceEncounter(s32 enemy_count)
{
    s16 enemies[8];
    u8 *battle_state;
    s16 *enemy;
    s32 announced;

    battle_state = Data_03001e74;
    Func_08015118();
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
            Func_080bb65c();
        } while (announced != enemy_count);
    }

    Func_08015218();
    if (battle_state[69] == BATTLE_ENCOUNTER_PARTY_FIRST) {
        Func_08015118();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000812);
        Func_080bb65c();
    } else if (battle_state[69] == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        Func_08015118();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000813);
        Func_080bb65c();
    }
}
