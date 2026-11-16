/// A flame thrower's flame that lands as it goes backward from `position`.
void Ride_SpawnLandingBackward(s32 *position, s32 speed, s32 mode) {
  struct Actor *actor = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                            position[1], position[2]);

  if (actor != NULL) {
    actor->bounce = 0;
    actor->gravity = 0;
    actor->riseBaseY = -speed;
    Actor_SetupScriptedPart(actor, 3);
    actor->update = Flame_UpdateLanding;
    if (mode)
      actor->retainSpriteCache = 2;
    Actor_SelectAnimation(actor, 2);
  }
}
