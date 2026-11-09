#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

extern u32 Data_0200eb60;
extern const u8 gData_02005aa4[];
extern const u8 gData_02005ab4[];
extern const u8 gData_02005ae0[];
extern const u8 gData_02005b0c[];

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void Animation_SetStateFlags(struct Actor *object, s32 flags);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02004ef0(const u8 *a, const u8 *b, const u8 *c, const u8 *d);
void Func_020004a4(void);

static __inline__ void PlaceObject(s32 id, s32 a, s32 b) {
  /* FAKEMATCH(wrapper): forwarded inline; as a direct call, 10 of 124
   * instructions differ in tla-en. */
  Actor_SetHorizontalPositionWithTerrain(id, a, b);
}

void Func_020001d8(void) {
  struct Actor *object;
  s32 i;

  Func_02004ef0(gData_02005aa4, gData_02005ab4, gData_02005ae0, gData_02005b0c);
  Actor_Get(gMainRoutineWork.currentOwner)->priorityFlags |= 0x40;
  for (i = 10; i <= 16; i++) {
    object = Actor_Get(i);
    object->priorityFlags |= 0x80;
    Animation_ApplyChildValues(object, 5);
  }
  if (!Event_GetFlag(0xa3c))
    return;
  PlaceObject(8, 0x2000000, 0x1080000);
  PlaceObject(9, 0x2000000, 0x10f0000);
  System_Wait(1);
  FieldActor_SetSpritePriority(8, 1);
  object = Actor_Get(8);
  object->motionFlags = 4;
  object->y = 0x700000;
  Animation_ApplyChildValues(object, 5);
  Actor_SetSingleChildFlags(object, 0);
  object->priorityFlags |= 0x20;
  FieldActor_SetSpritePriority(9, 2);
  object = Actor_Get(9);
  object->motionFlags = 4;
  object->y = 0x700000;
  object->scaleX = 0x14ccc;
  object->scaleY = 0x14ccc;
  Animation_ApplyChildValues(object, 5);
  Actor_SetSingleChildFlags(object, 0);
  object->priorityFlags |= 0x20;
  object->priorityFlags |= 2;
  Animation_SetStateFlags(object, 1);
  Data_0200eb60 = 0;
  FieldTask_AddFrame(Func_020004a4, 0x480);
}
