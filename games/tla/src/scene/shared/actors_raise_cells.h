/// For each statue of a list of (statue, cell, cap) rows ended by
/// ACTOR_LIST_END: holds it still, without a shadow, and raises the cell it
/// stands on 32 pixels above the ground there, so it stands as a block.
void MagmaRockInside_RaiseActorCells(u16 *list) {
  for (; *list != ACTOR_LIST_END; list += 3) {
    struct Actor *actor = Actor_Get(*list);
    s32 height =
        Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);

    actor->motionFlags = 0;
    actor->collisionFlags = 0;
    Actor_SetSpriteFlags(actor, 0);
    MagmaRockInside_SetCellShape(
        actor->layer, FIELD_CELL(actor->x.fixed), FIELD_CELL(actor->z.fixed),
        CELL_HEIGHT_STEPS(height) + CELL_HEIGHT_STEPS(toFIXED(32)));
  }
}
