/// @file
/// Scene code of overlay 0x69c, in ROM order.

#include "../shared/whirlwind.h"
#include "../shared/jupiter_lighthouse.h"
#include "../shared/effects.h"
#include "types.h"
#include "map.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "irq.h"
#include "event_work.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "call.h"
#include "field_effect.h"
#include "../shared/jupiter_lighthouse_aerie.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"

extern const s32 gWhirlwindEndScript[];

s32 Func_02002450(s32 x, s32 z, s32 layer);
s32 Position_FlatDistance(s32 *from, s32 *to);
void Actor_StartScript(struct Actor *object, const s32 *script);

DEFINE_WHIRLWIND_UPDATE_LOWER(Whirlwind_UpdateLower, Func_02002450, Position_FlatDistance,
    gWhirlwindEndScript, gWhirlwindRiding)

void Func_020022e0(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_02001788(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_WHIRLWIND_SPAWN(Whirlwind_Spawn, Func_020022e0, Whirlwind_UpdateLower, Func_02001788)

DEFINE_ACTOR_EMIT_RISING_SPARKS(Actor_EmitRisingSparks, Func_02001788)

DEFINE_JUPITER_LIGHTHOUSE_PLACE_RAISED_ACTORS(JupiterLighthouse_PlaceRaisedActors)

extern const s8 gData_02002484[][4];

/// Rewrites the map cells under a pushable actor: its sprite shape picks one of
/// the six footprints in gData_02002484 (four signed bytes each, in sixteenths
/// of a cell); the cells under it are copied from the map's spare area
/// (Map_CopyCellAttributeRect) and byte 2 of both collision layers there is
/// filled with 255. Returns 1, or 0 for a shape with no footprint. The twin of
/// overlay 6ad's Func_02004938.

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Rewrites the map cells under a pushable actor: its sprite shape picks one of
/// the six footprints in gData_02002484 (four signed bytes each, in sixteenths
/// of a cell); the cells under it are copied from the map's spare area
/// (Map_CopyCellAttributeRect) and byte 2 of both collision layers there is
/// filled with 255. Returns 1, or 0 for a shape with no footprint. The twin of
/// overlay 6ad's Func_02004938.
s32 Func_02000c78(s32 id) {
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  struct Actor *actor = Actor_Get(id);
  s16 shape = *(s16 *)((struct FieldSprite *)actor->sprite)->record;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gData_02002478[(u16)i]) {
  next:
    j = (u16)((i << 16) + 0x10000 >> 16);
    i = j;
    if (j <= 5 && (u16)shape != gData_02002478[j])
      goto next;
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gData_02002484[j][0]) + ABS(gData_02002484[j][2])) >> 4;
  height = (ABS(gData_02002484[j][1]) + ABS(gData_02002484[j][3])) >> 4;
  x = (actor->x + (gData_02002484[j][0] << 16)) >> 20;
  z = (actor->z + (gData_02002484[j][1] << 16)) >> 20;
  mapX = *(s32 *)(map + 328) >> 20;
  i = mapX; // i is reused to hold mapX.
  mapZ = *(s32 *)(map + 332) >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  Map_FillCellRectByte2(0, x, z, width, height, 255);
  Map_FillCellRectByte2(2, x, z, width, height, 255);
  return 1;
}

#include "../../field/map/cellfill.c"
