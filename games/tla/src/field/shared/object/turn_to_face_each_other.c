#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_work.h"
#include "types.h"
#include "facing_object.h"
#include "ram_buffer.h"

s32 ArcTan2(s32 y, s32 x);

void Actor_TurnToFaceEachOther(s32 firstId, s32 secondId, s32 wait) {
  void *first = ObjectTable_Get(firstId);
  void *second = ObjectTable_Get(secondId);

  if (first != NULL && second != NULL) {
    FacingObject_TurnPairToFaceEachOther(first, second);
    Event_WaitFrames(wait);
  }
}

void FacingObject_TurnPairToFaceEachOther(struct FacingObject *first,
                                          struct FacingObject *second) {
  s32 toward;
  s32 away;
  s32 frame;
  s32 turning;
  s32 delta;

  if (first == NULL || second == NULL)
    return;
  toward = (u16)ArcTan2(second->positionZ - first->positionZ,
                        second->positionX - first->positionX);
  away = toward + 0x8000;
  for (frame = 0; frame < 60; frame++) {
    delta = (s16)(toward - first->facing);
    turning = 2;
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      first->facing += delta;
    } else {
      turning = 1;
    }
    delta = (s16)(away - second->facing);
    if (delta != 0) {
      if (delta > 0x1000)
        delta = 0x1000;
      if (delta < -0x1000)
        delta = -0x1000;
      second->facing += delta;
    } else {
      turning--;
    }
    if (turning == 0)
      break;
    WaitFrames(1);
  }
}

void ObjectTable_DestroyById(s32 id) {
  void *object = ObjectTable_Get(id);
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (object != NULL) {
    Actor_Destroy(object);
    work->objects[id] = NULL;
  }
}
