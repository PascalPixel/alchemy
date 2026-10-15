#include "savedata.h"
#include "types.h"

// The signature that marks a save record
const u8 Save_Signature[] = "CAMELOT";
const u8 Save_HeaderTemplate[8] = "CAMELOTT";

const u32 Link_TestWord = 0;

// The data the link test sends
const u8 Link_TestDataA[] = "Trans Data A: This is a pen.";
const u8 Link_TestDataB[] = "Trans Data B: Hello world.";
