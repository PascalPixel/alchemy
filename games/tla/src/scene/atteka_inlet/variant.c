#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "field_sprite.h"

struct FieldActor *Actor_GetById(s32 id);

void Func_02000bb8(void) {
  s32 variant = Actor_GetById(gMainRoutineWork.currentOwner)->sprite->priority;

  Actor_SetSpritePriority(16, variant);
  Actor_SetSpritePriority(8, variant);
  Actor_SetSpritePriority(10, variant);
}

void Func_02000bf0(void) {
  Actor_SetSpritePriority(
      13, Actor_GetById(gMainRoutineWork.currentOwner)->sprite->priority);
}
