// First draft, differ 225 of 208: the heap slot 0xa0 work's first per-frame
// callback (Func_080d9b08 schedules it at 0x480). For each of the eight slots
// it builds an affine matrix from the slot's angle (when set) and draws a
// sprite at the midpoint of each pair of neighbouring nodes in the slot's
// chain, 4 pixels up, culled to the screen and sorted into the OAM buckets by
// depth. Structure and data agree; the native keeps the slot pointer at the
// angle (work + 16), prevX on the stack and started, prevY and prevZ in r11,
// r10 and r7, so allocation and order differ throughout. Func_080d8a00 is
// the heap slot 0x9c sibling.
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
  s8 used;
  u8 unk11[3];
  struct AnimationSpritePart sprite;
};

struct ChainSlot {
  s16 first;
  s16 second;
  u32 angle;
  s32 amplitude;
  u8 unk0c[4];
  s32 wobble;
  struct ChainNode *tail;
  struct ChainNode *head;
};

struct ChainWork {
  u8 unk000[8];
  s32 vramEntry;
  struct ChainSlot slots[8];
};

s32 AffineMatrix_BuildForEffect(struct ChainTransform *source);
void Func_080140d8(void *entry, s32 slot);

void Func_080d9b80(void) {
  struct ChainWork *work = Ram_HeapSlots->unk84[7];
  s32 prevX = 0;
  s32 started = 0;
  s32 prevY = 0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  struct ChainSlot *slot = work->slots;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 prevZ = 0;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = map->origin[1];
  struct ChainTransform transform;
  s32 i;

  for (i = 7; i >= 0; i--) {
    u32 angle = slot->angle;
    struct ChainNode *node = slot->head;
    s32 affine;
    s32 matrix;

    if (angle != 0) {
      transform.angle = angle;
      transform.scaleX = 0x100;
      transform.scaleY = 0x100;
      matrix = AffineMatrix_BuildForEffect(&transform);
      affine = 1;
    } else {
      affine = 0;
      matrix = 0;
    }
    for (; node != NULL; node = node->next) {
      s32 y = node->y + 0xfffc0000;
      s32 z = node->z;
      s32 x = node->x;

      if (started) {
        s32 screenX = (prevX + x) / 2 - viewX;
        s32 height = (prevY + y) / 2 - originY;
        s32 depth = (prevZ + z) / 2 - viewZ - originY;
        s32 screenY = depth - height;

        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          ((u32 *)&node->sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = ((((screenX >> 16) - 4) & 0x1ff) << 16) |
                                      (((screenY >> 16) - 4) & 0xff) | 0x2000;
          ((u32 *)&node->sprite)[2] = tile | 0x800;
          node->sprite.affine = affine;
          node->sprite.affineIndex = matrix;
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
    slot++;
  }
}
