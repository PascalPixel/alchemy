#include "types.h"

void *SceneData_FindSlotAtPosition(void *position, void *self)
{
	s32	 *pos = position;
	s32 **slots = (s32 **)((u8 *)gEventWork + 0x14);
	u32	  i;

	for (i = 8; i <= 65; i++)
	{
		s32 *p = slots[i];

		if ((pos[0] >> 20) == (p[2] >> 20) && (pos[1] >> 20) == (p[3] >> 20)
			&& (pos[2] >> 20) == (p[4] >> 20))
		{
			return p;
		}
	}
	return 0;
}
