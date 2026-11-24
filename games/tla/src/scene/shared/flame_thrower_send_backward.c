/// A flame thrower's flame (FLAME_BACKWARD), sent north from `position` at
/// `speed` (Flame_UpdateForward); `offScreen` keeps its sprite's picture
/// loaded out of sight.
void FlameThrower_SendBackward(s32 *position, s32 speed, s32 offScreen) {
  struct Actor *actor = FieldActor_Create(SPRITE_BLAZE_FLAME, position[0],
                                          position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->sceneWord = -speed;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateForward;
    if (offScreen)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}
