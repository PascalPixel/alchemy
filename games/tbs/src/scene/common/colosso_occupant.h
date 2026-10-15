#include "types.h"
#include "field_obj.h"
#include "game_state.h"

void *SceneActor_FindOccupantAheadOfSubject(void)
{
	struct FieldActor *record;
	s32				   facing;
	s32				   position[3];
	s32				  *occupant;

	record = Object_GetById(gGameState.selected_actor);

	facing = (record->facing + 0x2000) & 0xc000;

	position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
	position[1] = record->y.fixed;
	position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
	Vector_AddPolarOffset(0x100000, facing, position);

	occupant = SceneData_FindSlotAtPosition(position, record);
	if (occupant == 0)
	{
		position[0] = (record->x.fixed & 0xfff00000) + 0x80000;
		position[1] = record->y.fixed;
		position[2] = (record->z.fixed & 0xfff00000) + 0x80000;
		Vector_AddPolarOffset(0x200000, facing, position);

		occupant = SceneData_FindSlotAtPosition(position, record);
	}

	return occupant;
}
