// Draft, differ 318 of 243 (2026-10-25): the heap slot 0xa4 chain work's
// drawing callback, which Func_080da9a8 schedules first. For each of the eight
// chains it walks the links from the root and draws each link after the first
// at the midpoint between it and the one before, as a 16-pixel sprite whose
// picture is the link's kind (kinds 2 and up; 8 tiles each from the work's
// sheet), turned to the line between the two links: ArcTan2 of the across
// distance against the larger of the height and depth distances. The last angle
// is kept on the chain's root. Left: the native keeps the previous link's x on
// the stack and its y and z in r8 and r6, steps a pointer to each chain's root,
// and builds the transform's halfwords as words.
#include "types.h"
#include "affine.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

s16 ArcTan2(s32 y, s32 x);
void Func_080140d8(void *entry, s32 slot);

struct ChainSprite {
  void *link;
  u32 attr01; ///< bits: 31..16 attribute 1 | 15..0 attribute 0
  u32 attr2;
};

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 kind; ///< 0 or 1: not drawn; else its picture, from 2
  u8 unk13;
  struct ChainSprite sprite;
};

struct ChainSlot {
  s16 id;
  u8 unk02[22];
  struct ChainLink *root;
};

struct ChainWork {
  s32 sheet; ///< the resource entry of the links' tiles
  struct ChainSlot slots[8];
};

#define ABS(a, b) (a - b >= 0 ? a - b : b - a)

void Func_080daab0(void) {
  struct ChainWork *work = Ram_HeapSlots->unk84[8];
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ChainSlot *slot = work->slots;
  u32 tiles = gResourceTableEntries[work->sheet].offset >> 5;
  s32 viewX = map->viewX & 0xffff0000;
  s32 viewZ = map->viewZ & 0xffff0000;
  s32 height = map->origin[1];
  struct AffineTransform transform;
  struct ChainLink *link;
  s32 prevX = 0, prevY = 0, prevZ = 0;
  s32 x, y, z;
  u16 angle;
  s32 i;

  for (i = 7; i >= 0; i--, slot++) {
    for (link = slot->root; link != NULL; link = link->next) {
      x = link->x;
      y = link->y;
      z = link->z;
      if (link->kind > 1) {
        s32 dz = prevZ - z;
        s32 dy = prevY - y;
        s32 screenX, depth, base, screenY;
        u32 tile;

        if (ABS(prevZ, z) < ABS(prevY, y))
          angle = ArcTan2(prevX - x, dy);
        else
          angle = ArcTan2(prevX - x, dz);
        tile = tiles + link->kind * 8 - 16;
        screenX = (prevX + x) / 2 - viewX;
        depth = (prevY + y) / 2 - height;
        base = (prevZ + z) / 2 - viewZ - height;
        screenY = base - depth;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 priority = ((depth + base) >> 16) + 34;

          link->sprite.link = NULL;
          link->sprite.attr01 = (((screenY >> 16) - 8) & 0xff) |
                                ((((screenX >> 16) - 8) & 0x1ff) << 16) |
                                0x40002000;
          link->sprite.attr2 = tile | 0x800;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            ((u8 *)&link->sprite.attr01)[1] =
                (((u8 *)&link->sprite.attr01)[1] & ~3) | 1;
            ((u8 *)&link->sprite.attr01)[3] =
                (((u8 *)&link->sprite.attr01)[3] & ~0x3e) |
                ((AffineMatrix_BuildForEffect(&transform) & 31) << 1);
          }
          link->angle = angle;
          Func_080140d8(&link->sprite, priority);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
    }
    prevX = 0;
    prevY = 0;
    prevZ = 0;
    if (slot->root != NULL)
      slot->root->angle = angle;
  }
}
