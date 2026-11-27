// Vault, on the first visit (vault.c) and once the party has reached Kalay (vault_revisit.c): the
// two children who walk round the well.

// ACTOR_BOY_ROUND_THE_WELL's walk, round eight points for as long as the party stays on the map.
Sint32 VaultBoyRoundTheWellRoute[] = {
	SC_ANIMATION(ANIM_WALK),       SC_SPEED(0.8), SC_ACCELERATION(0.2), SC_LABEL(1),
	SC_MOVE(136, 0, 376),          SC_SYNC,       SC_MOVE(120, 0, 376), SC_SYNC,
	SC_MOVE(104, 0, 392),          SC_SYNC,       SC_MOVE(104, 0, 408), SC_SYNC,
	SC_MOVE(120, 0, 424),          SC_SYNC,       SC_MOVE(136, 0, 424), SC_SYNC,
	SC_MOVE(152, 0, 408),          SC_SYNC,       SC_MOVE(152, 0, 392), SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)), SC_END,
};

// ACTOR_GIRL_ROUND_THE_WELL's walk round the same eight points, half a circle apart.
Sint32 VaultGirlRoundTheWellRoute[] = {
	SC_ANIMATION(ANIM_WALK),       SC_SPEED(0.8), SC_ACCELERATION(0.2), SC_LABEL(1),
	SC_MOVE(120, 0, 424),          SC_SYNC,       SC_MOVE(136, 0, 424), SC_SYNC,
	SC_MOVE(152, 0, 408),          SC_SYNC,       SC_MOVE(152, 0, 392), SC_SYNC,
	SC_MOVE(136, 0, 376),          SC_SYNC,       SC_MOVE(120, 0, 376), SC_SYNC,
	SC_MOVE(104, 0, 392),          SC_SYNC,       SC_MOVE(104, 0, 408), SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)), SC_END,
};
