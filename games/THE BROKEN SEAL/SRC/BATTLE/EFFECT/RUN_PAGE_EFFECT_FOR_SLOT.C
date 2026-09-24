#include "TYPES.H"
#include "GLOBAL_CELLS.H"

struct MenuState {
    u8 padding_000[20];
    s32 slots[4];
    u8 padding_024[414 - 36];
    s16 mode;
};

s32 GameFlag_IsSet(s32);
void GameFlag_Set(void *);
s32 Djinn_AddToLeastLoadedOwnerFar(s32, void *);
void Battle_Reset(void);
void Battle_SetObjectFlag5bWhenMode3(void);
void BattleEffect_ClearOutOfBoundsObjects(void);
void Func_08096140(s32);
void Func_080965a8(s32);
void BattleEffect_RunPhasedRadialParticleSequence(s32);
void BattleFx_FinishHeavyImpact(s32);
void Func_0808b98c(void);
void Func_08015358(s32, s32, void *);
void Battle_ClearObjectFlag5bWhenMode3(void);
void BattleFx_FinishAction(void);

void BattleFx_RunPageEffectForSlot(s32 slot, s32 page, void *entries)
{
    struct MenuState *state;
    s32 selection;

    state = *(struct MenuState **)ADDR_03001EBC;
    if (GameFlag_IsSet(366) != 0) {
        selection = 0;
        GameFlag_Set((u8 *)entries + page * 20 + 48);
    } else {
        selection = Djinn_AddToLeastLoadedOwnerFar(page, entries);
    }

    if (selection >= 0) {
        Battle_Reset();
        Battle_SetObjectFlag5bWhenMode3();
        if (slot != -1) {
            if (state->mode == 3)
                BattleEffect_ClearOutOfBoundsObjects();

            if (page == 0)
                Func_08096140(slot);
            else if (page == 1)
                Func_080965a8(slot);
            else if (page == 2)
                BattleEffect_RunPhasedRadialParticleSequence(slot);
            else if (page == 3)
                BattleFx_FinishHeavyImpact(slot);

            state->slots[slot] = 0;
            if (state->mode == 3)
                Func_0808b98c();
        }

        Func_08015358(selection, page, entries);
        Battle_ClearObjectFlag5bWhenMode3();
        BattleFx_FinishAction();
    }
}
