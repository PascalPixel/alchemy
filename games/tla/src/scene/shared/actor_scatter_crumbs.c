/* The crumbs a fireball throws off as it crumbles away (scActorRollAndCrumble),
   which three Mars Lighthouse rooms keep a copy of, over the flames'
   FLAME_HIT_STATE (scene/flame_throwers.h). */

/* A fireball crumbling away throws off a crumb, a small flame drawn with the
   kept flame's tiles, that bounces off in a random direction and flickers out
   (the includer's CrumbsScript): as the Mars tower's fireball crashes on the
   centre wall, it shrinks away in a spray of little flames. */
void scActorScatterCrumbs(ACTOR *actor)
{
	FLAME_HIT_STATE *state = FLAME_THROWERS_STATE;
	Sint32			 i;

	if(state->sharedFlame == NULL)
		return;
	/* DEAD: a loop of one pass, as the ice chips' loop of six; the ROM keeps
	   its decrement and test. */
	for(i = 0; i < 1; i++) {
		ACTOR *object;
		Sint32 a, b, c, d, e;
		FIXED  x, y, z;

		a = syRandom();
		b = syRandom();
		x = actor->x.fixed + a * 16 - b * 16;
		c = syRandom();
		d = syRandom();
		e = syRandom();
		z = actor->z.fixed + d * 16 - e * 16;
		y = c * 8 + actor->y.fixed + toFIXED(4);
		object = (ACTOR *)syImportActorCreate(SPRITE_UNSEEN_MARKER, x, y, z);
		if(object != NULL) {
			Sint32 angle, radius;
			Uint32 sum;
			Sint32 scale;
			SPR	  *sprite;
			SPR	  *src;

			angle = syRandom();
			radius = syRandom() * 8;
			sum = syRandom() + syRandom();
			scale = (sum >> 2) + toFIXED(1) / 2;
			fdActorStartScript(object, CrumbsScript);
			fdActorSetSpriteFlags(object, 0);
			object->velocityY = angle * 4 + toFIXED(2);
			object->velocityZ = syMulFX(radius, syCos(angle));
			object->velocityX = syMulFX(radius, sySin(angle));
			object->gravity = toFIXED(33) / 100;
			object->scaleX = scale;
			object->scaleY = scale;
			object->acceleration = 0;
			object->bounce = toFIXED(1) / 2;
			sprite = object->sprite;
			src = ((ACTOR *)state->sharedFlame)->sprite;
			syVRAMCacheFreeEntry(sprite->slot);
			sprite->retainCacheEntry = 1;
			sprite->slot = state->flameVramBlock;
			fdSpriteAnimationClear(sprite->entries[0]);
			sprite->entries[0] = NULL;
			sprite->count = 0;
			sprite->width = src->width;
			sprite->height = src->height;
			sprite->image.shape = src->image.shape;
			sprite->image.size = src->image.size;
			sprite->image.tile = src->image.tile;
		}
	}
}
