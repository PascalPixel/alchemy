#ifndef ALCHEMY_TOLBI_SHIP_H
#define ALCHEMY_TOLBI_SHIP_H

/// @file
/// What the Tolbi-bound Ship's deck (tolbi_ship_deck.c) and the rooms below
/// it (ship_rooms.c) share on the voyage.

#include "flags.h"

// The voyage's area flags, cleared when the party enters another area.

/// A passenger chosen to row follows the party to the rowing deck.
#define FLAG_TOLBI_SHIP_ROWER_CHOSEN FLAG_AREA_TEMP_00
/// The hold's stairs are barred while monsters are on the rowing deck.
#define FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS FLAG_AREA_TEMP_01
/// The Kraken is on deck, and the deck shakes until it is defeated.
#define FLAG_TOLBI_SHIP_KRAKEN_ON_DECK FLAG_AREA_TEMP_02

/// A speaker, which is the actor with the speaker bits for where its message
/// window and portrait open.
#define SPEAKER(actorId, bits) ((bits) | (actorId))

#endif
