#ifndef ALCHEMY_KOLIMA_CURSE_DUST_H
#define ALCHEMY_KOLIMA_CURSE_DUST_H

/* The dust of Tret's curse that falls about the party in Kolima and at Kolima
   Forest's entrance (shared/kolima_curse_dust.c). */

#include "types.h"

/* Kolima_CurseDustState: how the curse dust falls about the point the map view
   follows (scKolimaUpdateCurseDust). */
typedef enum KOLIMA_CURSE_DUST_STATE {
	KOLIMA_DUST_CENTRED = 0, /* within 128 pixels of the point */
	/* Over a band that widens, by 50 a frame to 15000, as it closes in. */
	KOLIMA_DUST_CLOSING = 1,
	/* Over a band that widens, by 50 a frame to 30000, as it closes in
	   nearer. */
	KOLIMA_DUST_CLOSING_WIDE = 2,
	/* Drifting on until it is far past the point, then back to
	   KOLIMA_DUST_CENTRED. */
	KOLIMA_DUST_DRIFTING = 3
} KOLIMA_CURSE_DUST_STATE;

/* An event that sends down a speck of curse dust every eighth frame. */
void scKolimaUpdateCurseDust(void);

#endif
