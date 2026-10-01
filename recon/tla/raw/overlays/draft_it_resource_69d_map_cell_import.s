@ Reduced near miss: the localized scaffold exports the complete map-cell
@ writer through Func_08020330; Func_08020360 is unbound here.
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02001e28
	.thumb_func
Func_02001e28:
	.ifdef TLA_EDITION_DE
	overlay_veneer Func_08020330
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Func_08020330
	.else
	overlay_veneer Func_08020360
	.endif
	.endif
