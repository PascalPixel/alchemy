/// Lifts an actor out of its cell and opens the cells it stood on.
void Actor_RaiseFromCell(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = 4;
  height = object->groundHeight;
  object->bounce = 0;
  object->y.fixed = height + 0x200000;
  object->priorityFlags |= 128;
  height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
  MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0,
                        (height >> 19) + 6);
  MapGrid_SetAttributeBAt(object->x.fixed, object->z.fixed, 0, 128);
}
