/// @file
/// Screen shake, the copied BG row drawer and the ground plane scanlines.
// Part of field.c.

#include "../field_internal.h"

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/ram_code.h"
#include "module/field/resource_slot.h"

extern u32 Data_02018000[];

u8 WorldMap_GetBattleEnvironment(struct WorldPosition *position,
                                 s32 *terrainKind) {
  s32 selector = WorldMap_GetCollision(position);
  s32 x = position->x;
  s32 y;
  s32 flag = 0;
  u32 tileX;
  u32 index;
  u32 *tile;
  u32 kind;

  if (x < 0)
    x += 0x1fffff;
  tileX = (x >> 21) & 31;
  y = position->y;
  if (y < 0)
    y += 0x1fffff;

  index = tileX + (((y >> 21) & 31) << 5);
  tile = gMapBlocks + index;
  if (((u8 *)tile)[3] & 0x80)
    flag = 0x10;

  kind = (*tile << 2) >> 26;
  *terrainKind = kind;
  if (kind > 40)
    flag = 0x20;
  if (kind == 36)
    flag = 0x40;
  if (kind == 12 || kind == 15)
    flag = 0x30;

  return gWorldMap_TerrainBehaviorTable[flag + selector];
}

void MapGrid_SetScreenShake(s32 value0, s32 value1, s32 value2) {
  struct MapWork *work;

  work = Ram_HeapSlots->mapWork;
  if (value0 >= 0) {
    work->shakeX = value0;
  }
  if (value1 >= 0) {
    work->shakeY = value1;
  }
  if (value2 >= 0) {
    work->shakeDecay = value2;
  }
}

#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void MapGrid_WaitForShakeToSettle(void) {
  s32 *work;
  s32 cnt;

  work = (s32 *)Ram_HeapSlots->mapWork;
  cnt = 0;
  while (cnt < 300 && (work[1] > 255 || work[2] > 255)) {
    wait(1);
    cnt++;
  }
  work[3] = 0;
}

void TileMap_DrawRowsFromRam(s32 arg0, s32 arg1) {
  void (*routine)(s32, s32, u8 *, u8 *);
  u8 *tiles = gDecodeBuffer;

  RamCode_Load(routine, Render_DrawMapCode, Render_DrawMapCodeSize);
  routine(arg0, arg1, gData_0203c000, tiles + 0x1000);
  Heap_Release(routine);
}

void WorldMap_BuildScanlineTable(s32 pitchSlope, s32 *position,
                                 struct ScanlineRow *row) {
  s32 vector[3];
  s32 result[3];
  s32 height;
  s32 nearZ;
  s32 i;
  s32 z;
  s32 ratio;
  s32 dx;
  s32 sum;
  s32 zero;

  zero = 0;
  vector[0] = position[0];
  vector[1] = zero;
  vector[2] = position[2];
  Iwram_TransformVector(vector, result);
  height = result[1] - Iwram_MulQ16(result[2], pitchSlope);
  nearZ = -gCameraSceneParameters[0];
  for (i = 0; i < 160; i++) {
    s32 distance;

    z = Iwram_RatioMulQ14(nearZ, (gCameraSceneParameters[4] - i) * Q16_ONE);
    distance = z - pitchSlope;
    if (distance == 0)
      distance = 1;
    ratio = Iwram_RatioMulQ14(distance, height);
    if (ratio < 0) {
      row->scale = Iwram_RatioMulQ14(gCameraSceneParameters[0],
                                     Iwram_MulQ16(-ratio, Q16_ONE / 2));
      z = Iwram_MulQ16(ratio, z);
      dx = (result[2] - ratio) >> 4;
      z -= result[1];
      sum = Iwram_MulQ16(dx, dx);
      z >>= 4;
      sum += Iwram_MulQ16(z, z);
      {
        s32 length = Iwram_Sqrt(sum) << 12;

        if (z < 0)
          length = -length;
        row->depth = Iwram_MulQ16(length, Q16_ONE / 2);
      }
    } else {
      row->scale = 0;
      row->depth = 0;
    }
    row->offsetX = 0;
    row->offsetY = 0;
    row++;
  }
}
