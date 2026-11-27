/* Flame thrower `idx`'s flags: which flame it sends, or none
   (FLAME_THROWER_DRY). */
Sint32 scFlameThrowerGetFlags(Sint32 idx)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;

	return (state->entries + idx)->flg;
}
