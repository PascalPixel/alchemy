/// Scene_CreateActor through the imported creator, with extra priority flags.
struct Actor *Scene_CreateImportedActor(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = Import_ActorCreate(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(object, 0);
    Actor_SetSpriteColor(object, SPRITE_COLOR_HIDDEN);
    object->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
    return object;
  }
  return NULL;
}
