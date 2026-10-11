#include "edition.h"
#include "keystate.h"
#include "types.h"
#include "io_reg.h"
#include "scene.h"
#include "btlwork.h"


void WaitFrames(s32);

#if EDITION_INTERNATIONAL
#define DEBUG_SELECT_VALUE 0x18f
#else
#define DEBUG_SELECT_VALUE 480
#endif

s32 Runtime_AdjustDebugValueWithButtons(s32 ret)
{
	struct BtlWork *battle;
	volatile u32   *keys;

	if (gKeysHeld & KEY_START)
	{
		keys = &gKeysRepeat;
	loop:
		battle = BtlWk;
		if (*keys & 0x20)
			battle->debug_value -= 1;
		if (*keys & 0x10)
			battle->debug_value += 1;
		if (*keys & 0x40)
			battle->debug_value -= 100;
		if (*keys & 0x80)
			battle->debug_value += 100;
		if (gKeyState & KEY_A)
		{
			ret = battle->debug_value;
			goto done;
		}
		WaitFrames(1);
		goto loop;
	}
done:
	if (gKeysHeld & KEY_SELECT)
		ret = DEBUG_SELECT_VALUE;
	return ret;
}
