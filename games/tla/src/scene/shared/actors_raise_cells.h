/// The actor id that ends a list of actors or of (actor, kind, flag) entries.
#define ACTOR_LIST_END 0xffff

/// Raises the cells of a list's actors.
void MagmaRockInside_RaiseActorCells(u16 *list) {
  for (; *list != ACTOR_LIST_END; list += 3) {
    struct Actor *actor = Actor_Get(*list);
    s32 height =
        Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);

    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    MagmaRockInside_SetCellShape(actor->layer, FIELD_CELL(actor->x.fixed),
                                 FIELD_CELL(actor->z.fixed),
                                 (height >> 19) + 4);
  }
}
