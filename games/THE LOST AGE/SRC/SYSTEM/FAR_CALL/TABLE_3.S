@ Far-call stub table at 0x08020180: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020180
	.thumb_func
Func_08020180:
	overlay_veneer Func_0802b63c
	.global Func_08020188
	.thumb_func
Func_08020188:
	overlay_veneer Func_0802b700
	.global Func_08020190
	.thumb_func
Func_08020190:
	overlay_veneer Func_0802b71c
	.global Func_08020198
	.thumb_func
Func_08020198:
	overlay_veneer Func_0802cc74
	.global Func_080201a0
	.thumb_func
Func_080201a0:
	overlay_veneer Func_0802cc88
	.global Func_080201a8
	.thumb_func
Func_080201a8:
	overlay_veneer Func_0802ceb0
	.global Func_080201b0
	.thumb_func
Func_080201b0:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_CopyCellsRect
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_CopyCellsRect
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_CopyCellsRect
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_CopyCellsRect
	.else
	overlay_veneer Func_0802cea0
	.endif
	.endif
	.endif
	.endif
	.global Func_080201b8
	.thumb_func
Func_080201b8:
	.ifdef TLA_EDITION_DE
	overlay_veneer Map_CopyCellAttributeRect
	.else
	.ifdef TLA_EDITION_ES
	overlay_veneer Map_CopyCellAttributeRect
	.else
	.ifdef TLA_EDITION_FR
	overlay_veneer Map_CopyCellAttributeRect
	.else
	.ifdef TLA_EDITION_IT
	overlay_veneer Map_CopyCellAttributeRect
	.else
	overlay_veneer Func_0802ce4c
	.endif
	.endif
	.endif
	.endif
