.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080f8058
	.thumb_func
Func_080f8058:
	overlay_veneer Func_08104ef8
	.global Func_080f8060
	.thumb_func
Func_080f8060:
.ifdef TLA_EDITION_DE
	overlay_veneer MenuIcon_ReleaseObjects
.else
.ifdef TLA_EDITION_ES
	overlay_veneer MenuIcon_ReleaseObjects
.else
.ifdef TLA_EDITION_FR
	overlay_veneer MenuIcon_ReleaseObjects
.else
	overlay_veneer Func_08104fe0
.endif
.endif
.endif
	.global Menu_ReleaseEntryObjectsFar
	.thumb_func
Menu_ReleaseEntryObjectsFar:
.ifdef TLA_EDITION_DE
	overlay_veneer Menu_SetEntryObjectsHidden
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Menu_SetEntryObjectsHidden
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Menu_SetEntryObjectsHidden
.else
	overlay_veneer Menu_ReleaseEntryObjects
.endif
.endif
.endif
	.global Func_080f8070
	.thumb_func
Func_080f8070:
.ifdef TLA_EDITION_DE
	overlay_veneer MenuIcon_ReplaceObject
.else
.ifdef TLA_EDITION_ES
	overlay_veneer MenuIcon_ReplaceObject
.else
.ifdef TLA_EDITION_FR
	overlay_veneer MenuIcon_ReplaceObject
.else
	overlay_veneer Func_081051a8
.endif
.endif
.endif
	.global Func_080f8078
	.thumb_func
Func_080f8078:
.ifdef TLA_EDITION_DE
	overlay_veneer MenuIcon_SetPosition
.else
.ifdef TLA_EDITION_ES
	overlay_veneer MenuIcon_SetPosition
.else
.ifdef TLA_EDITION_FR
	overlay_veneer MenuIcon_SetPosition
.else
	overlay_veneer MenuIcon_ReleaseObjects
.endif
.endif
.endif
	.global Func_080f8080
	.thumb_func
Func_080f8080:
.ifdef TLA_EDITION_DE
	overlay_veneer MenuIcon_SetFrame
.else
.ifdef TLA_EDITION_ES
	overlay_veneer MenuIcon_SetFrame
.else
.ifdef TLA_EDITION_FR
	overlay_veneer MenuIcon_SetFrame
.else
	overlay_veneer Menu_SetEntryObjectsHidden
.endif
.endif
.endif
	.global Func_080f8088
	.thumb_func
Func_080f8088:
.ifdef TLA_EDITION_DE
	overlay_veneer Func_08105350
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Func_08105350
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Func_08105350
.else
	overlay_veneer MenuIcon_ReplaceObject
.endif
.endif
.endif
