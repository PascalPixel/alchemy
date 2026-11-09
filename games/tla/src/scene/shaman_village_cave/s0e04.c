#include "local.h"
#include "scene_tables.h"

struct ShamanVillageCaveSceneWork {
  u8 unk000[0x1ac];
  s32 screenEffect;
};

void Func_020003cc(void);
void Func_02000404(void);
void Func_02000a24(void);
void Func_0200087c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void ShamanVillageCave_ObjPlace(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

s32 Func_02000e04(void) {
  struct Actor *object;

  ((struct ShamanVillageCaveSceneWork *)Ram_HeapSlots->eventWork)
      ->screenEffect = 516;
  if (Event_GetFlag(0x9a9))
    Actor_SetPosition(8, 0, 0);
  if (Event_GetFlag(0x9aa))
    Actor_SetPosition(10, 0, 0);
  if (Event_GetFlag(83) || Event_GetFlag(163))
    Event_SetFlag(0x205);
  if (Event_GetFlag(0x9a8)) {
    object = Actor_Get(15);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->x = 0x2680000;
    object->y = 0;
    Actor_SetSingleChildFlags(object, 0);
    Func_02001060_(40, 19, 1, 1, 38, 19);
  }
  Actor_SetSingleChildFlags(Actor_Get(16), 0);
  Actor_SetAnimation(16, 1);
  if (Event_GetFlag(770)) {
    object = Actor_Get(16);
    Actor_SetAnimation(16, 5);
    Actor_SetSpriteColor(Actor_Get(16), 0);
    object->motionFlags = 0;
    object->priorityFlags = 0;
    object->y = 0;
    Func_02001060_(8, 42, 1, 1, 6, 42);
  }
  if (Event_GetFlag(518))
    ShamanVillageCave_ObjPlace(17, PIXELS(328), PIXELS(792));
  Actor_SetSpriteColor(Actor_Get(19), 1);
  Actor_SetSpriteColor(Actor_Get(20), 0);
  Actor_SetSpriteColor(Actor_Get(21), 2);
  if ((!Event_GetFlag(265) &&
       (gMainRoutineWork.entrance == 20 || gMainRoutineWork.entrance == 21)) ||
      Event_GetFlag(0xa71)) {
    Func_020003cc();
    if (!Event_GetFlag(265) && gMainRoutineWork.entrance == 20) {
      Func_02000a24();
    } else if (!Event_GetFlag(265) && gMainRoutineWork.entrance == 21) {
      Event_SetFlag(0xa71);
      Func_0200087c();
    }
  }
  if (Event_GetFlag(0xa73))
    Func_02000404();
  return 0;
}
