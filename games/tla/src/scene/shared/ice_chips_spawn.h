/* Two chips of ice fly up from a cell the rolling fireball breaks and bounce */
/* away for 20 frames, then flicker 20 more and are gone (IceChips_Script): */
/* SPRITE_WIND_STONE's picture of glowing orbs, painted SPRITE_COLOR_GRB, so */
/* pale cyan and blue. `speed` tips their throw; the fireball passes its */
/* facing. */
void syIceChipsSpawn(s32 x, s32 y, s32 z, s32 speed) {
	s32 i;

	for(i = 0; i < 2; i++) {
		ACTOR *object = scFieldActorCreate(SPRITE_WIND_STONE, x, y, z);

		if(object != NULL) {
			s32 angle;
			s32 rise;
			s32 radius;
			u32 size;
			FIXED scale;

			rise = speed + (syRandom() >> 4) - (syRandom() >> 4);
			angle = DEGtoANG(90) + rise;
			radius = syRandom() * 8;
			size = syRandom();
			fdActorStartScript(object, IceChips_Script);
			fdActorSetSprFlags(object, 0);
			scale = (size >> 1) + (FIXED_ONE / 4);
			object->velY = rise + (toFIXED(5) / 4);
			object->velZ = syMulFX(radius, syCos(angle));
			object->velX = syMulFX(radius, syMathSin(angle));
			object->acceleration = 0;
			object->gravity = toFIXED(33) / 100;
			object->scaleX = scale;
			object->scaleY = object->scaleX;
			object->bounce = FIXED_ONE / 2;
			fdActorSetSprColMode(object, SPRITE_COLOR_GRB);
		}
	}
}
