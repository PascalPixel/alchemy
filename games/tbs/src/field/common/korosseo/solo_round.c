#include "types.h"
#include "call.h"
#include "fieldobj.h"
#include "text/msg_ids.h"
#include "field_event.h"
#include "game_state.h"

void GameFlag_SetByte(s32, s32);
void Korosseo_SelectSoloCompetitor(s32 id);

void Korosseo_FinishSoloRound(s32 id)
{
	struct EventWork  *work;
	s32				   slot;
	struct FieldActor *obj;
	s32				   msg;
	s32				   res;

	work = gEventWork;
	// 話者を二度引き、どちらの結果も使わない (original behaviour: two real calls)
	Object_GetById(id);
	Object_GetById(id);
	slot = gGameState.selected_actor;
	obj = Object_GetById(slot);
	Engine_EventBegin();
	msg = (s32)MsgKorosseoCheering;
	Engine_EventSetMessage(msg);
	Engine_EventOpenMessage(id, 0);
	gEventWork->choice_message = (s32)MsgKorosseoCheeringChoices;
	gEventWork->unknown_cc4 = 4;
	res = Engine_EventChooseYesNo(slot, 0);
	if (res == 0)
	{
		Engine_EventSetMessage(msg + 1);
		Engine_EventShowMessage(id, 0);
		work->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		work->transition_frames = 15;
		Engine_EventCloseScreen();
		Engine_EventWaitForScreen();
		Call2(GameFlag_SetByte, (slot << 4) + 0x370, obj->x.fixed >> 20);
		Call2(GameFlag_SetByte, (slot << 4) + 0x378, obj->z.fixed >> 20);
		slot++;
		if (slot > 3)
		{
			Engine_EventRequestExit(10);
			Engine_GameFlagSet(0x11a);
		}
		else
		{
			Korosseo_SelectSoloCompetitor(slot);
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
			work->start_transition = 0;
		}
	}
	else
	{
		Engine_EventSetMessage(msg + 2);
		Engine_EventShowMessage(id, 0);
	}
	Engine_EventEnd();
}
