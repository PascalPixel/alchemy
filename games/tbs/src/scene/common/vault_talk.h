#include "types.h"
#include "field_obj.h"
#include "field_event.h"

static void SceneActor_RunActorCommandWithFlag91(s32 x)
{
	u8 *flag = &Object_GetById(x)->paused;
	s32 zero = 0;

	*flag = 1;
	Engine_EventBegin();
	Engine_ActorSetAnimation(x, 1);
	Engine_EventWait(2);
	Engine_EventShowMessage(x, 0);
	Engine_EventEnd();
	*flag = zero;
}
