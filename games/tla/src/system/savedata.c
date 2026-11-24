/// @file
/// The save signature and sector header template, and test data nothing reads.

#include "system/savedata.h"
#include "types.h"

/// The signature that marks a save part: Save_InitializeState counts a sector
/// as a save only when its first seven bytes match it.
const u8 gSave_Signature[] = "CAMELOT";
/// The sector header's first eight bytes, with no terminator: the signature,
/// then a byte in the part id's place that the writer overwrites.
const u8 gSave_HeaderTemplate[8] = "CAMELOTT";

/// Unused: nothing in the game reads it.
const u32 gLink_TestWord = 0;

/// Unused: two test messages left beside the link code; nothing sends them.
const u8 gLink_TestDataA[] = "Trans Data A: This is a pen.";
const u8 gLink_TestDataB[] = "Trans Data B: Hello world.";
