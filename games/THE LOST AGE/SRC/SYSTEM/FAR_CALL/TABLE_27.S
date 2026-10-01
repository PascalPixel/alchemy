@ Far-call stub table at 0x080c8378: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8378
	.thumb_func
Func_080c8378:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_ComputeBufferSteps
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_ComputeBufferSteps
.else
	overlay_veneer Func_080d170c
.endif
.endif
	.global Func_080c8380
	.thumb_func
Func_080c8380:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StartBufferInterpolation
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StartBufferInterpolation
.else
	overlay_veneer BattleFx_ApplyColorToSourceBuffer
.endif
.endif
	.global Func_080c8388
	.thumb_func
Func_080c8388:
	overlay_veneer Func_080d0c50
