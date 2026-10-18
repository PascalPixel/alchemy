// Draft, differ 0 of 68 (matches): shows icon id on its owner's 16 by 16
// sprite: Func_08038248 prepares the icon in heap slot 68, whose tiles at
// +0x400 go to the sprite's VRAM cache slot, and the sprite is reset to
// scale 1.0, 16-colour palette 0. No ☀️ twin found.
#include "types.h"
#include "mem.h"
#include "resource.h"
#include "render_input.h"

struct IconSprite {
  u8 unk00[4];
  struct RenderOam oam;
  s32 scale;
  u8 vramSlot;
  u8 unk11[3];
  u8 width;
  u8 height;
  u8 unk16;
  s8 unk17;
  u8 unk18;
  u8 unk19;
  u8 unk1a;
  u8 unk1b;
};

struct IconOwner {
  u8 unk00[80];
  struct IconSprite *sprite;
  u8 unk54[8];
  u8 unk5c;
  u8 unk5d[5];
  u8 iconId;
};

void Func_08038248(s32 id);

void Func_0802372c(struct IconOwner *owner, s32 id) {
  struct IconSprite *sprite;
  u8 *buffer;
  s32 tile;

  if (owner == 0)
    return;
  owner->unk5c = 1;
  owner->iconId = id;
  sprite = owner->sprite;
  if (sprite == 0)
    return;
  buffer = Runtime_AllocateHeapBlock(68, 0x608);
  Func_08038248(id);
  tile = VramCache_Load(sprite->vramSlot, 128, buffer + 0x400);
  Runtime_ReleaseHeapBlock(68);
  sprite->unk1b = 0;
  sprite->unk19 = 0;
  sprite->width = 16;
  sprite->height = 16;
  sprite->unk16 = 0;
  sprite->unk17 = 4;
  sprite->scale = 0x10000;
  sprite->oam.index = tile;
  sprite->oam.palette256 = 0;
  sprite->oam.palette = 0;
}
