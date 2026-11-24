/// @file
/// The three Colosseum finals matches, and three of Crossbone Isle Cave's side
/// rooms (CrossboneIsleKeys, CrossboneIsleStatues and CrossboneIsleBane): an
/// actor drawn as an item's menu icon, such as the two prizes at a
/// finals match's arena or the keys in CrossboneIsleKeys.

/// Draws actor id, when it has a sprite of its own, as the menu icon of item,
/// held still in place of its animation.
void Colosso_ShowItemIcon(s32 id, s32 item) {
  struct Actor *actor;
  struct Sprite *sprite;
  IconTransfer *icon;
  s32 tile;

  actor = Actor_GetById(id);
  if (actor != NULL && actor->spriteStorageKind == ACTOR_SPRITE_ONE) {
    sprite = actor->sprite;
    icon = HeapSlot_Alloc(HEAP_SLOT_ICON_TRANSFER, sizeof(IconTransfer));
    Dma_Fill32(0, icon->tiles, ICON_TILE_BYTES);
    Icon_DrawItem(item);
    tile = VramCache_Load(sprite->slot, ICON_TILE_BYTES, icon->tiles);
    HeapSlot_Release(HEAP_SLOT_ICON_TRANSFER);
    actor->retainSpriteCache = 1;
    SpriteAnimation_Clear(sprite->entries[0]);
    sprite->entries[0] = NULL;
    sprite->count = 0;
    sprite->image.fullColor = 0;
    sprite->image.tile = tile;
    sprite->dirty = 0;
    sprite->flags = 0;
  }
}
