/// A flame thrower's flame (FLAME_EAST), sent east from `position`
/// at `speed` (Flame_UpdateSideways); `offScreen` keeps its sprite's picture
/// loaded out of sight.
void FlameThrower_SendEast(s32 *position, s32 speed, s32 offScreen) {
  struct Actor *actor = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                            position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->sceneWord = speed;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateSideways;
    if (offScreen)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}
