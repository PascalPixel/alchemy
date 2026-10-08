.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08020308
	.thumb_func
Func_08020308:
.ifdef TLA_EDITION_JA
	overlay_veneer Func_0802d7b0
.else
.ifdef TLA_EDITION_EN
	overlay_veneer Func_0802d7b0
.else
	overlay_veneer Animation_WaitForTrackEnd
.endif
.endif
	.global Func_08020310
	.thumb_func
Func_08020310:
	overlay_veneer Func_0802d71c
	.global Func_08020318
	.thumb_func
Func_08020318:
.ifdef TLA_EDITION_DE
	overlay_veneer Runtime_CheckRadiusOverlap
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Runtime_CheckRadiusOverlap
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Runtime_CheckRadiusOverlap
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Runtime_CheckRadiusOverlap
.else
	overlay_veneer Func_0802db64
.endif
.endif
.endif
.endif
	.global Func_08020320
	.thumb_func
Func_08020320:
	overlay_veneer Func_0802db88
