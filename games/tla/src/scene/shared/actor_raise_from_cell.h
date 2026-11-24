/// Lifts an actor 32 pixels over its ground as a platform others may stand on,
/// and raises the map cell under it 48 pixels, a gap the leader jumps across
/// onto it.
void Actor_RaiseFromCell(s32 id) {
  struct Actor *actor;
  s32 height;

  actor = Actor_Get(id);
  actor->motionFlags = ACTOR_FLOATS;
  height = actor->groundHeight;
  actor->bounce = 0;
  actor->y.fixed = height + toFIXED(32);
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                        CELL_HEIGHT_STEPS(height) +
                            CELL_HEIGHT_STEPS(toFIXED(48)));
  MapGrid_SetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0, MAP_CELL_JUMP);
}
