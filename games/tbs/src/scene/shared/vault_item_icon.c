/// @file
/// Vault's rooms, before and after the thieves are caught: an item lying on
/// the floor, drawn as its menu icon (the Bone, at (568, 16, 672)).

/// Creates a blank actor at the 16.16 position and draws it as the item's
/// icon: the sprite's own animation is stopped (flags and count 0,
/// 16-colour, palette 0), the actor stays put (motionFlags 0) and keeps its
/// VRAM, and the icon's tiles, decoded into heap block 17, are loaded into
/// the sprite's VRAM block.
void Vault_PlaceItemIcon(s32 item, s32 fixed_x, s32 fixed_y, s32 fixed_z) {
  struct Actor *actor;
  struct Sprite *sprite;
  GlyphTransfer *icon;
  u8 clear = 0;

  actor = Actor_Create(SPRITE_BLANK, fixed_x, fixed_y, fixed_z);
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
    VramCache_Load(sprite->slot, 128, icon->tiles);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
  }
}
