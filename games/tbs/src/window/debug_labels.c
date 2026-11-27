/* The debug screens' labels: the flag and destination browsers' colon and hex
   digits, and the icon browser's headings. */

#include "types.h"

/* The colon after a destination or flag number. */
const Uint8 Menu_ColonString[] = ":";
/* The flag browser's column heading, one hex digit a column. */
const Uint8 Menu_HexDigitsString[] = "0123456789ABCDEF";

/* The icon browser's headings: item icons, Psynergy icons and status
   patterns. */
const Uint8 DebugMenu_ItemTitle[] = "ITEM";
const Uint8 DebugMenu_EnergyTitle[] = "ENERGY";
const Uint8 DebugMenu_StatusTitle[] = "STATUS";
/* The page number's label. */
const Uint8 DebugMenu_PageLabel[] = "PAGE:";
/* Between the first and last number of a page's range. */
const Uint8 DebugMenu_RangeDash[] = "-";
