.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Func_08020180
	.thumb_func
Func_08020180:
	overlay_veneer Map_ApplySetFlagEdits
	.global Func_08020188
	.thumb_func
Func_08020188:
	overlay_veneer Map_SetFlagAndApplyEdits
	.global Func_08020190
	.thumb_func
Func_08020190:
	overlay_veneer Map_SetFlagAndApplyAttributeEdits
	.global Func_08020198
	.thumb_func
Func_08020198:
	overlay_veneer Map_ClearLayerEntryFlag
	.global Func_080201a0
	.thumb_func
Func_080201a0:
	overlay_veneer Map_SetLayerEntryFlag
	.global DisplayBlend_DisableRunScriptFar
	.thumb_func
DisplayBlend_DisableRunScriptFar:
	overlay_veneer DisplayBlend_DisableRunScript
	.global DisplayBlend_EnableRunScriptFar
	.thumb_func
DisplayBlend_EnableRunScriptFar:
	overlay_veneer DisplayBlend_EnableRunScript
	.global DisplayBlend_StartScriptFar
	.thumb_func
DisplayBlend_StartScriptFar:
	overlay_veneer DisplayBlend_StartScript
