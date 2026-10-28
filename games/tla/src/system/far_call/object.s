.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Object_SetPositionAndResetMotionFar
	.thumb_func
Object_SetPositionAndResetMotionFar:
	overlay_veneer Object_SetPositionAndResetMotion
	.global Map_RenderAnimatedTilePictureFar
	.thumb_func
Map_RenderAnimatedTilePictureFar:
	overlay_veneer Map_RenderAnimatedTilePicture
	.global Map_RenderAnimatedTilePicturesForObjectFar
	.thumb_func
Map_RenderAnimatedTilePicturesForObjectFar:
	overlay_veneer Map_RenderAnimatedTilePicturesForObject
	.global Map_RenderAllAnimatedTilePicturesFar
	.thumb_func
Map_RenderAllAnimatedTilePicturesFar:
	overlay_veneer Map_RenderAllAnimatedTilePictures
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
