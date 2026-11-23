/// Scene_CreateActor through the overlay's import of the actor creator.
struct Actor *Scene_CreateImportedActor(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *actor = Import_ActorCreate(type, x, y, z);

  if (actor != NULL) {
    Scene_SetActorSpritePriority(actor, 1);
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    Actor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT | ACTOR_PRIORITY_IGNORES_RISES;
    return actor;
  }
  return NULL;
}
