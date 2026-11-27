/* fdEffectUpdateSpinAng and fdEffectUpdateRetreatCaster, the same in both
   games. The Broken Seal's field/script_commands_retreat.c and The Lost
   Age's field/field_psynergy_radial_effects.c include it where the functions
   stand. */

/* Per-tick update: spins by FACING_STEP + timer * 80 a tick, speeding up
   (timer counts up) until timer * 80 reaches FACING_STEP: from 1/16 turn a
   tick to just over 1/8 in 52 ticks, as fdEffectAdvanceAngWithRamp. */
void fdEffectUpdateSpinAng(ACTOR *actor)
{
	Uint32 step;

	step = actor->tmr * 80;
	actor->facing = actor->facing + step + FACING_STEP;
	if(step < FACING_STEP)
		actor->tmr = actor->tmr + 1;
}

/* Per-tick update: spins 1/8 turn a tick as it shrinks by 5/512 of its full
   size, rising a pixel a tick; once its scale falls below 3/16 (84 ticks from
   full size) its sprites are hidden, but it is not released. Retreat gives it
   to the caster. */
void fdEffectUpdateRetreatCaster(ACTOR *actor)
{
	Uint32 scale;

	actor->scaleY -= toFIXED(5) / 512;
	scale = actor->scaleX - toFIXED(5) / 512;
	actor->facing += DEGtoANG(45);
	actor->y.fixed += toFIXED(1);
	actor->scaleX = scale;
	if((Sint32)scale < toFIXED(3) / 16)
		actor->sprStorageKind = ACTOR_SPRITE_NONE;
}
