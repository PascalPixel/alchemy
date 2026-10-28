.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Render_ApplyProjectedPlacementFar
	.thumb_func
Render_ApplyProjectedPlacementFar:
	overlay_veneer Render_ApplyProjectedPlacement
	.global Func_08020018
	.thumb_func
Func_08020018:
	overlay_veneer Func_08022318
	.global Func_08020020
	.thumb_func
Func_08020020:
	overlay_veneer Func_0802254c
	.global InitializeAnimationObjectsFar
	.thumb_func
InitializeAnimationObjectsFar:
	overlay_veneer InitializeAnimationObjects
