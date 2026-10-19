.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global VramBlock_Reset93Far
	.thumb_func
VramBlock_Reset93Far:
.ifdef TLA_EDITION_DE
	overlay_veneer VramBlock_Load93
.else
.ifdef TLA_EDITION_ES
	overlay_veneer VramBlock_Load93
.else
.ifdef TLA_EDITION_FR
	overlay_veneer VramBlock_Load93
.else
.ifdef TLA_EDITION_IT
	overlay_veneer VramBlock_Load93
.else
	overlay_veneer VramBlock_Reset93
.endif
.endif
.endif
.endif
	.global Func_08020388
	.thumb_func
Func_08020388:
.ifdef TLA_EDITION_DE
	overlay_veneer Func_0802a650
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Func_0802a650
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Func_0802a650
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_0802a650
.else
	overlay_veneer VramBlock_Load93
.endif
.endif
.endif
.endif
	.global Func_08020390
	.thumb_func
Func_08020390:
.ifdef TLA_EDITION_DE
	overlay_veneer Func_0802de8c
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Func_0802de8c
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Func_0802de8c
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_0802de8c
.else
	overlay_veneer Func_0802a650
.endif
.endif
.endif
.endif
	.global Func_08020398
	.thumb_func
Func_08020398:
.ifdef TLA_EDITION_DE
	overlay_veneer Func_08026fc8
.else
.ifdef TLA_EDITION_ES
	overlay_veneer Func_08026fc8
.else
.ifdef TLA_EDITION_FR
	overlay_veneer Func_08026fc8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_08026fc8
.else
	overlay_veneer Func_0802de8c
.endif
.endif
.endif
.endif
