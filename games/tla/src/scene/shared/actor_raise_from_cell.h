/// Lifts an actor out of its cell and opens the cells it stood on.
void Actor_RaiseFromCell(s32 id) {
  struct Actor *actor;
  s32 height;

  actor = Actor_Get(id);
  actor->motionFlags = ACTOR_FLOATS;
  height = actor->groundHeight;
  actor->bounce = 0;
  actor->y.fixed = height + toFIXED(32);
  actor->priorityFlags |= 128;
  height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, (height >> 19) + 6);
  MapGrid_SetAttributeBAt(actor->x.fixed, actor->z.fixed, 0, 128);
}
