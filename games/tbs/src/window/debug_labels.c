/* The debug screens' labels: the flag and destination browsers' colon and hex
   digits, and the icon browser's headings. */

#include "types.h"

/* The colon after a destination or flag number. */
const Uint8 MenuColonString[] = ":";
/* The flag browser's column heading, one hex digit a column. */
const Uint8 MenuHexDigitsString[] = "0123456789ABCDEF";

/* The icon browser's headings: item icons, Psynergy icons and status
   patterns. */
const Uint8 DebugMenuItemTitle[] = "ITEM";
const Uint8 DebugMenuEnergyTitle[] = "ENERGY";
const Uint8 DebugMenuStatusTitle[] = "STATUS";
/* The page number's label. */
const Uint8 DebugMenuPageLabel[] = "PAGE:";
/* Between the first and last number of a page's range. */
const Uint8 DebugMenuRangeDash[] = "-";
