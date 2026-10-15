#include "types.h"
#include "fieldobj.h"
#include "field_event.h"

static void EV01_TOUCH(void)
{
	Engine_GameFlagSet(0x200);
	{
		Map_CopyCellAttributes(55, 26, 4, 2, 23, 26);
	}
}

static void EV02_TOUCH(void)
{
	Engine_GameFlagClear(0x200);
	{
		Map_CopyCellAttributes(23, 23, 4, 2, 23, 26);
	}
}
