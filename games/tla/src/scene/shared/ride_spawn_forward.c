/// A flame thrower's flame (FLAME_FORWARD), sent south from `position` at
/// `speed` (Flame_UpdateForward); `offScreen` keeps its sprite's picture
/// loaded out of sight.
void Ride_SpawnForward(s32 *position, s32 speed, s32 offScreen) {
  struct Actor *actor = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                            position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->sceneWord = speed;
    Actor_ResetScriptedFor(actor, 2);
    actor->update = Flame_UpdateForward;
    if (offScreen)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}
