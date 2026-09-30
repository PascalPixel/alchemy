@ Far-call stub table at 0x080200e8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Object_SetPositionAndResetMotionFar
	.thumb_func
Object_SetPositionAndResetMotionFar:
	overlay_veneer Object_SetPositionAndResetMotion
	.global Map_RenderAnimatedTileFrameFar
	.thumb_func
Map_RenderAnimatedTileFrameFar:
	overlay_veneer Map_RenderAnimatedTileFrame
	.global Map_RenderAnimatedTileFramesForObjectFar
	.thumb_func
Map_RenderAnimatedTileFramesForObjectFar:
	overlay_veneer Map_RenderAnimatedTileFramesForObject
	.global Map_RenderAllAnimatedTileFramesFar
	.thumb_func
Map_RenderAllAnimatedTileFramesFar:
	overlay_veneer Func_0802301c
	.global Func_08020108
	.thumb_func
Func_08020108:
	overlay_veneer Func_0802a6b8
	.global Func_08020110
	.thumb_func
Func_08020110:
	overlay_veneer Func_0802b998
	.global Func_08020118
	.thumb_func
Func_08020118:
	overlay_veneer Func_0802af9c
