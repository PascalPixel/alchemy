/// @file
/// The debug screens' labels: the flag and destination browsers' colon and
/// hex digits, and the workspace browser's titles.

#include "types.h"

/// The colon after a destination or flag number.
const u8 gMenu_ColonString[] = ":";
/// The flag browser's column heading, one hex digit a column.
const u8 gMenu_HexDigitsString[] = "0123456789ABCDEF";

/// The workspace browser's page titles.
const u8 gWorkspace_ItemTitle[] = "ITEM";
const u8 gWorkspace_EnergyTitle[] = "ENERGY";
const u8 gWorkspace_StatusTitle[] = "STATUS";
/// The page number's label.
const u8 gWorkspace_PageLabel[] = "PAGE:";
/// Between the first and last number of a page's range.
const u8 gWorkspace_RangeDash[] = "-";
