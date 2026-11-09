#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

void Func_02000358(void) {
  Event_SetFlag(0x200);
  return;
}

void Func_02000364(void) {
  Event_SetFlag(0x201);
  return;
}

void Func_02000374(void) {
  Event_SetFlag(0x202);
  return;
}

void Func_02000384(void) {
  Event_SetFlag(0x203);
  return;
}
