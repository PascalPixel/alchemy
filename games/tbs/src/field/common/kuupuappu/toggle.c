#include "types.h"
#include "fieldobj.h"
#include "field_event.h"

static void EV01_TOUCH(void)
{
	Engine_GameFlagSet(0x200);
	{
		int v1 = 23;
		int v2 = 26;
		Engine_MapCopyCellAttributes(55, 26, 4, 2, v1, v2);
	}
}

static void EV02_TOUCH(void)
{
	Engine_GameFlagClear(0x200);
	{
		int v1 = 23;
		int v2 = 26;
		Engine_MapCopyCellAttributes(23, 23, 4, 2, v1, v2);
	}
}
