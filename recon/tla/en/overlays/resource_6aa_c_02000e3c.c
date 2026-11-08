// Matches (tla-en, 2026-11-08, -mtune=gba); also differ 0 in tla-ja, de, es, fr and
// it (instructions; pools not compared). What closed it: the cell pointer
// stepped to the row below in one statement (cells += x + (z << 7) + 128, then
// cells->attributeB); as cells[128] after the step, 6; one index expression, 42.
// No device: the window-layer macro is a once-only block with a local that
// holds one constant (as REG_WINOUT = 0xf1f, 30 of 143 differ in tla-en), and
// the 0x100 argument is such a local too (as the literal, 2). Near twin:
// Func_02000fe0 in mars_lighthouse_entrance/s0fe0.c.
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "map.h"
#include "ram_buffer.h"
#include "scene_tables.h"
#include "system/io_reg.h"

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
void FieldEvent_Wait(s32 frames);
void Func_02000df0(void);
void Func_02004bc0(struct Actor *object, s32 a1);
void Func_02004ca8(void);
void Func_02004d00(s32 id, s32 a1, s32 a2);
void Func_02004d30(s32 a0);
void Func_02004da0(s32 a0);
void Func_02004de0(s32 cue);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
struct Actor *Actor_GetById(u32 id);
void Actor_SetModeById(u32 id, s32 mode);
void wait(s32 frames);
extern struct MapCell gMapCellBuffer[];

void Func_02000e3c(s32 arg) {
  struct Actor *actor;
  struct MapWork *map;
  struct MapCell *cells;
  struct MapCell *buffer;
  s32 x, z;
  s32 i;

  actor = Actor_GetById(gMainRoutineWork.currentOwner);
  Func_02004ca8();
  Func_02004da0(0);
  Actor_SetSingleChildFlags(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  actor->update = NULL;
  {
    s32 value = 0x100;

    Func_02004d00(gMainRoutineWork.currentOwner, value, 0);
  }
  Func_02004bc0(actor, 27);
  Actor_SetAnimationSpeed(actor, 48);
  wait(30);
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
  Actor_SetModeById(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  i = 0;
next:
  if (i == 5)
    Func_02004de0(204);
  actor->scaleX -= 0x400;
  actor->scaleY -= 0x300;
  actor->y -= 0x9999;
  wait(1);
  i++;
  if (i <= 39)
    goto next;
  Actor_GetById(gMainRoutineWork.currentOwner)->animationKind = 0;
  ((struct Overlay6aaEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02004d30(arg);
}
