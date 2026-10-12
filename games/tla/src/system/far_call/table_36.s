.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.include "games/common/include/game/ed_asm.h"
	.global Func_080c8950
	.thumb_func
Func_080c8950:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080c8950_Target
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080c8950_Target
.else
	overlay_veneer Func_080d072c
.endif
.endif
	.global Func_080c8958
	.thumb_func
Func_080c8958:
	overlay_veneer Func_080d25c8
	.global SceneEvent_FindActiveRecordFar
	.thumb_func
SceneEvent_FindActiveRecordFar:
.if EDITION_INTERNATIONAL
	overlay_veneer Func_080ccd78
.else
	overlay_veneer SceneEvent_FindActiveRecord
.endif
	.global Func_080c8968
	.thumb_func
Func_080c8968:
	overlay_veneer Func_080d1760
