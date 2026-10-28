// Draft, differ 2 of 93 (was 116), rewritten from the listing: draws a sprite
// (resource gData_020068a2's tiles) at the leader's cell on the ground, written
// to the OAM entry Data_020068a4 when on screen and handed to Func_02004b80
// with its depth; the same shape as the 69d family's light sprites
// (resource_69d/02008e58.c). Left: scheduling: the native stores the second
// word before it builds 0x400 for the third. Tried: the third word through tile
// |= 0x400 (2, or 12 earlier), a barrier between the two stores (2: it holds
// back the call's argument), every single-statement move, permute 5 minutes.
#include "types.h"
#include "ram_buffer.h"
#include "map.h"
#include "sequence_parent_work.h"
#include "actor.h"
#include "vram_tab.h"

extern u16 gData_020068a2;
extern u32 Data_020068a4[3];

struct Actor *Actor_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004b80(u32 *oam, s32 depth);

void Func_02000d18(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *camera = (s32 *)((u8 *)map + 228);
  s32 x = camera[0] & 0xffff0000;
  s32 cameraZ = camera[1] & 0xffff0000;
  s32 originY = map->origin[1];
  s32 tile = gResourceTableEntries[gData_020068a2].offset >> 5;
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 y, z, depth, sx, sy;
  u32 *word;

  x = ((leader->x >> 20) << 20) - x;
  y = Map_GetTerrainHeight(0, leader->x, leader->z) - originY;
  z = ((leader->z >> 20) << 20) - cameraZ - originY;
  sy = z - y;
  sx = x >> 16;
  sy = (sy >> 16) + 16;
  depth = ((y + z) >> 16) + 58;
  if ((u32)(sx + 15) > 334 || sy < -15 || sy > 239)
    return;
  sx &= 0x1ff;
  sy &= 0xff;
  word = Data_020068a4;
  *word++ = 0;
  *word++ = sy | (sx << 16) | 0x80008800;
  *word = tile | 0x400;
  Func_02004b80(Data_020068a4, depth);
}
