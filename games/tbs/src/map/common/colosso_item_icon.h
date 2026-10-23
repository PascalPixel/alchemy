/// @file
/// The three Colosseum finals matches, and three of Crossbone Isle Cave's side
/// rooms (CrossboneIsleKeys, CrossboneIsleStatues and CrossboneIsleBane): an
/// object event drawn as an item's menu icon, such as the two prizes at a
/// finals match's arena or the keys in CrossboneIsleKeys.
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "object_event.h"
#include "script_layer.h"
#include "animspr.h"
#include "mem.h"
#include "dma.h"
#include "io_reg.h"

/// Replaces the picture of object event id, when it has a sprite of its own,
/// with the icon of item: the icon's tiles, decoded into heap block 17, are
/// loaded into the sprite's VRAM block and its animation is stopped.
void Colosso_ShowItemIcon(s32 id, s32 item) {
  struct ObjectEvent *objectEvent;
  s32 one;
  struct Sprite *sprite;
  s32 base;
  s32 tile;
  s32 none;

  objectEvent = ObjectEvent_GetById(id);
  if (objectEvent != 0) {
    one = objectEvent->spriteStorageKind;
    if (one == 1) {
      sprite = (struct Sprite *)objectEvent->sprite;
      base = (s32)AllocHeapSlot(17, 0x608);
      none = 0;
      base += 0x400;
      Dma_Fill32(none, (void *)base, 0x80);
      Icon_DrawItem(item);
      tile = VramCache_Load(sprite->slot, 128, (const void *)base);
      FreeHeapSlot(17);
      objectEvent->retainSpriteCache = one;
      SpriteAnim_Clear(sprite->entries[0]);
      sprite->entries[0] = (void *)none;
      sprite->count = none;
      sprite->image.fullColor = none;
      sprite->image.tile = tile;
      sprite->dirty = none;
      sprite->flags = none;
    }
  }
}
