/* Vault, before and after the thieves are caught: villagers who walk about stop
   when someone is close in front of them. */

/* Stops the actor (paused, standing) when the target is nearer than range
   pixels and lies ahead of it, within a sixteenth of a turn either side of its
   heading; force stops it regardless. Returns 1 when it stopped. A target out
   of range sets it walking again. */
Sint32 scVaultStopIfFacingNear(
	ACTOR *actor, ACTOR *tgt, Sint32 range, Sint32 force)
{
	Sint32 result = 0;
	if(fdActorDistance(tgt, actor) < range || force != 0) {
		Uint32 ang = (Uint16)syAtan(
			tgt->z.fixed - actor->z.fixed, tgt->x.fixed - actor->x.fixed);
		Uint32 left = FACING_SIXTEENTH(ang - FACING_STEP);
		Uint32 right = FACING_SIXTEENTH(ang + FACING_STEP);
		Uint32 forward = FACING_SIXTEENTH(ang);
		Uint32 facing = FACING_SIXTEENTH(actor->facing);
		if(forward == facing || right == facing || left == facing ||
			force != 0) {
			actor->paused = 1;
			fdActorSelectAnimation(actor, ANIM_STAND);
			result = 1;
		}
	} else {
		actor->paused = 0;
		fdActorSelectAnimation(actor, ANIM_WALK);
	}
	return result;
}
