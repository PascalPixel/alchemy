.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
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
