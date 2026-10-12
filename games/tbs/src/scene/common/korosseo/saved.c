#include "types.h"
#include "field_event.h"

void SceneActor_PlacePartyAtSavedTiles(void)
{
	{
		s32 x = GameFlag_GetByte(896);
		s32 y = GameFlag_GetByte(904);

		x <<= 20;
		x += 0x80000;
		y <<= 20;
		y += 0x80000;
		Engine_ActorSetPosition(GERALD, x, y);
	}
	{
		s32 x = GameFlag_GetByte(912);
		s32 y = GameFlag_GetByte(920);

		x <<= 20;
		x += 0x80000;
		y <<= 20;
		y += 0x80000;
		Engine_ActorSetPosition(IWAN, x, y);
	}
	{
		s32 x = GameFlag_GetByte(928);
		s32 y = GameFlag_GetByte(936);

		x <<= 20;
		x += 0x80000;
		y <<= 20;
		y += 0x80000;
		Engine_ActorSetPosition(MEARI, x, y);
	}
}
