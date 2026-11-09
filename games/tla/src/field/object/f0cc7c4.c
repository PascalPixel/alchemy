#include "types.h"
#include "main_routine_work.h"
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

/// Reads the map's entrance stream (gData_0202e000) for the current entrance,
/// places the party there unless flag 0x109 keeps its position, then sets the
/// camera bounds from the first box that holds it.
s32 Func_080cc7c4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 xFixed;
  u8 *cursor = gData_0202e000;
  s32 *angle;
  u16 *turn;
  s32 entrance = work->entrance;
  s32 height;
  s32 x;
  s32 z;
  s32 zFixed;
  s32 base;
  s32 flags;
  s32 left;
  s32 top;
  s32 bottom;
  s32 right;
  s32 leftEdge, topEdge, rightEdge, bottomEdge;

  cursor = gData_0202e000 + (u16)Stream_ReadU16(&cursor);
  angle = &Data_02000448;
  turn = &Data_0200044c;
  for (;; cursor += 6) {
    if (*(s8 *)cursor == -1) {
      *angle = 0x4000;
      {
        // A halfword constant through a plain pointer comes from the pool.
        s32 zero = 0;
        *turn = zero;
      }
      return 0;
    }
    if (*(s8 *)cursor == entrance) {
      /* FAKEMATCH(barrier): an empty asm reading work keeps it live across the
       * loop in r6, so entrance takes r7; without it, 14 of 211 instructions
       * differ in tla-en. */
      asm("" : : "r"(work));
      cursor++;
      x = Stream_ReadS16(&cursor);
      z = Stream_ReadS16(&cursor);
      xFixed = x << 16;
      zFixed = z << 16;
      height = Map_GetTerrainHeightFar(0, xFixed, zFixed) >> 16;
      flags = *cursor;
      base = (s32)gData_0202e000;
      if (!GameFlag_Test(0x109)) {
        gMainRoutineWork.x = xFixed;
        gMainRoutineWork.z = zFixed;
        *angle = (flags & 15) << 12;
        *turn = 0;
        if (flags & 16)
          gMainRoutineWork.y = 0x80000;
      }
      if (flags & 32)
        return 1;

      cursor = Data_0202e004;
      cursor = (u8 *)((u16)Stream_ReadU16(&cursor) + base);
      while (*(s8 *)cursor != -1) {
        {
          left = *(s8 *)cursor;
          leftEdge = left << 4;
          cursor++;
          top = *(s8 *)cursor;
          topEdge = top << 4;
          cursor++;
          right = *(s8 *)cursor;
          rightEdge = right << 4;
          cursor++;
          bottom = *(s8 *)cursor;
          bottomEdge = bottom << 4;
          cursor++;
          if (x >= leftEdge && x <= rightEdge && z - height >= topEdge &&
              z - height <= bottomEdge) {
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
        }
      }
      return 1;
    }
  }
}
