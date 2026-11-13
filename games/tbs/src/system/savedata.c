/// @file
/// The save signature and sector header template, and test data nothing reads.

#include "savedata.h"
#include "types.h"

/// The signature that marks a save part: Save_InitializeWork
/// counts a sector as a save only when its first seven bytes match it.
const u8 gSave_Signature[] = "CAMELOT";
/// The sector header's first eight bytes, with no terminator. The writers
/// copy it over a new header: the seven signature bytes, and a partId byte
/// ('T') they then replace.
const u8 gSave_HeaderTemplate[8] = {'C', 'A', 'M', 'E', 'L', 'O', 'T', 'T'};

/// Nothing in the game reads it.
const u32 gLink_TestWord = 0;

const u8 gLink_TestDataA[] = "Trans Data A: This is a pen.";
const u8 gLink_TestDataB[] = "Trans Data B: Hello world.";
