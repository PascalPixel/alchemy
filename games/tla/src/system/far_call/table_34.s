.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080c8730
	.thumb_func
Func_080c8730:
	overlay_veneer Func_080eab98
	.global Func_080c8738
	.thumb_func
Func_080c8738:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080cdd80
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080cdd80
.else
	overlay_veneer Func_080eaf28
.endif
.endif
	.global Func_080c8740
	.thumb_func
Func_080c8740:
	overlay_veneer Func_080cdc74
	.global Func_080c8748
	.thumb_func
Func_080c8748:
.ifdef TLA_EDITION_ES
	overlay_veneer Object_Spawn
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Object_Spawn
.else
	overlay_veneer Func_080cdd80
.endif
.endif
	.global Func_080c8750
	.thumb_func
Func_080c8750:
	overlay_veneer Func_080eaeb4
	.global Object_SpawnFar
	.thumb_func
Object_SpawnFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Map_TestSpecialTerrainPair
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Map_TestSpecialTerrainPair
.else
	overlay_veneer Object_Spawn
.endif
.endif
	.global Func_080c8760
	.thumb_func
Func_080c8760:
	overlay_veneer Func_080e25e8
	.global Func_080c8768
	.thumb_func
Func_080c8768:
.ifdef TLA_EDITION_ES
	overlay_veneer EventRuntime_GetControlledOwner
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EventRuntime_GetControlledOwner
.else
	overlay_veneer Func_080dbcd8
.endif
.endif
