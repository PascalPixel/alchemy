@ Far-call stub table at 0x08020180: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
@ The European editions place this table six veneers later, where its first
@ six entries reach the blend script pair and four later map routines.
	.set TABLE_3_EUROPE, 0
	.ifdef TLA_EDITION_DE
	.set TABLE_3_EUROPE, 1
	.endif
	.ifdef TLA_EDITION_ES
	.set TABLE_3_EUROPE, 1
	.endif
	.ifdef TLA_EDITION_FR
	.set TABLE_3_EUROPE, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set TABLE_3_EUROPE, 1
	.endif
	.global Func_08020180
	.thumb_func
Func_08020180:
	.if TABLE_3_EUROPE
	overlay_veneer DisplayBlend_EnableRunScript
	.else
	overlay_veneer Func_0802b63c
	.endif
	.global Func_08020188
	.thumb_func
Func_08020188:
	.if TABLE_3_EUROPE
	overlay_veneer DisplayBlend_StartScript
	.else
	overlay_veneer Func_0802b700
	.endif
	.global Func_08020190
	.thumb_func
Func_08020190:
	.if TABLE_3_EUROPE
	overlay_veneer Func_0802d45c
	.else
	overlay_veneer Func_0802b71c
	.endif
	.global Func_08020198
	.thumb_func
Func_08020198:
	.if TABLE_3_EUROPE
	overlay_veneer GetMapCellCollision
	.else
	overlay_veneer Func_0802cc74
	.endif
	.global Func_080201a0
	.thumb_func
Func_080201a0:
	.if TABLE_3_EUROPE
	overlay_veneer Func_0802d4d8
	.else
	overlay_veneer Func_0802cc88
	.endif
	.global Func_080201a8
	.thumb_func
Func_080201a8:
	.if TABLE_3_EUROPE
	overlay_veneer Func_0802d530
	.else
	overlay_veneer DisplayBlend_DisableRunScript
	.endif
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
	overlay_veneer DisplayBlend_EnableRunScript
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
	overlay_veneer DisplayBlend_StartScript
	.endif
	.endif
	.endif
	.endif
