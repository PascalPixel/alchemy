/// @file
/// Kolima and Kolima Forest (MapId_Kolima and MapId_KolimaForestEntrance):
/// the sparkling dust that falls when Tret's curse strikes. "That sparkly stuff
/// on the ground... What is it?"
#include "action_script.h"
#include "types.h"
#include "object_event.h"
#include "script_layer.h"

/// An object script call, each frame of the fall: the speck flickers between
/// palettes 10 and 7, every second frame.
static s32 Kolima_FlickerCurseDust(struct Object *interpreter) {
  struct ObjectEvent *speck = (struct ObjectEvent *)interpreter;
  if ((gFrameCount >> 1) & 1) {
    ObjectGroup_SetChildValue(speck, 10);
  } else {
    ObjectGroup_SetChildValue(speck, 7);
  }
  return 0;
}
