/* Once a flame or a rolling fireball has hit the leader (the flames' state */
/* active), it carries him along (scFireballCarryLeader); 1 when it did. */
s32 scFireballCarryLeaderIfHit(void) {
	if((FLAME_THROWERS_STATE)->active) {
		scFireballCarryLeader();
		return 1;
	}
	return 0;
}
