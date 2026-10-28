// Draft, differ 177 (201 before statements reordered 2026-10-26).
// Draft, differ 196 of 211 (2026-10-18), from the listing: reads the map's
// entrance stream (gData_0202e000) for the current entrance, places the
// party there unless flag 0x109 keeps its position, then sets the camera
// bounds from the first box that holds it. The loop over entrances is a for
// with its step last, as the native lays it out. Left: register choice
// (the cursor's address and x swap r10 and r8), the halfword 0 for
// Data_0200044c comes from the literal pool where the native has movs, and
// the first pool is placed after the return 0 instead of at the end.
#include "types.h"
#include "adventure.h"
#include "ram_buffer.h"
#include "map.h"

extern u8 gData_0202e000[];
extern u8 Data_0202e004[];
extern s32 Data_02000448;
extern u16 Data_0200044c;
u32 Stream_ReadU16(u8 **cursor);
s16 Stream_ReadS16(u8 **cursor);
s32 Map_GetTerrainHeightFar(s32 layer, s32 x, s32 z);
s32 GameFlag_Test(s32 flag);

s32 Func_080cc7c4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  u8 *cursor = gData_0202e000;
  s32 entrance = gAdventure.entrance;
  s32 height;
  s32 x;
  s32 z;
  s32 flags;
  s32 left;
  s32 top;
  s32 bottom;
  s32 right;

  cursor = gData_0202e000 + (u16)Stream_ReadU16(&cursor);
  for (;; cursor += 6) {
    if (*(s8 *)cursor == -1) {
      Data_02000448 = 0x4000;
      Data_0200044c = 0;
      return 0;
    }
    if (*(s8 *)cursor == entrance) {
      cursor++;
      x = Stream_ReadS16(&cursor);
      z = Stream_ReadS16(&cursor);
      height = Map_GetTerrainHeightFar(0, x << 16, z << 16) >> 16;
      flags = *cursor;
      if (!GameFlag_Test(0x109)) {
        gAdventure.x = x << 16;
        gAdventure.z = z << 16;
        Data_02000448 = (flags & 15) << 12;
        Data_0200044c = 0;
        if (flags & 16)
          gAdventure.y = 0x80000;
      }
      if (flags & 32)
        return 1;

      cursor = Data_0202e004;
      cursor = gData_0202e000 + (u16)Stream_ReadU16(&cursor);
      if (*(s8 *)cursor != -1) {
        do {
          left = *(s8 *)cursor++;
          top = *(s8 *)cursor++;
          right = *(s8 *)cursor++;
          bottom = *(s8 *)cursor++;
          if (x >= left << 4 && x <= right << 4 && z - height >= top << 4 &&
              z - height <= bottom << 4) {
            map->minX = left << 20;
            map->minZ = top << 20;
            map->maxX = right << 20;
            map->maxZ = bottom << 20;
            if (map->minX + (240 << 16) > map->maxX)
              map->minX = map->maxX - (240 << 16);
            if (map->minZ + (160 << 16) > map->maxZ)
              map->minZ = map->maxZ - (160 << 16);
            break;
          }
        } while (*(s8 *)cursor != -1);
      }
      return 1;
    }
  }
}
