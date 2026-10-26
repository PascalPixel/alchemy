// Draft, differ 215 of 243 (318 before, 2026-10-26): the heap slot 0xa4 chain work's
// drawing callback, which Func_080da9a8 schedules first. For each of the eight
// chains it walks the links from the root and draws each link after the first
// at the midpoint between it and the one before, as a 16-pixel sprite whose
// picture is the link's kind (kinds 2 and up; 8 tiles each from the work's
// sheet), turned to the line between the two links: ArcTan2 of the across
// distance against the larger of the height and depth distances. The last angle
// is kept on the chain's root. The chains are indexed by a counter (the native's pointer to each root
// is its strength reduction), the transform is built a word at a time, the
// angle is the u16 of ArcTan2's result and the midpoint's height and depth
// reuse the previous y and z, which are dead by then (262 without that).
// Left: register choice: the native keeps the angle in r7 and the previous y
// in r8, here swapped, and spills less (frame 40, here 48).
#include "types.h"
#include "affine.h"
#include "map.h"
#include "ram_buffer.h"
#include "graphics/vram_tab.h"

s32 ArcTan2(s32 y, s32 x);
void Func_080140d8(void *entry, s32 slot);

// The transform as this function builds it, a word at a time.
struct ChainTransform {
  u32 scaleX : 16;
  u32 scaleY : 16;
  u32 angle : 16;
  u32 unk06 : 16;
};

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
  s32 i;
  u32 angle;
  s32 prevX = 0;
  u32 tiles = gResourceTableEntries[work->sheet].offset >> 5;
  s32 prevZ = 0;
  s32 prevY = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 height = map->origin[1];
  struct ChainTransform transform;
  struct ChainLink *link;

  for (i = 0; i < 8; i++) {
    for (link = work->slots[i].root; link != NULL; link = link->next) {
      s32 x = link->x;
      s32 z = link->z;
      s32 y = link->y;

      if (link->kind > 1) {
        s32 dz = prevZ - z;
        s32 dy = prevY - y;
        s32 screenX, depth, base, screenY;
        u32 tile;

        if (ABS(prevZ, z) < ABS(prevY, y))
          angle = (u16)ArcTan2(prevX - x, dy);
        else
          angle = (u16)ArcTan2(prevX - x, dz);
        tile = tiles + link->kind * 8 - 16;
        screenX = (prevX + x) / 2 - viewX;
        prevY = (prevY + y) / 2 - height;
        prevZ = (prevZ + z) / 2 - viewZ - height;
        screenY = prevZ - prevY;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 priority = ((prevY + prevZ) >> 16) + 34;

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
                ((AffineMatrix_BuildForEffect((struct AffineTransform *)&transform) & 31) << 1);
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
    if (work->slots[i].root != NULL)
      work->slots[i].root->angle = angle;
  }
}
