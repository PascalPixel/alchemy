.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
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
	overlay_veneer Map_RenderAllAnimatedTileFrames
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
