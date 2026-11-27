/* scSetArrivalFlags, compiled inside each Jupiter Lighthouse scene that calls
   it and inside Mars Lighthouse room F, at its place in each overlay. */
#include "types.h"
#include "flags.h"
#include "field/script.h"

/* Marks the party as inside Jupiter Lighthouse; once Alex has healed the party
   there, also ends the area's random encounters and blocks Retreat. */
void scSetArrivalFlags(Sint32 unused)
{
	syGameFlagSet(FLAG_IN_JUPITER_LIGHTHOUSE);
	if(syGameFlagGet(FLAG_JUPITER_LIGHTHOUSE_ALEX_HEALED_THE_PARTY)) {
		syGameFlagSet(FLAG_NO_ENCOUNTERS_IN_AREA);
		syGameFlagSet(FLAG_RETREAT_BLOCKED);
	}
}
