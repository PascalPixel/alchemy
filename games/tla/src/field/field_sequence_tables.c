/* @file */
/* The search command's messages. */

#include "types.h"

/* The message of each fixed search spot, CELL_BEHAVIOR_JAR to */
/* CELL_BEHAVIOR_WELL, as an offset from MsgTileChecked and MsgTileResult. */
const u8 Field_ExamineTriggerMessageOffsets[] = {
	2,	/* a jar */
	3,	/* a barrel */
	13, /* a wooden box */
	8,	/* a grave */
	30, /* books */
	31, /* a well */
};
