@ A far-call bank is its list (games/tbs/include/farcall): this file turns each
@ entry into its 8-byte stub, and farcall.h turns the same entry into its C
@ declaration.
#include "../../../../common/include/game/edition.h"
	.syntax unified
	.thumb
	.include "games/tbs/src/system/overlay.inc"
	.macro far_call name, target
	.global \name
	.thumb_func
\name:
	overlay_veneer \target
	.endm
	.macro far_slot target
	overlay_veneer \target
	.endm
#define FAR_CALL(name, target, type, params) far_call name, target
#define FAR_ENTRY(name, target) far_call name, target
#define FAR_SLOT(target) far_slot target
