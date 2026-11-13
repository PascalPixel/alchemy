/// @file
/// Magma Rock's inside, the volcano's heart: the Moai that flood its rooms with
/// lava when Burst wakes them, the platforms that ride the lava and the gates
/// that drain it, the pillars pushed into it, Fury the Djinni, and deep inside
/// the tablet where Jenna learns Blaze and the flames Blaze moves.

#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"

#include "magma_rock_inside.h"

struct Actor *MagmaRockInside_CreateEffectActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *object = FieldActor_Create(kind, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}
