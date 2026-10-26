@ Bank jump tables use the same 8-byte entries as overlay imports and entries.
@ Each bank lists its exports in assembly order; C interfaces live separately
@ in include/farcall. A named export is a callable symbol; a bare veneer keeps
@ an entry that has no exported name. version.h selects the bank's
@ patch-dependent entries.
#include "../../../include/version.h"
	.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
