.syntax unified
	.thumb
	.global BattleActor_SpawnObjectsForList
	.thumb_func
BattleActor_SpawnObjectsForList:
	push {lr}
	movs r2, #1
	bl Func_0811b75c
	pop {pc}
	.2byte 0x0000
