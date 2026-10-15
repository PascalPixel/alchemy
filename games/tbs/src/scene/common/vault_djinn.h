#include "types.h"
#include "field_obj.h"
#include "field_event.h"

static s32 OverlayObject_RunObject2WhenFlagged(void)
{
	BattlePlacement_UpdateTimedEntriesTwentyTimes();
	if ((UnitGet(2)->djinn_available[0] & 1) != 0)
	{
		Djinn_Transfer(2, 0, 0, 0);
		Engine_AudioPlayCue(126);
		UnitCalc(0);
		UnitCalc(2);
	}
}
