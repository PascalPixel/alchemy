/* @file */
/* Kolima and Kolima Forest (SceneId_Kolima and SceneId_KolimaForestEntrance):
 */
/* the sparkling dust that falls over the party when Tret's curse strikes */
/* ("Watch out! It's happening again!"). */

#include "macros.h"
#include "field/map_grid_scroll.h"

/* Where the dust falls, east of the point the map view follows: the band's */
/* west edge, 16.16 pixels from that point, and its width, in 256ths of a */
/* pixel; the event closes the band in (KOLIMA_CURSE_DUST_STATE). */
s32 Kolima_CurseDustOffset BSS_DATA;
s32 Kolima_CurseDustWidth BSS_DATA;
u32 Kolima_CurseDustState BSS_DATA;

/* A task: every eighth frame a speck (its script Kolima_CurseDustScript) */
/* starts 160 pixels up at a random point of the band, or anywhere within 128 */
/* pixels of the point the map view follows once the dust is centred, with */
/* SE_GLITTER every 64 frames. */
void scKolimaUpdateCurseDust(void) {
	ACTOR *actor = NULL;
	s32 *origin;
	SPR *sprite;
	s32 x;
	FIXED z;

	switch(Kolima_CurseDustState) {
	case KOLIMA_DUST_CLOSING:
		if(Kolima_CurseDustWidth < 15000)
			Kolima_CurseDustWidth += 50;
		if(Kolima_CurseDustOffset > toFIXED(60))
			Kolima_CurseDustOffset -= toFIXED(1) / 4;
		break;
	case KOLIMA_DUST_CLOSING_WIDE:
		if(Kolima_CurseDustWidth < 30000)
			Kolima_CurseDustWidth += 50;
		if(Kolima_CurseDustOffset > toFIXED(24))
			Kolima_CurseDustOffset -= toFIXED(1) / 4;
		break;
	case KOLIMA_DUST_DRIFTING:
		if(Kolima_CurseDustOffset < -toFIXED(128)) {
			Kolima_CurseDustState = KOLIMA_DUST_CENTRED;
		}
		else {
			Kolima_CurseDustWidth += 50;
			Kolima_CurseDustOffset -= toFIXED(1) / 4;
		}
		break;
	}
	if((FrameCount & 7) == 0 &&
	   (actor = syFieldActorCreate(SPRITE_SPARKLE, 0, 0, 0)) != 0) {
		origin = ((MAP_GRID_SCROLL_STATE *)MapGridState[0])->origin;
		if((FrameCount & 63) == 0)
			sySndPlay(SE_GLITTER);
		if(Kolima_CurseDustState != KOLIMA_DUST_CENTRED)
			x = origin[0] + ((syRandomBelow(Kolima_CurseDustWidth)) << 8) +
				Kolima_CurseDustOffset;
		else
			x = origin[0] + (syRandomNext() << 8) - toFIXED(128);
		z = origin[2] + (syRandomNext() << 8) - toFIXED(128);
		actor->motionFlg = 0;
		actor->y.fixed = toFIXED(160);
		sprite = actor->spr;
		actor->scaleX = 9 * FIXED_ONE / 10;
		actor->scaleY = 9 * FIXED_ONE / 10;
		actor->x.fixed = x;
		actor->z.fixed = z;
		sprite->flg = 0;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		sprite->image.pri = 1;
		fdActorSelectAnimation(actor, 1);
		fdActorStartScript(actor, Kolima_CurseDustScript);
	}
}
