.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global ObjectTable_SnapshotFar
	.thumb_func
ObjectTable_SnapshotFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d46a4
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d46a4
.else
	overlay_veneer ObjectTable_Snapshot
.endif
.endif
	.global Func_080c8630
	.thumb_func
Func_080c8630:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d4714
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d4714
.else
	overlay_veneer Func_080cae5c
.endif
.endif
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
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d1ee8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d1ee8
.else
	overlay_veneer Func_080d4714
.endif
.endif
