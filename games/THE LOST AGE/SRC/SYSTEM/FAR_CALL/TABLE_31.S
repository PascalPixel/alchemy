.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8628
	.thumb_func
Func_080c8628:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080dd698
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d4698
.else
	overlay_veneer ObjectTable_Snapshot
.endif
.endif
	.global Func_080c8630
	.thumb_func
Func_080c8630:
	overlay_veneer Func_080cae5c
	.global Func_080c8638
	.thumb_func
Func_080c8638:
.ifdef TLA_EDITION_ES
	overlay_veneer Scene_GetAreaNameIndex
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Scene_GetAreaNameIndex
.else
	overlay_veneer Func_080d46a4
.endif
.endif
	.global Func_080c8640
	.thumb_func
Func_080c8640:
	overlay_veneer Func_080d4714
