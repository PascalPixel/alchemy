#include "../shared/routines.h"
#include "types.h"
#include "system.h"
#include "iwram_call.h"
#include "ram_buffer.h"
#include "irq.h"
#include "event_work.h"
#include "actor.h"
#include "main_routine_work.h"
#include "field_sprite.h"
#include "event_inline.h"
#include "call.h"
#include "field_effect.h"
#include "field_coordinate.h"
#include "field_event.h"
#include "task.h"
#include "frame.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "module/scene/jupiter_lighthouse_entrance.h"
#include "module/scene/scene.h"

DEFINE_FUNC_020003D0(Func_020005b8)

extern const s32 gData_0200219c[];
extern s32 gData_020021bc;

s32 Func_02001b2c(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

void Func_020005e4(struct Actor *self) {
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
  if (Func_02001b2c(self->x, self->z, 2) != 255) {
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
        if (Func_020005b8(&leader->x, &self->x) <= 12 && distance < 0xc0000)
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
  Actor_StartScript(self, gData_0200219c);
  Event_PlaySound(228);
  gData_020021bc = 0;
}

void Actor_SetSingleChildFlags(struct FieldEffect *effect, s32 flags);

DEFINE_FUNC_020007A4(Func_020007a4, Func_020019fc, Func_020005e4, Func_02000eac)

s32 Func_0200087c(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.priority = 3;
    params.startScaleX = 0xb333;
    params.startScaleY = 0xb333;
    params.palette = 14;
    x = object->x + (((s32)((u32)(Random16Far() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random16Far() * 20) >> 16)) << 16);
    speed = ((((s32)((u32)(Random16Far() * 5) >> 16)) << 16) + 0x50000) / 10;
    Func_02000eac(x, y, object->z, 0, speed, phase, 0xb0000, &params);
  }
  return 0;
}

void Animation_ApplyChildValues(struct PositionedActor *object, s32 value);

DEFINE_FUNC_0200091C(Func_0200091c)

// Queues the lit actors' sprites: for each listed actor with a position, projects it
// from the map's view, skips it off screen, and writes a 16 by 16 sprite, or a 32 by
// 64 one when the big switch is set, at its animation picture and depth.
#include "map.h"
#include "vram_tab.h"

extern s16 gData_02002200;
extern u32 gData_02002204[];

void Func_020019d4(struct LightOam *oam, s32 depth);

DEFINE_FUNC_02000AAC(Func_02000aac, gData_02002200, Data_02002244,
    gData_02002202, gData_02002204, gData_02002304, gData_02002306,
    Func_02001b24, Func_020019d4)

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

DEFINE_FUNC_02000A98(Func_02000c68, gData_02002204, gData_02002200,
    Func_02000aac, gData_02002202, gData_02002304, gData_02002306)

DEFINE_FUNC_02000CE8(Func_02000ce8, gData_02002204, gData_02002200,
    Func_02000aac, gData_02002202, gData_02002304, gData_02002306)

DEFINE_FUNC_02000D68(Func_02000d68, gData_02002204, gData_02002200,
    Func_02000aac, gData_02002202, gData_02002304, gData_02002306)

DEFINE_FUNC_02000DF0(Func_02000df0, gData_02002202, gData_02002204)

void Func_02000e20(s32 value) { gData_02002306 = value; }

#include "../../field/object/set_sprite_priority.c"
