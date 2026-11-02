#include "types.h"
#include "affine.h"
#include "map.h"
#include "ram_buffer.h"
#include "sprite.h"
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

struct ChainLink {
  struct ChainLink *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 kind; ///< 0 or 1: not drawn; else its picture, from 2
  u8 unk13;
  struct AnimationSpritePart sprite;
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

void Func_080daab0(void) {
  struct ChainWork *work = Ram_HeapSlots->unk84[8];
  s32 i;
  u32 angle;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tiles = gResourceTableEntries[work->sheet].offset >> 5;
  s32 prevZ = 0;
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
      s32 y = link->y;
      s32 z = link->z;

      if (link->kind > 1) {
        s32 dz = prevZ - z;
        s32 absZ = dz >= 0 ? dz : z - prevZ;
        s32 dy = prevY - y;
        s32 screenX, screenY;
        u32 tile;

        if (dy >= 0 ? absZ < dy : absZ < y - prevY)
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
          s32 sx = ((screenX >> 16) - 8) & 0x1ff;
          s32 sy = ((screenY >> 16) - 8) & 0xff;
          s32 priority = ((prevY + prevZ) >> 16) + 34;
          struct AnimationSpritePart *sprite = &link->sprite;

          ((u32 *)sprite)[0] = 0;
          ((u32 *)&link->sprite)[1] = sy | (sx << 16) | 0x40002000;
          ((u32 *)&link->sprite)[2] = tile | 0x800;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            link->sprite.affine = 1;
            link->sprite.affineIndex = AffineMatrix_BuildForEffect(
                (struct AffineTransform *)&transform);
          }
          link->angle = angle;
          Func_080140d8(sprite, priority);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
    }
    prevX = 0;
    prevY = 0;
    link = work->slots[i].root;
    prevZ = 0;
    if (link != NULL)
      link->angle = angle;
  }
}
