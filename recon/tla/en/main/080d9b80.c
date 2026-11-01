// Draft, differ 4 of 208, skeleton gap 2 (tla-en, 2026-11-01): the heap slot 0xa0 work's first
// per-frame callback (Func_080d9b08 schedules it at 0x480). For each of the
// eight slots it builds an affine matrix from the slot's angle (when set) and
// draws a sprite at the midpoint of each pair of neighbouring nodes in the
// slot's chain, 4 pixels up, culled to the screen and sorted into the OAM
// buckets by depth. Func_080d8a00 is the heap slot 0x9c sibling.
//
// What closed it: prevY and prevZ are reused for the midpoints (the native's
// r10 and r7 hold both); y is loaded alone and adjusted by a separate +=;
// affine is set before the matrix call, so it crosses the call into r6; the
// sprite word is top | (left << 16) | 0x2000, whose operand order chooses
// every scratch register; declaration order prevX, prevY, prevZ, started sets
// the live lengths and the reload order, and nothing else here gives it.
//
// Left: one site, two instructions. At the top the native issues sub sp and
// the zero before `adds r3, #160; ldr r0, [r3]`; here those two come first,
// and every register and everything after them match. sched2 ranks the add 39
// and sub sp and the zero 38: the load chain (32, 36, 38, 42, 48, the tile
// store, the map load, the view reads, the origin load, 565) is one step too
// long, or the store of prevX one too short. Tried with no gain: work assigned
// after each zero (the zero then takes r0, not r2), statement orders of tile,
// map, view and origin, chained zero assignments, a heap local, an origin or
// view pointer, const on the view and origin loads. A device cannot help: the
// add and the load are one read and a barrier would also pin the `adds r3, r1`
// the native keeps above sub sp. This may be the load model (K2), not source.
#include "types.h"
#include "sprite.h"
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
  s32 i;
  s32 prevX = 0;
  s32 prevY = 0;
  s32 prevZ = 0;
  s32 started = 0;
  u32 tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  s32 originY = map->origin[1];
  struct ChainTransform transform;

  for (i = 0; i < 8; i++) {
    u32 angle = work->slots[i].angle;
    struct ChainNode *node = work->slots[i].head;
    s32 affine;
    s32 matrix;

    if (angle != 0) {
      transform.angle = angle;
      transform.scaleX = 0x100;
      transform.scaleY = 0x100;
      affine = 1;
      matrix = AffineMatrix_BuildForEffect(&transform);
    } else {
      affine = 0;
      matrix = 0;
    }
    for (; node != NULL; node = node->next) {
      s32 y = node->y;
      s32 x = node->x;
      s32 z = node->z;

      y += 0xfffc0000;

      if (started) {
        s32 screenX = (prevX + x) / 2 - viewX;
        s32 screenY;

        prevY = (prevY + y) / 2 - originY;
        prevZ = (prevZ + z) / 2 - viewZ - originY;
        screenY = prevZ - prevY;

        if ((u32)(screenX + 0x1fffff) <= 0x12ffffe && screenY > -0x200000 &&
            screenY <= 0xdfffff) {
          s32 left = ((screenX >> 16) - 4) & 0x1ff;
          s32 top = ((screenY >> 16) - 4) & 0xff;
          s32 key = ((prevY + prevZ) >> 16) + 58;

          ((u32 *)&node->sprite)[0] = 0;
          ((u32 *)&node->sprite)[1] = top | (left << 16) | 0x2000;
          ((u32 *)&node->sprite)[2] = tile | 0x800;
          node->sprite.affine = affine;
          node->sprite.affineIndex = matrix;
          Func_080140d8(&node->sprite, key);
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
  }
}
