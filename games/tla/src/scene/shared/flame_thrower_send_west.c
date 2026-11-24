/// A flame thrower's flame (FLAME_WEST), sent west from `position`
/// at `speed` (Flame_UpdateSideways); `offScreen` keeps its sprite's picture
/// loaded out of sight.
void FlameThrower_SendWest(s32 *position, s32 speed, s32 offScreen) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],
                                          position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->sceneWord = -speed;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateSideways;
    if (offScreen)
      actor->retainSpriteCache = ACTOR_COMPOSE_OFF_SCREEN;
    Actor_SelectAnimation(actor, 2);
  }
}
