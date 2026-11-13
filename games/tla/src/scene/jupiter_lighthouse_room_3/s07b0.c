/// @file
/// Scene code of overlay 0x69c, in ROM order, from s07b0's offset up to the next object the overlay links.

#include "../shared/routines.h"
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
#include "../jupiter_lighthouse_aerie/jupiter_lighthouse_aerie.h"
#include "field_coordinate.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_3.h"
#include "module/scene/scene.h"

extern const s32 gData_02002cc8[];

s32 Func_02002450(s32 x, s32 z, s32 layer);
s32 Func_02000784(s32 *from, s32 *to);
void Actor_StartScript(struct Actor *object, const s32 *script);

void Func_020007b0(struct Actor *self) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 position[3];
  s32 distance;
  s32 near;

  if (work->unk166 != 0 || *(s16 *)&work->unk15a != 0 ||
      *(s16 *)&work->unk15c[2] != 0 || *(s16 *)work->unk168 != 0) {
    Actor_SetAnimationSpeed(self, 0);
    return;
  }
  Actor_SetAnimationSpeed(self, 16);
  if ((++*(u16 *)&self->timer & 31) == 31)
    Event_PlaySound(231);
  ((struct FieldSprite *)self->sprite)->priority =
      ((struct FieldSprite *)leader->sprite)->priority;
  if (Func_02002450(self->x, self->z, 2) != 255) {
    position[0] = self->x;
    position[1] = self->y;
    position[2] = self->z;
    Camera_WorldToScreen(position);
    distance = position[0];
    if (distance > 0x1100000)
      goto stop;
    if (distance < -0x200000)
      goto stop;
    {
      if (*((u8 *)self + 0x62) == 0) {
        distance = leader->y - self->y;
        if (distance < 0)
          distance = self->y - leader->y;
        near = 0;
        if (Func_02000784(&leader->x, &self->x) <= 12 && distance < 0xc0000)
          near = 1;
        if (near && !Event_GetFlag(260)) {
          leader->facing = self->facing;
          leader->watchedObject = self;
          leader->priorityFlags &= ~1;
          work->unk16a = 200;
          gMainRoutineWork.renderMode = 0;
          *((u8 *)self + 0x62) = 1;
          *((u8 *)self + 0x63) = 1;
        }
      }
      // IwramMulQ16 reached from IwramIrqMain rather than at its fixed address:
      // the address is a linked word read after the cosine, as the native
      // reads it after the speed (at Iwram_MulQ16 it is built before).
      self->x += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(
          Math_Cosine(self->facing), self->speedLimit);
      self->z += ((s32 (*)(s32, s32))(IwramIrqMain + Iwram_MulQ16Offset))(
          Math_Sin(self->facing), self->speedLimit);
      return;
    }
  }
stop:
  {
    s32 zero = 0;

    /* FAKEMATCH(temporary): one word zero clears the ride byte, then the
     * update; as self->update = 0; *((u8 *)self + 0x63) = 0;, 4 of 197
     * instructions differ in tla-en (byte first, 15). */
    *((u8 *)self + 0x63) = zero;
    self->update = (void *)zero;
  }
  Actor_StartScript(self, gData_02002cc8);
  Event_PlaySound(228);
  gData_02002ce8 = 0;
}

void Func_020022e0(struct GustObject *object, s32 mode);
void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_02001788(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_FUNC_020007E8(Func_02000970, Func_020022e0, Func_020007b0, Func_02001788)

DEFINE_FUNC_020008C0(Func_02000a48, Func_02001788)

DEFINE_FUNC_0200091C(Func_02000ae8)

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
