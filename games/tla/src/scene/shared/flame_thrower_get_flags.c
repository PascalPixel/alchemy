/* Flame thrower `index`'s flags: which flame it sends, or none */
/* (FLAME_THROWER_DRY). */
s32 scFlameThrowerGetFlags(s32 index) {
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	return (state->entries + index)->flg;
}
