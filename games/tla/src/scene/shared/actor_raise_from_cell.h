/* Lifts an actor 32 pixels over its ground as a platform others may stand on,
 */
/* and raises the map cell under it 48 pixels, a gap the leader jumps across */
/* onto it. */
void scActorRaiseFromCell(s32 id) {
	ACTOR *actor;
	s32 height;

	actor = fdActorGet(id);
	actor->motionFlg = ACTOR_FLOATS;
	height = actor->groundHeight;
	actor->bounce = 0;
	actor->y.fixed = height + toFIXED(32);
	actor->priFlg |= ACTOR_PRIORITY_PLATFORM;
	height =
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
	fdMapGridSetCellHeight(actor->x.fixed, actor->z.fixed, 0,
						   CELL_HEIGHT_STEPS(height) +
							   CELL_HEIGHT_STEPS(toFIXED(48)));
	fdMapGridSetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
								 MAP_CELL_JUMP);
}
