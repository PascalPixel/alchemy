/* Looks up the cell Burst is cast at in an (actor, cell, value) table ended by
 */
/* ACTOR_LIST_END: on a match, makes the row's statue the target and returns */
/* its value. */
s32 syStatueFindUnderBurst(u16 *table) {
	FIELD_EFFECT_STATE *watch = Ram_HeapSlots->fieldEffState;
	s32 result = 0;
	s32 cell =
		fdMapCellGetAttributes(MAPGRID_LAYER_GROUND, FIELD_CELL(watch->x),
							   FIELD_CELL(watch->z)) >>
		8;
	u32 id;

	for(; *table != ACTOR_LIST_END; result = *table++) {
		id = *table++;
		if(cell == *table++) {
			watch->tgt = fdActorGet(id);
			result = *table;
			break;
		}
	}
	return result;
}
