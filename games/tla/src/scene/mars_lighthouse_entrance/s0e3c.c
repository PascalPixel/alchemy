/// @file
/// The leader shrinks and sinks over 40 frames, then the screen transition
/// ends.

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/io_reg.h"
#include "event_inline.h"

// A once-only block with a local that holds one constant, as the native
// builds the value.
#define SET_SPRITE_WINDOW_LAYERS()                                             \
  do {                                                                         \
    u16 winout = 0xf1f;                                                        \
                                                                               \
    REG_WINOUT = winout;                                                       \
  } while (0)

struct Overlay6aaEventWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000df0(void);
void Func_02004bc0(struct Actor *object, s32 a1);
void Func_02004ca8(void);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
extern struct MapCell gMapCellBuffer[];

void Func_02000e3c(s32 arg) {
  struct Actor *actor;
  struct MapWork *map;
  struct MapCell *cells;
  struct MapCell *buffer;
  s32 x, z;
  s32 i;

  actor = Actor_Get(gMainRoutineWork.currentOwner);
  Func_02004ca8();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetSingleChildFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  actor->update = NULL;
  {
    s32 value = 0x100;

    FieldActor_ShowEmote(gMainRoutineWork.currentOwner, value, 0);
  }
  Func_02004bc0(actor, 27);
  Actor_SetAnimationSpeed(actor, 48);
  System_Wait(30);
  actor->angle = FACING_SOUTH;
  actor->motionFlags = 0;
  buffer = gMapCellBuffer;
  x = actor->x >> 20;
  z = actor->z >> 20;
  map = Ram_HeapSlots->mapWork;
  if (map != NULL)
    cells = map->layers[actor->terrainId & 3].cells;
  else
    cells = buffer;
  cells += x + (z << 7) + 128;
  if (!(cells->attributeB & 0x40)) {
    SET_SPRITE_WINDOW_LAYERS();
    REG_DISPCNT |= DISPCNT_OBJWIN_ON;
    Func_02000df0();
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Event_PlaySound(204);
  actor->scaleX -= 0x400;
  actor->scaleY -= 0x300;
  actor->y -= 0x9999;
  System_Wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct Overlay6aaEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(arg);
}
