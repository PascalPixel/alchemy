#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "party_state.h"
#include "field_event.h"
#include "field_sprite.h"

struct FieldActor *Object_GetById(s32 id);

void Func_02000bb8(void) {
  s32 variant = Object_GetById(gPartyState.currentOwner)->sprite->priority;

  ObjectMotion_SetActionVariant(16, variant);
  ObjectMotion_SetActionVariant(8, variant);
  ObjectMotion_SetActionVariant(10, variant);
}

void Func_02000bf0(void) {
  ObjectMotion_SetActionVariant(
      13, Object_GetById(gPartyState.currentOwner)->sprite->priority);
}
