/* For each statue of a list of (statue, cell, cap) rows ended by */
/* ACTOR_LIST_END: holds it still, without a shadow, and raises the cell it */
/* stands on 32 pixels above the ground there, so it stands as a block. */
void scMagmaRockInsideRaiseActorCells(u16 *list) {
	for(; *list != ACTOR_LIST_END; list += 3) {
		ACTOR *actor = fdActorGet(*list);
		s32 height =
			fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);

		actor->motionFlg = 0;
		actor->collisionFlg = 0;
		fdActorSetSprFlags(actor, 0);
		scMagmaRockInsideSetCellShape(actor->layer, FIELD_CELL(actor->x.fixed),
									  FIELD_CELL(actor->z.fixed),
									  CELL_HEIGHT_STEPS(height) +
										  CELL_HEIGHT_STEPS(toFIXED(32)));
	}
}
