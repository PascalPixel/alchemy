/// Raises the cells of a list's actors.
void MagmaRockInside_RaiseActorCells(u16 *list) {
  for (; *list != 0xffff; list += 3) {
    struct Actor *object = Actor_Get(*list);
    s32 height =
        Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);

    object->motionFlags = 0;
    object->collisionFlags = 0;
    Actor_SetSpriteFlags(object, 0);
    MagmaRockInside_SetCellShape(object->layer, object->x.fixed >> 20,
                                 object->z.fixed >> 20, (height >> 19) + 4);
  }
}
