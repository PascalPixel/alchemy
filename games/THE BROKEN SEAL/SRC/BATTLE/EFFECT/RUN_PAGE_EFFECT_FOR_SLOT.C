#include "TYPES.H"
#include "GLOBAL_CELLS.H"

struct MenuState {
    u8 padding_000[20];
    s32 slots[4];
    u8 padding_024[414 - 36];
    s16 mode;
};

s32 GameFlag_TestFar(s32);
void GameFlag_SetBitFar(void *);
s32 Djinn_AddToLeastLoadedOwnerFar(s32, void *);
void Battle_Reset(void);
void Battle_SetObjectFlag5bWhenMode3(void);
void BattleEffect_ClearOutOfBoundsObjects(void);
void BattleFx_RunVenusDjinnCapture(s32);
void BattleFx_RunMercuryDjinnCapture(s32);
void BattleEffect_RunPhasedRadialParticleSequence(s32);
void BattleFx_FinishHeavyImpact(s32);
void BattleEffect_ClearAllObjects(void);
void Djinn_ShowJoinedMessageFar(s32, s32, void *);
void Battle_ClearObjectFlag5bWhenMode3(void);
void BattleFx_FinishAction(void);

void BattleFx_RunPageEffectForSlot(s32 slot, s32 page, void *entries)
{
    struct MenuState *state;
    s32 selection;

    state = *(struct MenuState **)ADDR_03001EBC;
    if (GameFlag_TestFar(366) != 0) {
        selection = 0;
        GameFlag_SetBitFar((u8 *)entries + page * 20 + 48);
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
                BattleFx_RunVenusDjinnCapture(slot);
            else if (page == 1)
                BattleFx_RunMercuryDjinnCapture(slot);
            else if (page == 2)
                BattleEffect_RunPhasedRadialParticleSequence(slot);
            else if (page == 3)
                BattleFx_FinishHeavyImpact(slot);

            state->slots[slot] = 0;
            if (state->mode == 3)
                BattleEffect_ClearAllObjects();
        }

        Djinn_ShowJoinedMessageFar(selection, page, entries);
        Battle_ClearObjectFlag5bWhenMode3();
        BattleFx_FinishAction();
    }
}
