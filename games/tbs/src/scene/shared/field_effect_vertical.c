/* Vale and its home, storm and Babi scenes, Sol Sanctum's eruption, the World
   Map and Venus Lighthouse's aerie: the golden ring that rises around someone
   using Psynergy, as the main image's script_commands_arcs.c draws it. The ring
   is two arcs of SPRITE_GOLDEN_RINGS, its back half under the caster and its
   front half, the same arc upside down, in front. */

/* How long a ring lives, in frames, swelling and shrinking over half a sine
   wave. */
#define VERTICAL_EFFECT_FRAMES 32

/* The ring's back half: it rises a pixel a frame on the watched actor, up to 13
   pixels south of it while small. */
static void scFieldEffectRiseRingBackHalf(ACTOR *actor)
{
	ACTOR *anchor;
	Sint32 frame;
	FIXED  amplitude;

	anchor = (ACTOR *)actor->watchedActor;
	actor->tmr = actor->tmr + 1;
	frame = actor->tmr;

	if(frame >= VERTICAL_EFFECT_FRAMES) {
		fdActorRelease(actor);
		return;
	}

	amplitude = sySin(frame * (DEGtoANG(180) / VERTICAL_EFFECT_FRAMES));
	actor->scaleX = amplitude;
	actor->scaleY = amplitude;
	actor->x.fixed = anchor->x.fixed;
	actor->y.fixed += toFIXED(1);
	actor->z.fixed =
		anchor->z.fixed + (toFIXED(1) - amplitude) * 5 + toFIXED(8);
}

/* The ring's front half, the back half's arc upside down, rising with it. */
static void scFieldEffectRiseRingFrontHalf(ACTOR *actor)
{
	ACTOR *anchor;
	Sint32 frame;
	FIXED  amplitude;

	anchor = (ACTOR *)actor->watchedActor;
	actor->tmr = actor->tmr + 1;
	frame = actor->tmr;

	if(frame >= VERTICAL_EFFECT_FRAMES) {
		fdActorRelease(actor);
		return;
	}

	amplitude = sySin(frame * (DEGtoANG(180) / VERTICAL_EFFECT_FRAMES));
	actor->scaleX = amplitude;
	actor->scaleY = -amplitude;
	actor->x.fixed = anchor->x.fixed;
	actor->y.fixed += toFIXED(1);
	actor->z.fixed =
		anchor->z.fixed - (toFIXED(1) - amplitude) * 5 + toFIXED(16);
}
