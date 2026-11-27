/* The save signature and sector header template, and test data nothing
   reads. */

#include "types.h"

/* The signature that marks a save part: syBupInitState counts a sector as a
   save only when its first seven bytes match it. */
const Uint8 BupSignature[] = "CAMELOT";
/* The sector header's first eight bytes, with no terminator: the signature,
   then a byte in the part id's place that the writer overwrites. */
const Uint8 BupHeaderTemplate[8] = "CAMELOTT";

/* Unused: nothing in the game reads it. */
const Uint32 LinkTestWord = 0;

/* Unused: two test messages left beside the link code; nothing sends them. */
const Uint8 LinkTestDataA[] = "Trans Data A: This is a pen.";
const Uint8 LinkTestDataB[] = "Trans Data B: Hello world.";
