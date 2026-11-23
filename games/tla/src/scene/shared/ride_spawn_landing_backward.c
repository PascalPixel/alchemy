/// A flame thrower's flame (FLAME_LANDING_BACKWARD), sent west from `position`
/// at `speed` (Flame_UpdateLanding); `offScreen` keeps its sprite's picture
/// loaded out of sight.
void Ride_SpawnLandingBackward(s32 *position, s32 speed, s32 offScreen) {
  struct Actor *actor = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                            position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->riseBaseY = -speed;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateLanding;
    if (offScreen)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}
