/// @file
/// Mercury and Venus Lighthouses' aeries.

/// How far from the leader a floating platform is looked for, in pixels.
#define FLOATING_PLATFORM_SEARCH_DISTANCE 640

/// Returns the placed actor drawn as SPRITE_FLOATING_PLATFORM nearest
/// the leader, within FLOATING_PLATFORM_SEARCH_DISTANCE, or 0.
s32 Actor_FindNearestFloatingPlatform(void) {
  struct FieldState *state = gFieldState;
  struct Actor **slot;
  struct Actor *origin;
  s32 nearestActorId = 0;
  s32 minDist;
  u32 actorId;

  minDist = FLOATING_PLATFORM_SEARCH_DISTANCE;
  origin = Actor_GetById(ACTOR_PARTY_LEADER);
  actorId = ACTOR_FIRST_PLACED;
  slot = &state->actors[ACTOR_FIRST_PLACED];
  for (; actorId < sizeof(state->actors) / sizeof(state->actors[0]);
       actorId++) {
    struct Actor *actor = *slot++;
    if (actor != NULL) {
      if (actor->sprite->entries[0]->animationId == SPRITE_FLOATING_PLATFORM) {
        s32 dist = Actor_Distance(origin, actor);
        if (dist < minDist) {
          minDist = dist;
          nearestActorId = actorId;
        }
      }
    }
  }
  return nearestActorId;
}
