/// Resets a tile sprite to the map's tile, frame 0.
s32 TileSprite_Reset(struct Actor *object) {
  u16 *tile = (u16 *)((u8 *)Ram_HeapSlots->mapGrid + 286);
  struct FieldSprite *sprite = object->sprite;

  sprite->rotation = *tile;
  sprite->flags = 0;
  return 1;
}
