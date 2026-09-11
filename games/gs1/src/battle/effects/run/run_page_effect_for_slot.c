#include "types.h"
#include "scene.h"
#include "abi/battle/effects/run/run_page_effect_for_slot.h"
#include "global_cells.h"

struct MenuState {
    u8 padding_000[20];
    s32 slots[4];
    u8 padding_024[414 - 36];
    s16 mode;
};

s32 GameFlag_IsSet(s32);
void GameFlag_Set(void *);

void Battle_Reset(void);

void BattleFx_FinishHeavyImpact(s32);

void BattleFx_RunPageEffectForSlot(s32 slot, s32 page, void *entries)
{
    struct MenuState *state;
    s32 selection;

    state = *(struct MenuState **)ADDR_03001EBC;
    if (GameFlag_IsSet(366) != 0) {
        selection = 0;
        GameFlag_Set((u8 *)entries + page * 20 + 48);
    } else {
        selection = Battle_Apply(page, entries);
    }

    if (selection >= 0) {
        Battle_Reset();
        Battle_Run();
        if (slot != -1) {
            if (state->mode == 3)
                Battle_Run2();

            if (page == 0)
                Battle_Do(slot);
            else if (page == 1)
                Battle_Do2(slot);
            else if (page == 2)
                Battle_Do3(slot);
            else if (page == 3)
                BattleFx_FinishHeavyImpact(slot);

            state->slots[slot] = 0;
            if (state->mode == 3)
                Battle_Run3();
        }

        Battle_Place(selection, page, entries);
        Battle_Run4();
        Battle_Run5();
    }
}
