#ifndef ALCHEMY_KOLIMA_H
#define ALCHEMY_KOLIMA_H

// The globals Kolima's scenes share: the falling effect and the party's state.

#include "fixed_math.h"
#include "types.h"

// The curse dust that falls over the party (scKolimaUpdateCurseDust). In phases 1 to 3 each speck
// starts at a random x in a band that begins at the offset from the point the map view follows; in
// phase 0, within 128 pixels of that point. Where the band starts, from the followed point,
// in 16.16 fixed-point pixels.
extern FIXED  KolimaCurseDustOffset;
extern Sint32 KolimaCurseDustWidth; // the band's width in 1/256 pixels
extern Uint32 KolimaCurseDustState; // the dust's phase, 0 to 3

// Set when a curse sparkle bursts on a Psynergy field: the field then takes its next pose, once
// (scKolimaFlashHitPsynergyFields).
extern Sint32 KolimaPsynergyFieldHit;
// Whether Mia was in the party as the curse struck, which decides her part in the script; read from
// FLAG_IN_PARTY(MIA) as it begins.
extern Sint32 KolimaMiaInParty;
// Nonzero while the curse's sparkles fall: each new one plays SE_SPARKLE_FALL.
extern Sint32 KolimaSparkleSoundOn;

// The same state in Kolima Forest's entrance (kolima_forest_entrance.c), declared after the falling
// effect's as its bss is laid out. Set by a Psynergy field's hit, cleared once answered.
extern Sint32 KolimaForestEntrancePsynergyFieldHit;
// Whether Mia was in the party as the curse struck.
extern Sint32 KolimaForestEntranceMiaInParty;
// Nonzero while the curse's sparkles fall with their sound.
extern Sint32 KolimaForestEntranceSparkleSoundOn;

// Scripts both Kolima scenes have, each in its own overlay.
extern Sint32 KolimaPartySquashScript[];
extern Sint32 KolimaPartyRestoreScaleScript[];
extern Sint32 KolimaSparkleBurstScript[];
extern Sint32 KolimaCurseDustScript[];

#endif
