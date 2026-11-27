#ifndef ALCHEMY_BATTLE_EFFECT_H
#define ALCHEMY_BATTLE_EFFECT_H

/* The battle effects the battle module calls by name, and the canvas blitters
   they draw with. */

#include "edition.h"
#include "fixed_math.h"
#include "types.h"

struct BATTLE_EFFECT_PARM;
void btEffectRunStarfieldFall(struct BATTLE_EFFECT_PARM *effect, Sint32 mode);
/* The battle dissolves into darkness and a sun climbs over a ring of orbs while
   burning drops rain, then the scene bursts in embers and flashes. */
void btEffectRunMeteor(struct BATTLE_EFFECT_PARM *effect);
/* As Meteor, with the user throwing two great flames.

   Abilities: Outer Space. */
void btEffectRunOuterSpace(struct BATTLE_EFFECT_PARM *effect);
void btEffectRunCybele(struct BATTLE_EFFECT_PARM *effect);

/* Fills a size by size/2 byte map with circles of growing radius, each shaded
   by the cosine of its radius (0 to 63) times scale (16.16). */
void btEffectDrawShadedCircles(Uint8 *map, Sint32 size, FIXED scale);
/* btEffectDrawShadedCircles for a size by size map, each circle drawn in all
   four quarters. */
void btEffectDrawShadedCirclesMirrored(Uint8 *map, Sint32 size, Sint32 scale);

void btResourceLoadAndDecompress(
	Sint32 resourceId, void *target, Sint32 unpack, Sint32 wait);

/* The flags of a composite draw: clip the picture at the canvas's edges, and
   mirror it. */
enum {
	COMPOSITE_CLIP_X = 1,
	COMPOSITE_CLIP_Y = 2,
	COMPOSITE_FLIP_X = 4,
	COMPOSITE_FLIP_Y = 8,
	COMPOSITE_FLIP = COMPOSITE_FLIP_X | COMPOSITE_FLIP_Y
};

/* How a blitter btEffectBuildBlitter builds lays a picture on the canvas (its
   mode): BLITTER_ADDITIVE adds the picture's levels to the canvas's, and
   BLITTER_MAX keeps the brighter of the two. */
enum { BLITTER_ADDITIVE, BLITTER_MAX, BLITTER_TRANSPARENT, BLITTER_COPY };

typedef Sint32 (*DRAW_RECTANGLE)(void *destination, const void *source,
	Sint32 x, Sint32 y, Sint32 width, Sint32 height);

#endif
