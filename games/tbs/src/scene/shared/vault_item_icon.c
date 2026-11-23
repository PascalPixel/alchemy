/// @file
/// Vault's inn, before and after the thieves are caught: an item lying in the
/// room drawn as its menu icon (the Bone on the barrel, at (568, 16, 672)).

/// Places the item's icon, standing still, at the 16.16 position.
void Vault_PlaceItemIcon(s32 item, FIXED x, FIXED y, FIXED z) {
  struct Actor *actor;
  struct Sprite *sprite;
  GlyphTransfer *icon;
  u8 clear = 0;

  actor = Actor_Create(SPRITE_BLANK, x, y, z);
  if (actor != NULL) {
    sprite = actor->sprite;
    sprite->flags = clear;
    sprite->count = clear;
    sprite->image.fullColor = 0;
    sprite->image.palette = 0;
    actor->motionFlags = clear;
    actor->retainSpriteCache = 1;
    icon = HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer));
    Icon_DrawItem(item);
    VramCache_Load(sprite->slot, ICON_TILE_BYTES, icon->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}
