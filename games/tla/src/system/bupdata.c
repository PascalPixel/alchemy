/* The save signature and sector header template, and test data nothing
   reads. */

#include "types.h"

/* The signature that marks a save part: syBupInitState counts a sector as a
   save only when its first seven bytes match it. */
const Uint8 Bup_Signature[] = "CAMELOT";
/* The sector header's first eight bytes, with no terminator: the signature,
   then a byte in the part id's place that the writer overwrites. */
const Uint8 Bup_HeaderTemplate[8] = "CAMELOTT";

/* Unused: nothing in the game reads it. */
const Uint32 Link_TestWord = 0;

/* Unused: two test messages left beside the link code; nothing sends them. */
const Uint8 Link_TestDataA[] = "Trans Data A: This is a pen.";
const Uint8 Link_TestDataB[] = "Trans Data B: Hello world.";
