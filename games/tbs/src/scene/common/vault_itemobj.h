/// @file
/// Vault's interiors, before and after the thieves are caught: an item lying on
/// the floor, drawn as its menu icon (the Bone, at (568, 16, 672)).
#include "types.h"
#include "actor.h"
#include "script_layer.h"

/// Creates object 22 at the 16.16 position and draws it as the item's icon: the
/// sprite's own animation is stopped (flags and count 0, 16-colour, palette 0),
/// the object stays put (motionFlags 0), and the icon's tiles, decoded into
/// heap block 17, are loaded into the sprite's VRAM block.
void Vault_PlaceItemIcon(s32 item, s32 fixed_x, s32 fixed_y, s32 fixed_z) {
  u8 *object;
  u8 *sprite;
  u8 *field;
  u8 *icon;
  s32 zero;
  s32 mask;

  zero = 0;
  object = (u8 *)Object_CreateField(SPRITE_BLANK, fixed_x, fixed_y, fixed_z);
  if (object != 0) {
    sprite = (u8 *)((struct Actor *)object)->sprite;
    field = sprite + 38; // flags, then count
    *field = zero;
    field += 1;
    *field = zero;
    mask = 33;
    mask = -mask; // ~32: image.fullColor
    sprite[5] &= mask;
    sprite[9] &= 15;     // image.palette
    object[0x55] = zero; // motionFlags
    object[0x5c] = 1;
    icon = AllocHeapSlot(HEAP_SLOT_GLYPH_TRANSFER, 0x608);
    Icon_DrawItem(item);
    icon += 0x400;
    VramCache_Load(sprite[28], 0x80, icon); // sprite->slot
    FreeHeapSlot(HEAP_SLOT_GLYPH_TRANSFER);
  }
}
