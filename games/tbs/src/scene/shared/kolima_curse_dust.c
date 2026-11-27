// Kolima and Kolima Forest (SceneIdKolima and SceneIdKolimaForestEntrance): the sparkling dust that
// falls over the party when Tret's curse strikes ("Watch out! It's happening again!").

#include "macros.h"
#include "field/map_grid_scroll.h"

// Where the dust falls, east of the point the map view follows: the band's west edge, 16.16 pixels
// from that point, and its width, in 256ths of a pixel; the script closes the band in
// (KOLIMA_CURSE_DUST_STATE).
Sint32 KolimaCurseDustOffset BSS_DATA;
Sint32 KolimaCurseDustWidth  BSS_DATA;
Uint32 KolimaCurseDustState  BSS_DATA;

// An event: every eighth tick a speck (its script KolimaCurseDustScript) starts 160 pixels up at a
// random point of the band, or anywhere within 128 pixels of the point the map view follows once
// the dust is centred, with SE_GLITTER every 64 ticks.
void                         scKolimaUpdateCurseDust(void)
{
	ACTOR  *actor = NULL;
	Sint32 *origin;
	SPR    *sprite;
	Sint32  x;
	FIXED   z;

	switch (KolimaCurseDustState)
	{
		case KOLIMA_DUST_CLOSING:
			if (KolimaCurseDustWidth < 15000)
				KolimaCurseDustWidth += 50;
			if (KolimaCurseDustOffset > toFIXED(60))
				KolimaCurseDustOffset -= toFIXED(1) / 4;
			break;
		case KOLIMA_DUST_CLOSING_WIDE:
			if (KolimaCurseDustWidth < 30000)
				KolimaCurseDustWidth += 50;
			if (KolimaCurseDustOffset > toFIXED(24))
				KolimaCurseDustOffset -= toFIXED(1) / 4;
			break;
		case KOLIMA_DUST_DRIFTING:
			if (KolimaCurseDustOffset < -toFIXED(128))
			{
				KolimaCurseDustState = KOLIMA_DUST_CENTRED;
			}
			else
			{
				KolimaCurseDustWidth  += 50;
				KolimaCurseDustOffset -= toFIXED(1) / 4;
			}
			break;
	}
	if ((TickCount & 7) == 0 && (actor = syFieldActorCreate(SPRITE_SPARKLE, 0, 0, 0)) != 0)
	{
		origin = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
		if ((TickCount & 63) == 0)
			sySoundPlay(SE_GLITTER);
		if (KolimaCurseDustState != KOLIMA_DUST_CENTRED)
			x = origin[0] + ((syRandomBelow(KolimaCurseDustWidth)) << 8) + KolimaCurseDustOffset;
		else
			x = origin[0] + (syRandomNext() << 8) - toFIXED(128);
		z                       = origin[2] + (syRandomNext() << 8) - toFIXED(128);
		actor->motionFlags      = 0;
		actor->y.fixed          = toFIXED(160);
		sprite                  = actor->sprite;
		actor->scaleX           = toFIXED(9) / 10;
		actor->scaleY           = toFIXED(9) / 10;
		actor->x.fixed          = x;
		actor->z.fixed          = z;
		sprite->flags           = 0;
		actor->priorityFlags   &= ~ACTOR_PRIORITY_AUTOMATIC;
		sprite->image.priority  = 1;
		fdActorSelectAnimation(actor, 1);
		fdActorStartScript(actor, KolimaCurseDustScript);
	}
}
