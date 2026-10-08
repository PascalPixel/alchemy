.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	overlay_veneer Map_ApplySetFlagEdits
	.endif
	.global Func_08020188
	.thumb_func
Func_08020188:
	.if TABLE_3_EUROPE
	overlay_veneer DisplayBlend_StartScript
	.else
	overlay_veneer Map_SetFlagAndApplyEdits
	.endif
	.global Func_08020190
	.thumb_func
Func_08020190:
	.if TABLE_3_EUROPE
	overlay_veneer Func_0802d45c
	.else
	overlay_veneer Map_SetFlagAndApplyAttributeEdits
	.endif
	.global Func_08020198
	.thumb_func
Func_08020198:
	.if TABLE_3_EUROPE
	overlay_veneer GetMapCellCollision
	.else
	overlay_veneer Map_ClearLayerEntryFlag
	.endif
	.global Func_080201a0
	.thumb_func
Func_080201a0:
	.if TABLE_3_EUROPE
	overlay_veneer Func_0802d4d8
	.else
	overlay_veneer Map_SetLayerEntryFlag
	.endif
	.global DisplayBlend_DisableRunScriptFar
	.thumb_func
DisplayBlend_DisableRunScriptFar:
	.if TABLE_3_EUROPE
	overlay_veneer Func_0802d530
	.else
	overlay_veneer DisplayBlend_DisableRunScript
	.endif
	.global DisplayBlend_EnableRunScriptFar
	.thumb_func
DisplayBlend_EnableRunScriptFar:
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
	.global DisplayBlend_StartScriptFar
	.thumb_func
DisplayBlend_StartScriptFar:
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
