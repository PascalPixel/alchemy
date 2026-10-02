@ Far-call stub table at 0x08020120: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08020120
	.thumb_func
Func_08020120:
	overlay_veneer Func_0802aa74
	.global Func_08020128
	.thumb_func
Func_08020128:
	overlay_veneer Func_0802c4d8
	.global Func_08020130
	.thumb_func
Func_08020130:
	overlay_veneer Func_0802be4c
	.global Object_ResetMotion
	.thumb_func
Object_ResetMotion:
	overlay_veneer ObjectDispatch_InitFromTable6
	.global Object_RefreshSelector
	.thumb_func
Object_RefreshSelector:
	overlay_veneer ObjectDispatch_WaitForCommandEnd
	.global Object_SetPosition
	.thumb_func
Object_SetPosition:
	overlay_veneer Object_SetMoveTarget
	.global Object_CommitPosition
	.thumb_func
Object_CommitPosition:
	overlay_veneer Script_WaitForEventTimeout
	.global Func_08020158
	.thumb_func
Func_08020158:
	overlay_veneer Func_08023088
	.global Func_08020160
	.thumb_func
Func_08020160:
	overlay_veneer Func_0802c240
	.global Func_08020168
	.thumb_func
Func_08020168:
	overlay_veneer Func_0802cb08
	.global Func_08020170
	.thumb_func
Func_08020170:
	overlay_veneer Func_0802b2d4
	.global Func_08020178
	.thumb_func
Func_08020178:
	overlay_veneer Map_CopyMetatileIndicesRect
