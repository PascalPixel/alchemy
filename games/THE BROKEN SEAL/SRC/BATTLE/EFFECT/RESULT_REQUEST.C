#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_LOOKUP.H"
#include "RESOURCE_IDS.H"

#define BattleFx_GetPhaseResult ((s16 (*)(s32))BattleFx_GetPhaseResult)
extern struct EventRuntime *gEventWork;

struct ResultWork {
    u8 unknown_000[470];
    u16 special;
    u8 unknown_1d8[28];
    u32 object_id;
    u8 unknown_1f8[60];
    u16 request;
};

extern struct ResultWork gGameState;

void BattleFx_SetWeightedResult(s32 arg0, s32 arg1)
{
    register s32 first = arg0;
    register s32 second = arg1;
    register struct EventRuntime *runtime = gEventWork;

    runtime->value_17c = BattleFx_GetWeightedResult(first, second);
    if (first == 98 && second == 0)
        gGameState.special = (s32)&ResourceId_VinasuChojoStormBackdrop;
    if (runtime->mode_19e == 3)
        BattleFx_LookupResult(ObjectTable_Get(gGameState.object_id) + 8);
    BattleFx_SelectBattleCue(first, second);
}

void BattleFx_SetPhaseRequest(s32 flags, s32 value)
{
    struct EventRuntime *state;
    struct ResultWork *shared;
    s32 high;

    state = gEventWork;
    high = flags & 0x800;
    flags &= 0xff;

    if (high == 0)
        EventActor_RunHopAndLeapSequence(flags);

    shared = &gGameState;
    shared->request = (value + 300) | high;
    state->value_17c = BattleFx_GetPhaseResult(value);
    if (state->mode_19e == 3) {
        void *object = ObjectTable_Get(shared->object_id);

        BattleFx_LookupResult(object + 8);
    }
    BattleFx_SelectBattleCue(0, 0);
}
