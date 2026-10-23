/// @file
/// The three Colosseum finals matches, and three of Crossbone Isle Cave's side
/// rooms (CrossboneIsleKeys, CrossboneIsleStatues and CrossboneIsleBane): an
/// actor drawn as an item's menu icon, such as the two prizes at a finals
/// match's arena or the keys in CrossboneIsleKeys.
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

/// Replaces the picture of actor id, when it has a sprite of its own, with the
/// icon of item: the icon's tiles, decoded into heap block 17, are loaded into
/// the sprite's VRAM block and its animation is stopped.
void Colosso_ShowItemIcon(s32 id, s32 item) {
  struct ObjectEvent *obj;
  s32 one;
  struct AnimationObject *spr;
  s32 base;
  s32 tile;
  s32 none;

  obj = ObjectEvent_GetById(id);
  if (obj != 0) {
    one = obj->spriteStorageKind;
    if (one == 1) {
      spr = (struct AnimationObject *)obj->sprite;
      base = (s32)Runtime_AllocateHeapBlock(17, 0x608);
      none = 0;
      base += 0x400;
      Dma_Fill32(none, (void *)base, 0x80);
      ItemIcon_LoadTilesFar(item);
      tile = Engine_VramLoad(spr->slot, 128, (const void *)base);
      Runtime_ReleaseHeapBlock(17);
      obj->retainSpriteCache = one;
      ResourceMetadata_ClearRecord(spr->entries[0]);
      spr->entries[0] = (void *)none;
      spr->count = none;
      spr->image.fullColor = none;
      spr->image.tile = tile;
      spr->dirty = none;
      spr->flags = none;
    }
  }
}
