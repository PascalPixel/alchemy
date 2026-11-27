/* The search command's messages. */

#include "types.h"

/* The message of each fixed search spot, METATILE_BEHAVIOR_JAR to
   METATILE_BEHAVIOR_WELL, as an offset from MsgTileChecked and
   MsgTileResult. */
const Uint8 Field_ExamineCueMessageOffsets[] = {
	2,	/* a jar */
	3,	/* a barrel */
	13, /* a wooden box */
	8,	/* a grave */
	30, /* books */
	31, /* a well */
};
