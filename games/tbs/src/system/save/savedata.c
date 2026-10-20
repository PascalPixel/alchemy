#include "savedata.h"
#include "types.h"

/// The signature that marks a save record
const u8 gSave_Signature[] = "CAMELOT";
/// The record header's first eight bytes, with no terminator.
const u8 gSave_HeaderTemplate[8] = {'C', 'A', 'M', 'E', 'L', 'O', 'T', 'T'};

const u32 gLink_TestWord = 0;

/// The data the link test sends
const u8 gLink_TestDataA[] = "Trans Data A: This is a pen.";
const u8 gLink_TestDataB[] = "Trans Data B: Hello world.";
