/// @file
/// Scene code of overlay 0x663, in ROM order.

#include "module/scene/airs_rock.h"

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_LifeScripts
#define EFFECT_SPRITE_PRIORITY
#include "../../field/field_effect/spawn.c"

#include "types.h"

s32 Func_0200033c(s32 a0) {
  Actor_SetSpriteFlags(a0, 0);
  return 0;
}

#include "ram_buffer.h"

extern u8 gMapCellBuffer[];

s32 Func_02000348(u32 layer, s32 x, s32 y) {
  struct AirsRockMapWork *map =
      (struct AirsRockMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  return (cell[2] << 8) | cell[3];
}

s32 Func_02000384(u32 layer, s32 x, s32 y, s32 value) {
  struct AirsRockMapWork *map =
      (struct AirsRockMapWork *)Ram_HeapSlots->mapWork;
  u8 *cell;
  s32 old;

  if (map == NULL)
    return 0;
  if (layer <= 2)
    cell = map->layers[layer].cells;
  else
    cell = gMapCellBuffer;
  cell += (x + (y << 7)) << 2;
  old = (cell[2] << 8) | cell[3];
  cell[2] = value >> 8;
  cell[3] = value;
  return old;
}

#include "../../field/object/wait_until_idle.c"
