.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08020328
	.thumb_func
Func_08020328:
	overlay_veneer Func_0802b7e0
	.global Func_08020330
	.thumb_func
Func_08020330:
.ifdef TLA_EDITION_JA
	overlay_veneer Func_080237c8
.else
.ifdef TLA_EDITION_EN
	overlay_veneer Func_080237c8
.else
	overlay_veneer Func_0802d600
.endif
.endif
	.global Animation_WaitForTrackEndFar
	.thumb_func
Animation_WaitForTrackEndFar:
.ifdef TLA_EDITION_JA
	overlay_veneer Animation_WaitForTrackEnd
.else
.ifdef TLA_EDITION_EN
	overlay_veneer Animation_WaitForTrackEnd
.else
	overlay_veneer Func_0802d644
.endif
.endif
