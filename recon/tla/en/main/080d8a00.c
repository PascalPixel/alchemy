// First draft, differ 254 of 246: the heap slot 0x9c work's per-frame
// callback (Func_080d897c schedules it). For each of the eight slots it draws
// a sprite at the midpoint of each pair of neighbouring nodes in the slot's
// chain, 4 pixels up, turned along the pair (ArcTan2 plus a quarter turn,
// through an affine matrix when the angle is not 0) and given the slot's
// priority, culled to the screen and sorted into the OAM buckets by depth; the
// chain's first node takes the last angle. Structure and data agree;
// allocation and order differ throughout. Func_080d9b80 is the heap slot 0xa0
// sibling.
#include "types.h"
#include "animspr.h"
#include "map.h"
#include "ram_buffer.h"
#include "vram_tab.h"

struct ChainTransform {
  u32 scaleX : 16;
  u32 scaleY : 16;
  u32 angle : 16;
  u32 unk06 : 16;
};

struct ChainNode {
  struct ChainNode *next;
  s32 x;
  s32 y;
  s32 z;
  u16 angle;
  s8 used;
  u8 unk13;
  struct AnimationSpritePart sprite;
};

struct ChainSlot {
  s16 id;
  s16 value;
  s16 depth;
  s16 priority;
  s16 flag;
  s16 state;
  u8 unk0c[16];
  struct ChainNode *head;
};

struct ChainWork {
  u8 unk000[8];
  s32 vramEntry;
  struct ChainSlot slots[8];
};

s16 ArcTan2(s32 y, s32 x);
s32 AffineMatrix_BuildForEffect(struct ChainTransform *source);
void Func_080140d8(void *entry, s32 slot);

void Func_080d8a00(void) {
  struct ChainWork *work = Ram_HeapSlots->unk84[6];
  struct ChainSlot *slot = work->slots;
  s32 prevX = 0;
  s32 prevY = 0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  s32 prevZ = 0;
  s32 started = 0;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = map->origin[1];
  struct ChainTransform transform;
  struct ChainNode *node;
  u32 angle;
  s32 i;

  for (i = 7; i >= 0; i--) {
    for (node = slot->head; node != NULL; node = node->next) {
      s32 priority = slot->priority;
      s32 y = node->y + 0xfffc0000;
      s32 z = node->z;
      s32 x = node->x;

      if (started) {
        s32 screenX;
        s32 height;
        s32 depth;
        s32 screenY;

        angle = ((u16)ArcTan2(prevX - x, (z - prevZ) + (prevY - y)) + 0x4000) &
                0xffff;
        screenX = (prevX + x) / 2 - viewX;
        height = (prevY + y) / 2 - originY;
        depth = (prevZ + z) / 2 - viewZ - originY;
        screenY = depth - height;
        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          ((u32 *)&node->sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = ((((screenX >> 16) - 4) & 0x1ff) << 16) |
                                      (((screenY >> 16) - 4) & 0xff) | 0x2000;
          ((u32 *)&node->sprite)[2] = tile | 0x800;
          if (angle != 0) {
            transform.angle = angle;
            transform.scaleX = 0x100;
            transform.scaleY = 0x100;
            node->sprite.affine = 1;
            node->sprite.affineIndex = AffineMatrix_BuildForEffect(&transform);
          }
          node->sprite.priority = priority;
          node->angle = angle;
          Func_080140d8(&node->sprite, ((height + depth) >> 16) + 58);
        }
      }
      prevX = x;
      prevY = y;
      prevZ = z;
      started = 1;
    }
    prevX = 0;
    prevY = 0;
    prevZ = 0;
    started = 0;
    if (slot->head != NULL)
      slot->head->angle = angle;
    slot++;
  }
}
