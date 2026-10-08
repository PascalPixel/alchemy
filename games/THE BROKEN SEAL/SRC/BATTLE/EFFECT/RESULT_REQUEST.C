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

struct ResultWork
{
	u8	unknown_000[0x1d6];
	u16 special;
	u8	unknown_1d8[0x1c];
	u32 object_id;
	u8	unknown_1f8[0x3c];
	u16 request;
};

extern struct ResultWork gGameState;

void BattleFx_SetWeightedResult(s32 arg0, s32 arg1)
{
	register s32				  first = arg0;
	register s32				  second = arg1;
	register struct EventRuntime *runtime = gEventWork;

	runtime->value_17c = BattleFx_GetWeightedResult(first, second);
	if (first == 98 && second == 0)
		gGameState.special = (s32)&ResourceId_VinasuChojoStormBackdrop;
	if (runtime->mode_19e == 3)
		BattleFx_LookupResult((u8 *)ObjectTable_Get(gGameState.object_id) + 8);
	BattleFx_SelectBattleCue(first, second);
}

void BattleFx_SetPhaseRequest(s32 flags, s32 value)
{
	struct EventRuntime *state;
	struct ResultWork	*shared;
	s32					 high;

	state = gEventWork;
	high = flags & 0x800;
	flags &= 0xff;

	if (high == 0)
		EventActor_RunHopAndLeapSequence(flags);

	shared = &gGameState;
	shared->request = (value + 0x12c) | high;
	state->value_17c = BattleFx_GetPhaseResult(value);
	if (state->mode_19e == 3)
	{
		void *object = ObjectTable_Get(shared->object_id);

		BattleFx_LookupResult((u8 *)object + 8);
	}
	BattleFx_SelectBattleCue(0, 0);
}
