/// A flame thrower's flame that lands as it goes forward from `position`.
void Ride_SpawnLandingForward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}
