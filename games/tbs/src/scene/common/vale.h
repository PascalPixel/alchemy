#include "types.h"
#include "fieldobj.h"
#include "field_event.h"

static void Vale_RequestBackdropExit(s32 no)
{
	struct EventWork *p;
	if (Engine_GameFlagIsSet(0x834) != 0)
	{
		BattleFx_SetBlock30ValuesMaxZero();
	}
	p = gEventWork;
	p->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	p->transition_frames = 16;
	Engine_EventRequestExit(no);
}
