#include "types.h"
#include "adventure.h"
#include "field_event.h"
#include "field_sprite.h"

struct FieldActor *Actor_GetById(s32 id);
void Actor_SetSpritePriority(u32 actor, s32 variant);

void Func_02002a08(void) {
  s32 variant = Actor_GetById(gAdventure.currentOwner)->sprite->priority;

  Actor_SetSpritePriority(8, variant);
  Actor_SetSpritePriority(10, variant);
}
