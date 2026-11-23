/// Lets an actor fall, waits up to sixty frames for it to land, plays the cue
/// (when not 0) and holds it still ten frames later: a pushed half face or
/// Felix in the Ankohl Ruins, a pillar of Gaia Rock's first cliffs.
void Actor_WaitToLandWithCue(s32 id, s32 cue) {
  s32 frames = 60;
  struct Actor *actor = Actor_Get(id);

  actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (actor->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  actor->motionFlags = 0;
}
