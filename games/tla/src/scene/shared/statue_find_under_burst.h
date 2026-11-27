/* Looks up the metatile Burst is cast at in an (actor, metatile, value) table
   ended by ACTOR_LIST_END: on a match, makes the row's statue the target and
   returns its value. */
Sint32 scStatueFindUnderBurst(Uint16 *tbl)
{
	FIELD_EFFECT_STATE *watch = Ram_HeapSlots->fieldEffState;
	Sint32				result = 0;
	Sint32 metatile = fdMapMetatileGetAttributes(MAPGRID_LAYER_GROUND,
						  FIELD_METATILE(watch->x), FIELD_METATILE(watch->z)) >>
		8;
	Uint32 id;

	for(; *tbl != ACTOR_LIST_END; result = *tbl++) {
		id = *tbl++;
		if(metatile == *tbl++) {
			watch->tgt = fdActorGet(id);
			result = *tbl;
			break;
		}
	}
	return result;
}
