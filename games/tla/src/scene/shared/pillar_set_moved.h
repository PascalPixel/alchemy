/// Marks a pillar moved: it stops and its cell moves.
void GondowanCliffs_FreezeFirstPuddle(void) {
  struct Actor *object = Actor_Get(12);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  MapGrid_CopyCellAttributes(79, 28, 1, 1, 81, 28);
  Event_SetFlag(FLAG_FIRST_PUDDLE_FROZEN);
}
