#ifndef ALCHEMY_TOLBI_SHIP_H
#define ALCHEMY_TOLBI_SHIP_H

/* What the Tolbi-bound Ship's deck (tolbi_ship_deck.c), the rooms below it
   (ship_rooms.c), its mast (tolbi_ship_mast.c) and the World Map's voyage
   (world_map.c) share. */

#include "flags.h"
#include "types.h"

/* The voyage's area flags, cleared when the party enters another area. */

/* A passenger chosen to row follows the party to the rowing deck. */
#define FLAG_TOLBI_SHIP_ROWER_CHOSEN		FLAG_AREA_TEMP_00
/* The hold's stairs are barred while monsters are on the rowing deck. */
#define FLAG_TOLBI_SHIP_MONSTERS_BELOWDECKS FLAG_AREA_TEMP_01
/* The Kraken is on deck, and the deck shakes until it is defeated. */
#define FLAG_TOLBI_SHIP_KRAKEN_ON_DECK		FLAG_AREA_TEMP_02

/* The rowers' balance that takes the ship off course to the island: the rowing
   values of the passengers in the first and third seats (the west seats) less
   those in the second and fourth (scTolbiShipMastGetRowingBalance,
   scWorldMapGetRowingBalance). */
#define ROWING_BALANCE_OFF_COURSE 11

/* A speaker, which is the actor with the speaker bits for where its message
   window and portrait open. */
#define SPEAKER(actorId, bits) ((bits) | (actorId))

/* The rowing seats' values (shared/tolbi_ship_rowing.c). */
extern Uint32 TolbiShipPassengerRowValues[];

#endif
