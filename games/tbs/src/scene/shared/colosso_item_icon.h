/// @file
/// The three Colosseum finals matches, and three of Crossbone Isle Cave's side
/// rooms (CrossboneIsleKeys, CrossboneIsleStatues and CrossboneIsleBane): an
/// actor drawn as an item's menu icon, such as the two prizes at a
/// finals match's arena or the keys in CrossboneIsleKeys.
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "battle_sprite_id.h"
#include "actor.h"
#include "event.h"
#include "sprite.h"
#include "heap_release.h"
#include "dma.h"
#include "io_reg.h"

/// Replaces the picture of actor id, when it has a sprite of its own,
/// with the icon of item: the icon's tiles, decoded into heap block 17, are
/// loaded into the sprite's VRAM block and its animation is stopped.
void Colosso_ShowItemIcon(s32 id, s32 item) {
  struct Actor *actor;
  struct Sprite *sprite;
  u8 *icon;
  s32 tile;

  actor = Actor_GetById(id);
  if (actor != 0 && actor->spriteStorageKind == 1) {
    sprite = actor->sprite;
    icon =
        (u8 *)HeapSlot_Alloc(HEAP_SLOT_GLYPH_TRANSFER, sizeof(GlyphTransfer)) +
        1024;
    Dma_Fill32(0, icon, 128);
    Icon_DrawItem(item);
    tile = VramCache_Load(sprite->slot, 128, icon);
    HeapSlot_Release(HEAP_SLOT_GLYPH_TRANSFER);
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
