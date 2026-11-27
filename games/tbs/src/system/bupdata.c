/* The save signature and sector header template, and link test data nothing
   reads. */

#include "system.h"

/* The signature that marks a save part: syBupInitState counts a sector as a
   save only when its first seven bytes match it. */
const Uint8 Bup_Signature[] = "CAMELOT";
/* The sector header's first eight bytes: the signature, then a part id byte
   each writer replaces. */
const Uint8 Bup_HeaderTemplate[8] = {'C', 'A', 'M', 'E', 'L', 'O', 'T', 'T'};

/* Unused: a word and two strings for a link test, which nothing reads. */
const Uint32 Link_TestWord = 0;
const Uint8	 Link_TestDataA[] = "Trans Data A: This is a pen.";
const Uint8	 Link_TestDataB[] = "Trans Data B: Hello world.";
