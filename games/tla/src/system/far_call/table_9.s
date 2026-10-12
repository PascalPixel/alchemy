.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_08020340
	.thumb_func
Func_08020340:
.ifdef TLA_EDITION_JA
	overlay_veneer Func_080237e8
.else
.ifdef TLA_EDITION_EN
	overlay_veneer Func_080237e8
.else
	overlay_veneer Func_080219cc
.endif
.endif
	.global Runtime_CheckRadiusOverlapFar
	.thumb_func
Runtime_CheckRadiusOverlapFar:
.ifdef TLA_EDITION_DE
	overlay_veneer Func_0802b828
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Func_0802b828
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Func_0802b828
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_0802b828
.else
	overlay_veneer Runtime_CheckRadiusOverlap
.endif
.endif
.endif
.endif
	.global Func_08020350
	.thumb_func
Func_08020350:
.ifdef TLA_EDITION_DE
	overlay_veneer VramBlock_Reset93
.else
.ifdef TLA_EDITION_ES
	overlay_veneer VramBlock_Reset93
.else
.ifdef TLA_EDITION_FR
	overlay_veneer VramBlock_Reset93
.else
.ifdef TLA_EDITION_IT
	overlay_veneer VramBlock_Reset93
.else
	overlay_veneer Func_0802da88
.endif
.endif
.endif
.endif
