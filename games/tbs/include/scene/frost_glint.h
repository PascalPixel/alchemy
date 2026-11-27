#ifndef ALCHEMY_FROST_GLINT_H
#define ALCHEMY_FROST_GLINT_H

/* Altin Peak's frozen puddle and the main image's Frost (frost.c), each with
   its own colour-mode function. */

/* An update function that makes a frozen thing glint: its colour mode is
   SPRITE_COLOR_RBG for two ticks in every eight. */
#define DEFINE_FROST_GLINT(linkage, setColorMode) \
	linkage void fdEffectUpdateFrostGlint(struct ACTOR *actor) \
	{ \
		Sint32 tick = TickCount & 7; \
		if(tick == 0) { \
			setColorMode(actor, SPRITE_COLOR_RBG); \
		} else if(tick == 2) { \
			setColorMode(actor, SPRITE_COLOR_NORMAL); \
		} \
	}

#endif
