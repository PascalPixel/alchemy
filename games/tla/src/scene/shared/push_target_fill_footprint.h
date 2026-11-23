/// Makes the cells under a pushable actor solid, by its footprint: its sprite
/// picks one of the footprints in gPushFootprints, and the cells' own
/// behaviors are kept in the map's spare area for when it moves. Returns 1, or
/// 0 for a sprite that cannot be pushed.
s32 PushTarget_FillFootprint(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_Get(id);
  s16 shape = ((struct Sprite *)actor->sprite)->entries[0]->animationId;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gPushKinds[(u16)i]) {
    do {
      j = (u16)NEXT_HALFWORD(i);
      i = j;
    } while (j <= PUSH_KINDS - 1 && (u16)shape != gPushKinds[j]);
  }
  j = (u16)i;
  if (j == PUSH_KINDS)
    return 0;
  width = (ABS(gPushFootprints[j][0]) + ABS(gPushFootprints[j][2])) >> 4;
  height = (ABS(gPushFootprints[j][1]) + ABS(gPushFootprints[j][3])) >> 4;
  x = FIELD_CELL(actor->x.fixed + toFIXED(gPushFootprints[j][0]));
  z = FIELD_CELL(actor->z.fixed + toFIXED(gPushFootprints[j][1]));
  mapX = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetX);
  i = mapX;
  mapZ = FIELD_CELL(map->layers[MAPGRID_LAYER_MIDDLE].offsetY);
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_GROUND, x, z, width,
                                         height, CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(MAPGRID_LAYER_OVERHEAD, x, z, width,
                                         height, CELL_BEHAVIOR_WALL);
  return 1;
}
