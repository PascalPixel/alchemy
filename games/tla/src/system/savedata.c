#include "savedata.h"
#include "types.h"

/// The signature that marks a save sector.
const u8 gSave_Signature[] = "CAMELOT";
const u8 gSave_HeaderTemplate[8] = "CAMELOTT";

const u32 gLink_TestWord = 0;

// The data the link test sends.
const u8 gLink_TestDataA[] = "Trans Data A: This is a pen.";
const u8 gLink_TestDataB[] = "Trans Data B: Hello world.";
