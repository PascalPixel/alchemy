#include "types.h"
#include "fieldobj.h"
#include "field_event.h"

static s32 SceneEffect_SetModeByFrameBit1(s32 arg0)
{
	if ((gFrameCount >> 1) & 1)
	{
		ObjectGroup_SetChildValue(arg0, 10);
	}
	else
	{
		ObjectGroup_SetChildValue(arg0, 7);
	}
	return 0;
}
