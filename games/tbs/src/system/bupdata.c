// The save signature and sector header template, and link test data nothing reads.

#include "system.h"

// The signature that marks a save part: syBupInitState counts a sector as a save only when its
// first seven bytes match it.
const Uint8  BupSignature[]       = "CAMELOT";
// The sector header's first eight bytes: the signature, then a part id byte each writer replaces.
const Uint8  BupHeaderTemplate[8] = {'C', 'A', 'M', 'E', 'L', 'O', 'T', 'T'};

// Unused: a word and two strings for a link test, which nothing reads.
const Uint32 LinkTestWord         = 0;
const Uint8  LinkTestDataA[]      = "Trans Data A: This is a pen.";
const Uint8  LinkTestDataB[]      = "Trans Data B: Hello world.";
