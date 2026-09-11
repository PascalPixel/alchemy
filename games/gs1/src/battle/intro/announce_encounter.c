#include "types.h"
#include "scene.h"
#include "abi/battle/intro/announce_encounter.h"
#include "battle_msg.h"
#include "battle_party.h"
#include "battle_intro.h"

extern u8 *gBattleWork;

void UiText_DrawQuantity(s32, s32);
void UiText_ShowMessageAndWait(s32);

void BattleIntro_AnnounceEncounter(s32 enemy_count)
{
    s16 enemies[8];
    u8 *battle_state;
    s16 *enemy;
    s32 announced;

    battle_state = gBattleWork;
    Battle_Run();
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
            Battle_Run2();
        } while (announced != enemy_count);
    }

    Battle_Run3();
    if (battle_state[69] == BATTLE_ENCOUNTER_PARTY_FIRST) {
        Battle_Run();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000812);
        Battle_Run2();
    } else if (battle_state[69] == BATTLE_ENCOUNTER_ENEMIES_FIRST) {
        Battle_Run();
        UiText_DrawQuantity(0, 1);
        UiText_ShowMessageAndWait((s32)&Value_00000813);
        Battle_Run2();
    }
}
