.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global BattleActor_SpawnObjectsForListFar
	.thumb_func
BattleActor_SpawnObjectsForListFar:
	overlay_veneer BattleActor_SpawnObjectsForList
	.global Func_08118018
	.thumb_func
Func_08118018:
	overlay_veneer Func_08126944
	.global Func_08118020
	.thumb_func
Func_08118020:
	overlay_veneer Func_08126978
