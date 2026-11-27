/* Two chips of ice fly up from a metatile the rolling fireball breaks and
   bounce away for 20 ticks, then flicker 20 more and are gone
   (IceChipsScript):
   SPRITE_WIND_STONE's picture of glowing orbs, painted SPRITE_COLOR_GRB, so
   pale cyan and blue. `speed` tips their throw; the fireball passes its
   facing. */
void scIceChipsSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 speed)
{
	Sint32 i;

	for(i = 0; i < 2; i++) {
		ACTOR *object = scFieldActorCreate(SPRITE_WIND_STONE, x, y, z);

		if(object != NULL) {
			Sint32 angle;
			Sint32 rise;
			Sint32 radius;
			Uint32 size;
			FIXED  scale;

			rise = speed + (syRandom() >> 4) - (syRandom() >> 4);
			angle = DEGtoANG(90) + rise;
			radius = syRandom() * 8;
			size = syRandom();
			fdActorStartScript(object, IceChipsScript);
			fdActorSetSpriteFlags(object, 0);
			scale = (size >> 1) + (toFIXED(1) / 4);
			object->velocityY = rise + (toFIXED(5) / 4);
			object->velocityZ = syMulFX(radius, syCos(angle));
			object->velocityX = syMulFX(radius, syMathSin(angle));
			object->acceleration = 0;
			object->gravity = toFIXED(33) / 100;
			object->scaleX = scale;
			object->scaleY = object->scaleX;
			object->bounce = toFIXED(1) / 2;
			fdActorSetSpriteColorMode(object, SPRITE_COLOR_GRB);
		}
	}
}
