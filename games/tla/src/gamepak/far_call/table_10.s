.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global GetWorldMapCollisionFar
	.thumb_func
GetWorldMapCollisionFar:
	overlay_veneer GetWorldMapCollision
	.global Func_08020360
	.thumb_func
Func_08020360:
	overlay_veneer Func_0802d600
	.global Func_08020368
	.thumb_func
Func_08020368:
	overlay_veneer Func_0802d644
