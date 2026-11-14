/// Resets a tile sprite to the map's tile, frame 0.
s32 TileSprite_Reset(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapWork + 286);
  struct TileSprite *sprite = object->sprite;

  sprite->tile = *tile;
  sprite->frame = 0;
  return 1;
}
