.syntax unified
	.thumb
	.global Func_0811a484
	.thumb_func
Func_0811a484:
	push {lr}
	bl GetBattleObjectSlot
	ldr r0, [r0, #20]
	pop {pc}
	.2byte 0x0000
