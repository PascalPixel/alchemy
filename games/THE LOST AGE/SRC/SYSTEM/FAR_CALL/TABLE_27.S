.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8378
	.thumb_func
Func_080c8378:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_InterpolateBuffers
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_InterpolateBuffers
.else
	overlay_veneer BattleFx_ApplyColorToTargetBuffer
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
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080d0c50
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080d0c50
.else
	overlay_veneer BattleFx_InterpolateBuffers
.endif
.endif
