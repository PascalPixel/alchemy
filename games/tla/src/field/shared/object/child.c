#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "object_lookup.h"
#include "actor_view.h"

#define Actor_SetPartAttribute                                                \
  ((void (*)(struct ActorView *, s32))Actor_SetPartAttribute)

void Actor_ApplyIndexedChildValue(struct ActorView *);

void Actor_ConfigureSpriteColor(s32 objectId, s32 value) {
  struct ActorView *object;

  object = ObjectTable_Get(objectId);
  if (object != NULL) {
    if (value & 0x100) {
      object->update = Actor_ApplyIndexedChildValue;
      return;
    }
    object->update = NULL;
    Actor_SetPartAttribute(object, value);
  }
}
