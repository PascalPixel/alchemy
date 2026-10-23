#include "unit/field/field.h"
#include "types.h"
#include "object_event.h"
#include "game_state.h"

// Imil and Fuchin Falls, and Vale's cave.

/// Moves the character the player controls (in animation 27) and object event
/// id together by (dx, dz) pixels to the centres of their cells, at a pixel a
/// frame, as when pushing it; the object event faces the way it goes.
static void MapObjectEvent_PlacePairAtOffset(s32 id, s32 dx, s32 dz) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *other;
  s32 x;
  s32 z;

  objectEvent = ObjectEvent_GetById(gGameState.selectedObjectEvent);
  other = ObjectEvent_GetById(id);
  Engine_ScriptStart();
  {
    x = ((objectEvent->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
    z = ((objectEvent->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

    objectEvent->speed = 0x10000;
    objectEvent->acceleration = 0x8000;
    Engine_ObjectSetPosition(objectEvent, x, objectEvent->y.fixed, z);
  }
  ObjectEvent_SetMode(objectEvent, 27);
  {
    x = ((other->x.fixed + (dx << 16)) & 0xFFF00000) + 0x80000;
    z = ((other->z.fixed + (dz << 16)) & 0xFFF00000) + 0x80000;

    other->speed = 0x10000;
    other->acceleration = 0x8000;
    Engine_ObjectSetPosition(other, x, other->y.fixed, z);
  }
  if (dx < 0 || dz < 0) {
    ObjectEvent_SetMode(other, 4);
  } else {
    ObjectEvent_SetMode(other, 3);
  }
  FieldObject_WaitForMove(objectEvent);
  Engine_ScriptEnd();
}
