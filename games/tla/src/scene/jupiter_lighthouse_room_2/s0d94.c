/// @file
/// Scene code of overlay 0x69b, in ROM order, from s0d94's offset up to the
/// next object the overlay links.

#include "../shared/routines.h"
#include "types.h"
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
#include "map.h"
#include "field_event.h"
#include "vram_tab.h"
#include "task.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_room_2.h"
#include "module/scene/scene.h"

extern const s32 gData_02002b3c[];

void Actor_StartScript(struct Actor *object, const s32 *script);

void Func_02000d94(struct Actor *self) {
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
  if (Func_020022fc(self->x, self->z, 2) != 255) {
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
        if (Func_02000d68(&leader->x, &self->x) <= 12 && distance < 0xc0000)
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
  Actor_StartScript(self, gData_02002b3c);
  Event_PlaySound(228);
  Data_02002b94 = 0;
}

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

void Func_0200165c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags, struct EffectOptions *options);

DEFINE_FUNC_020007E8(Func_02000f54, Func_020021b4, Func_02000d94, Func_0200165c)

DEFINE_FUNC_020008C0(Func_0200102c, Func_0200165c)

DEFINE_FUNC_0200091C(Func_020010cc)

extern s16 gData_02002cdc;

void Func_0200218c(struct LightOam *oam, s32 depth);

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
DEFINE_FUNC_02000AAC(Func_0200125c, gData_02002bd8, Data_02002c1c,
    gData_02002bda, gData_02002bdc, gData_02002cdc, gData_02002cde,
    Func_020022f4, Func_0200218c)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02001418, gData_02002bdc, gData_02002bd8,
    Func_0200125c, gData_02002bda, gData_02002cdc, gData_02002cde)

DEFINE_FUNC_02000CE8(Func_02001498, gData_02002bdc, gData_02002bd8,
    Func_0200125c, gData_02002bda, gData_02002cdc, gData_02002cde)

DEFINE_FUNC_02000D68(Func_02001518, gData_02002bdc, gData_02002bd8,
    Func_0200125c, gData_02002bda, gData_02002cdc, gData_02002cde)

void Func_020015a0(s32 id, s32 value) {
  struct Actor *object = Actor_Get(id);

  if (object) {
    object->timer = value;
    {
      s16 *count;

      count = &gData_02002bda;
      gData_02002bdc[*count] = object;
      (*count)++;
    }
  }
}

void Func_020015d0(s32 a0) { gData_02002cde = a0; }

#include "../../field/object/set_sprite_priority.c"
