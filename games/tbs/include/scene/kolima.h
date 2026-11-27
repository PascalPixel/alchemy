#ifndef ALCHEMY_KOLIMA_H
#define ALCHEMY_KOLIMA_H

/* The globals Kolima's scenes share: the falling effect and the party's
   state. */

#include "fixed_math.h"
#include "types.h"

/* The curse dust that falls over the party (scKolimaUpdateCurseDust). In phases
   1 to 3 each speck starts at a random x in a band that begins at the offset
   from the point the map view follows; in phase 0, within 128 pixels of that
   point. Where the band starts, from the followed point, in 16.16 fixed-point
   pixels. */
extern FIXED  Kolima_CurseDustOffset;
extern Sint32 Kolima_CurseDustWidth; /* the band's width in 1/256 pixels */
extern Uint32 Kolima_CurseDustState; /* the dust's phase, 0 to 3 */

/* Set when a curse sparkle bursts on a Psynergy field: the field then takes its
   next pose, once (scKolimaFlashHitPsynergyFields). */
extern Sint32 Kolima_PsynergyFieldHit;
/* Whether Mia was in the party as the curse struck, which decides her part in
   the script; read from FLAG_IN_PARTY(MIA) as it begins. */
extern Sint32 Kolima_MiaInParty;
/* Nonzero while the curse's sparkles fall: each new one plays
   SE_SPARKLE_FALL. */
extern Sint32 Kolima_SparkleSoundOn;

/* The same state in Kolima Forest's entrance (kolima_forest_entrance.c),
   declared after the falling effect's as its bss is laid out. Set by a Psynergy
   field's hit, cleared once answered. */
extern Sint32 KolimaForestEntrance_PsynergyFieldHit;
/* Whether Mia was in the party as the curse struck. */
extern Sint32 KolimaForestEntrance_MiaInParty;
/* Nonzero while the curse's sparkles fall with their sound. */
extern Sint32 KolimaForestEntrance_SparkleSoundOn;

/* Scripts both Kolima scenes have, each in its own overlay. */
extern Sint32 Kolima_PartySquashScript[];
extern Sint32 Kolima_PartyRestoreScaleScript[];
extern Sint32 Kolima_SparkleBurstScript[];
extern Sint32 Kolima_CurseDustScript[];

#endif
