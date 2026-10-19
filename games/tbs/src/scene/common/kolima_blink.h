/// @file
/// Kolima and its forest bridge: the sparkling dust that falls when Tret's
/// curse strikes ("That sparkly stuff on the ground... What is it?").
#include "script.h"
#include "types.h"
#include "field_obj.h"
#include "field_event.h"

/// An object script call, each frame of the fall: the speck flickers between
/// palettes 10 and 7, every second frame.
static s32 Kolima_FlickerCurseDust(struct ScriptInterpreter *interpreter) {
  struct FieldActor *speck = (struct FieldActor *)interpreter;
  if ((gFrameCount >> 1) & 1) {
    ObjectGroup_SetChildValue(speck, 10);
  } else {
    ObjectGroup_SetChildValue(speck, 7);
  }
  return 0;
}
