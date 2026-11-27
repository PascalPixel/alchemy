/* @file */
/* Vale, its home, storm and Babi scenes, Sol Sanctum's eruption and Venus */
/* Lighthouse's aerie: Psynergy shown in an event. One is the villager's in */
/* Vale: "Behold, the power of Psynergy!" */

#include "ability_ids.h"
#include "field/field_sequence.h"

/* Prepares the effect state and spark tiles of Move (ABILITY_MOVE) for the */
/* event's Psynergy. */
void scStartEventPsynergy(void) { fdPsynergyLoadResources(ABILITY_MOVE, 0); }

/* Ends the event's Psynergy and releases its effect state. */
void scEndEventPsynergy(void) { fdPsynergyEnd(); }
