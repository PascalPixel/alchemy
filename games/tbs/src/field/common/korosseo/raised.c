#include "types.h"
#include "fieldobj.h"
#include "game_state.h"
#include "field_event.h"
#include "animspr.h"

void SceneState_RaiseTriggerOnPsynergyTarget(void)
{
	struct EventWork *work = gEventWork;
	s32				  sel = gGameState.selected_actor;

	if (sel != 0 && (work->psynergy_request >> 10) == sel && Engine_GameFlagIsSet(321) != 0)
	{
		u16 *p = (u16 *)&work->raised_trigger;
		s32	 val = 99;

		*p = val;
	}
}

static void EV99_RAISED(void)
{

	u8				  *state = (u8 *)gEventWork;
	s32				   best = 8;
	s32				   bestd = 0x100000;
	s32				   n = gGameState.selected_actor;
	struct FieldActor *p = Object_GetById(n);
	s32				   i;
	s32				  *q;
	s32				   base;

	Engine_EventBegin();
	for (i = 8; i <= 66; i++)
	{
		struct FieldActor *o = Object_GetById(i);

		if (o != 0 && o->active == 1
			&& ((struct AnimationObject *)o->sprite)->entries[0]->anim_id == 165)
		{
			s32 dx = (p->x.fixed - o->x.fixed) / 65536;
			s32 dy = (p->z.fixed - o->z.fixed) / 65536;

			if (dy <= 0)
			{
				s32 a = dx;
				s32 d;

				if (a < 0)
					a = -a;
				if (dy < 0)
					dy = -dy;
				d = a + dy;
				if (d < bestd)
				{
					best = i;
					bestd = d;
				}
			}
		}
	}
	Engine_EventSetMessage((s32)MsgKorosseoMatchAboutBeginPleaseTake);
	Engine_EventShowMessage(best, 0);
	q = &((struct EventWork *)state)->start_transition;
	*q = 512;
	((struct EventWork *)state)->transition_frames = 15;
	Engine_EventWait(20);
	Engine_EventCloseScreen();
	Engine_EventWaitForScreen();
	base = n << 4;
	GameFlag_SetByte(base + 880, p->x.fixed >> 20);
	{
		s32 v = p->z.fixed >> 20;

		GameFlag_SetByte(base + 888, v);
	}
	n++;
	if (n > 3)
	{
		Engine_EventRequestExit(10);
		Engine_GameFlagSet(282);
	}
	else
	{
		Korosseo_SelectSoloCompetitor(n);
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		*q = 0;
	}
	Engine_EventEnd();
}
