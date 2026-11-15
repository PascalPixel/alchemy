/// @file
/// The search command's messages.

#include "types.h"

/// The message of each searchable cell behavior from 242 to 247, as an
/// offset from MsgTileChecked and MsgTileResult.
const u8 gField_ExamineTriggerMessageOffsets[] = {
    2,  // a jar
    3,  // a barrel
    13, // a wooden box
    8,  // a grave
    30, // books
    31, // a well
};
