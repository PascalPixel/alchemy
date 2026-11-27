/* Mercury and Venus Lighthouses' aeries. */

/* How far from the leader a floating platform is looked for, in pixels. */
#define FLOATING_PLATFORM_SEARCH_DISTANCE 640

/* Returns the placed actor drawn as SPRITE_FLOATING_PLATFORM nearest the
   leader, within FLOATING_PLATFORM_SEARCH_DISTANCE, or 0. */
Sint32 scActorFindNearestFloatingPlatform(void)
{
	FIELD_STATE *state = FieldStateWork;
	ACTOR	   **slot;
	ACTOR		*origin;
	Sint32		 nearestActorId = 0;
	Sint32		 minDist;
	Uint32		 actorId;

	minDist = FLOATING_PLATFORM_SEARCH_DISTANCE;
	origin = fdActorGetById(ACTOR_PARTY_LEADER);
	actorId = ACTOR_FIRST_PLACED;
	slot = &state->actors[ACTOR_FIRST_PLACED];
	for(; actorId < sizeof(state->actors) / sizeof(state->actors[0]);
		actorId++) {
		ACTOR *actor = *slot++;
		if(actor != NULL) {
			if(actor->spr->entries[0]->anmId == SPRITE_FLOATING_PLATFORM) {
				Sint32 dist = fdActorDistance(origin, actor);
				if(dist < minDist) {
					minDist = dist;
					nearestActorId = actorId;
				}
			}
		}
	}
	return nearestActorId;
}
