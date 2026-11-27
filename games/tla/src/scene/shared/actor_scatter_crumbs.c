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
			Sint32 ang, radius;
			Uint32 sum;
			Sint32 scale;
			SPR	  *spr;
			SPR	  *src;

			ang = syRandom();
			radius = syRandom() * 8;
			sum = syRandom() + syRandom();
			scale = (sum >> 2) + toFIXED(1) / 2;
			fdActorStartScript(object, CrumbsScript);
			fdActorSetSprFlags(object, 0);
			object->velY = ang * 4 + toFIXED(2);
			object->velZ = syMulFX(radius, syCos(ang));
			object->velX = syMulFX(radius, sySin(ang));
			object->gravity = toFIXED(33) / 100;
			object->scaleX = scale;
			object->scaleY = scale;
			object->acceleration = 0;
			object->bounce = toFIXED(1) / 2;
			spr = object->spr;
			src = ((ACTOR *)state->sharedFlame)->spr;
			syVRAMCacheFreeEntry(spr->slot);
			spr->retainCacheEntry = 1;
			spr->slot = state->flameVramBlock;
			fdSprAnimationClear(spr->entries[0]);
			spr->entries[0] = NULL;
			spr->cnt = 0;
			spr->width = src->width;
			spr->height = src->height;
			spr->image.shape = src->image.shape;
			spr->image.size = src->image.size;
			spr->image.tile = src->image.tile;
		}
	}
}
