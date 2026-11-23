/// Creates a hidden, still actor of `kind` at (x, y, z), untargeted and drawn
/// underfoot, its sprite at priority 1.
struct Actor *Scene_CreateActor(s32 x, s32 y, s32 z, s32 kind) {
  struct Actor *actor;

  actor = FieldActor_Create(kind, x, y, z);
  if (actor != 0) {
    Scene_SetActorSpritePriority(actor, 1);
    actor->motionFlags = 0;
    actor->collisionFlags = ACTOR_NOT_TARGETED;
    Actor_SetSpriteFlags(actor, 0);
    FieldActor_SetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT |
                           ACTOR_PRIORITY_IGNORES_RISES | actor->priorityFlags;
    return actor;
  }
  return 0;
}
